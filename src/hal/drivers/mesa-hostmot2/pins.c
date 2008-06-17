
//
//    Copyright (C) 2007-2008 Sebastian Kuzminsky
//
//    This program is free software; you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation; either version 2 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with this program; if not, write to the Free Software
//    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301 USA
//

#include <linux/slab.h>

#include "rtapi.h"
#include "rtapi_app.h"
#include "rtapi_string.h"
#include "rtapi_math.h"

#include "hal.h"

#include "hal/drivers/mesa-hostmot2/hostmot2.h"




int debug_pin_descriptors = 0;
RTAPI_MP_INT(debug_pin_descriptors, "Developer/debug use only!  Enable debug logging of the HostMot2\nPin Descriptors.");




// FIXME: the static automatic string makes this function non-reentrant
static const char* hm2_get_pin_secondary_name(hm2_pin_t *pin) {
    static char unknown[100];
    int sec_pin = pin->sec_pin & 0x7F;  // turn off the "pin is an output" bit

    switch (pin->sec_tag) {

        case HM2_GTAG_ENCODER:
            switch (sec_pin) {
                case 1: return "A";
                case 2: return "B";
                case 3: return "Index";
            }
            break;

        case HM2_GTAG_PWMGEN:
            switch (sec_pin) {
                case 1: return "Out0 (PWM or Up)";
                case 2: return "Out1 (Dir or Down)";
                case 3: return "Not-Enable";
            }
            break;

        case HM2_GTAG_STEPGEN:
            // FIXME: these depend on the stepgen mode
            switch (sec_pin) {
                case 1: return "Step";
                case 2: return "Direction";
                case 3: return "(unused)";
                case 4: return "(unused)";
                case 5: return "(unused)";
                case 6: return "(unused)";
            }
            break;

    }

    rtapi_snprintf(unknown, sizeof(unknown), "unknown-pin-%d", sec_pin & 0x7F);
    return unknown;
}




static void hm2_print_pin_descriptors(int msg_level, hostmot2_t *hm2) {
    int i;

    PRINT(msg_level, "%d HM2 Pin Descriptors:\n", hm2->num_pins);

    for (i = 0; i < hm2->num_pins; i ++) {
        PRINT(msg_level, "    pin %d:\n", i);
        PRINT(
            msg_level,
            "        Secondary Pin: 0x%02X (%s, %s)\n",
            hm2->pin[i].sec_pin,
            hm2_get_pin_secondary_name(&hm2->pin[i]),
            ((hm2->pin[i].sec_pin & 0x80) ? "Output" : "Input")
        );
        PRINT(
            msg_level,
            "        Secondary Tag: 0x%02X (%s)\n",
            hm2->pin[i].sec_tag,
            hm2_get_general_function_name(hm2->pin[i].sec_tag)
        );
        PRINT(msg_level, "        Secondary Unit: 0x%02X\n", hm2->pin[i].sec_unit);
        PRINT(
            msg_level,
            "        Primary Tag: 0x%02X (%s)\n",
            hm2->pin[i].primary_tag,
            hm2_get_general_function_name(hm2->pin[i].primary_tag)
        );
    }
}




int hm2_read_pin_descriptors(hostmot2_t *hm2) {
    int i;
    int addr = hm2->idrom_offset + hm2->idrom.offset_to_pin_desc;

    i = 0;

    do {
        u32 d;

        if (!hm2->llio->read(hm2->llio, addr, &d, sizeof(u32))) {
            ERR("error reading Pin Descriptor %d (at 0x%04x)\n", i, addr); 
            return -EIO;
        }

        hm2->pin[i].sec_pin     = (d >>  0) & 0x000000FF;
        hm2->pin[i].sec_tag     = (d >>  8) & 0x000000FF;
        hm2->pin[i].sec_unit    = (d >> 16) & 0x000000FF;
        hm2->pin[i].primary_tag = (d >> 24) & 0x000000FF;

        if (hm2->pin[i].primary_tag == 0) {
            hm2->num_pins = i;
            break;
        }

        if (hm2->pin[i].primary_tag != HM2_GTAG_IOPORT) {
            ERR(
                "pin %d primary tag is %d (%s), not IOPort!\n",
                i,
                hm2->pin[i].primary_tag,
                hm2_get_general_function_name(hm2->pin[i].primary_tag)
            );
            return -EINVAL;
        }

        hm2->pin[i].gtag = hm2->pin[i].primary_tag;

        i++;
        addr += 4;
    } while (i < HM2_MAX_PIN_DESCRIPTORS);


    if (hm2->num_pins != hm2->idrom.io_width) {
        ERR("there are %d Pin Descriptors but IDROM IO_Width is %d!\n", hm2->num_pins, hm2->idrom.io_width);
        return -EINVAL;
    }

    if (debug_pin_descriptors) {
        hm2_print_pin_descriptors(RTAPI_MSG_DBG, hm2);
    }

    return 0;
}




static void hm2_set_pin_source(hostmot2_t *hm2, int pin_number, int source) {
    int ioport_number;
    int bit_number;

    ioport_number = pin_number / 24;
    bit_number = pin_number % 24;

    if ((pin_number < 0) || (ioport_number > hm2->ioport.num_instances)) {
        WARN("invalid pin number %d\n", pin_number);
        return;
    }

    if (source == HM2_PIN_SOURCE_IS_PRIMARY) {
        hm2->ioport.alt_source_reg[ioport_number] &= ~(1 << bit_number);
        hm2->pin[pin_number].gtag = hm2->pin[pin_number].primary_tag;
    } else if (source == HM2_PIN_SOURCE_IS_SECONDARY) {
        hm2->ioport.alt_source_reg[ioport_number] |= (1 << bit_number);
        hm2->pin[pin_number].gtag = hm2->pin[pin_number].sec_tag;
    } else {
        WARN("invalid pin source 0x%08X\n", source);
        return;
    }
}




void hm2_set_pin_direction(hostmot2_t *hm2, int pin_number, int direction) {
    int ioport_number;
    int bit_number;

    ioport_number = pin_number / 24;
    bit_number = pin_number % 24;

    if ((pin_number < 0) || (ioport_number > hm2->ioport.num_instances)) {
        WARN("invalid pin number %d\n", pin_number);
        return;
    }

    if (direction == HM2_PIN_DIR_IS_INPUT) {
        hm2->ioport.ddr_reg[ioport_number] &= ~(1 << bit_number);
    } else if (direction == HM2_PIN_DIR_IS_OUTPUT) {
        hm2->ioport.ddr_reg[ioport_number] |= (1 << bit_number);
    } else {
        WARN("invalid pin direction 0x%08X\n", direction);
    }
}




void hm2_print_pin_usage(int msg_level, hostmot2_t *hm2) {
    int i;

    PRINT(msg_level, "%d I/O Pins used:\n", hm2->num_pins);

    for (i = 0; i < hm2->num_pins; i ++) {
        int port = i / hm2->idrom.port_width;

        if (hm2->pin[i].gtag == hm2->pin[i].sec_tag) {
            PRINT(
                msg_level,
                "    I/O Pin %s.%03d: %s #%d, pin %s (%s)\n",
                hm2->llio->ioport_connector_name[port],
                i,
                hm2_get_general_function_name(hm2->pin[i].gtag),
                hm2->pin[i].sec_unit,
                hm2_get_pin_secondary_name(&hm2->pin[i]),
                ((hm2->pin[i].sec_pin & 0x80) ? "Output" : "Input")
            );
        } else {
            PRINT(
                msg_level,
                "    I/O Pin %s.%03d: %s\n",
                hm2->llio->ioport_connector_name[port],
                i,
                hm2_get_general_function_name(hm2->pin[i].gtag)
            );
        }
    }
}




// all pins whose secondary_tag == gtag and whose
// secondary_unit < num_instances get their source set to secondary and
// their pin direction updated to match
static void hm2_pins_allocate(hostmot2_t *hm2, int gtag, int num_instances) {
    int i;

    for (i = 0; i < hm2->num_pins; i ++) {
        if (
            (hm2->pin[i].sec_tag == gtag)
            && (hm2->pin[i].sec_unit < num_instances)
        ) {
            hm2_set_pin_source(hm2, i, HM2_PIN_SOURCE_IS_SECONDARY);
            if (hm2->pin[i].sec_pin & 0x80){
                hm2_set_pin_direction(hm2, i, HM2_PIN_DIR_IS_OUTPUT);
            }
        }
    }
}




// sets up all the IOPort instances, return 0 on success, -errno on failure
void hm2_configure_pins(hostmot2_t *hm2) {

    // 
    // the bits in the alt_source register of the ioport function say
    // whether *output* data comes from the primary source (ioport
    // function) (0) or the secondary source (1)
    // 
    // the bits in the data direction register say whether the pins are
    // inputs (0) or outputs (1)
    // 
    // if a pin is marked as an input in the ddr, it can be used for its
    // function (encoder, say) *and* as a digital input pin without
    // conflict, but (FIXME) the driver does not yet support this
    // 
    // Each function instance that is not disabled by the relevant
    // num_<functions> modparam has all its pins marked 1 in the alt_source
    // register.  The driver uses this to to keep track of which pins are
    // "allocated" to functions and which pins are available for use as
    // gpios.
    // 


    hm2_pins_allocate(hm2, HM2_GTAG_ENCODER, hm2->encoder.num_instances);
    hm2_pins_allocate(hm2, HM2_GTAG_PWMGEN,  hm2->pwmgen.num_instances);
    hm2_pins_allocate(hm2, HM2_GTAG_STEPGEN, hm2->stepgen.num_instances);

    // anything not allocated remains a gpio input
}

