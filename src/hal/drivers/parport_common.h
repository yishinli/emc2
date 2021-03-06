#ifndef HAL_PARPORT_COMMON_H
#define HAL_PARPORT_COMMON_H

#include <linux/parport.h>

typedef struct hal_parport_t
{
    unsigned short base;
    unsigned short base_hi;
    struct pardevice *linux_dev;
    void *region;
    void *region_hi;
} hal_parport_t;

static int
hal_parport_get(int comp_id, hal_parport_t *port,
        unsigned short base, unsigned short base_hi, unsigned int modes)
{
    int retval = 0;
    struct parport *linux_port = 0;
    memset(port, 0, sizeof(hal_parport_t));

    // I/O addresses 1..16 are assumed to be linux parport numbers
    if(base < 16) {
        linux_port = parport_find_number(base);
        if(!linux_port)
        {
            rtapi_print_msg(RTAPI_MSG_ERR,
                    "PARPORT: ERROR: linux parport %d not found\n",
                    base);
            return -ENODEV;
        }
    } else {
        linux_port = parport_find_base(base);
    }

    if(linux_port)
    {
        if((modes & linux_port->modes) != modes)
        {
            rtapi_print_msg(RTAPI_MSG_ERR,
                "PARPORT: ERROR: linux parport %s does not support mode %x\n",
                linux_port->name, modes);
            return -ENODEV;
        }
        rtapi_print_msg(RTAPI_MSG_INFO,
                  "PARPORT: Using Linux parport %s at ioaddr=0x%lx:0x%lx\n",
                  linux_port->name, linux_port->base, linux_port->base_hi);
        port->linux_dev = parport_register_device(linux_port,
                hal_comp_name(comp_id), NULL, NULL, NULL, 0, NULL);
        parport_put_port(linux_port);

        if(!port->linux_dev)
        {
            rtapi_print_msg(RTAPI_MSG_ERR,
                "PARPORT: ERROR: port %s register failed\n", linux_port->name);
            return -EIO;
        }

        retval = parport_claim(port->linux_dev);
        if(retval < 0)
        {
            parport_unregister_device(port->linux_dev);
            rtapi_print_msg(RTAPI_MSG_ERR,
                "PARPORT: ERROR: port %s claim failed\n", linux_port->name);
            return retval;
        }

        port->base = linux_port->base;
        port->base_hi = linux_port->base_hi;
    } else {
        if(base_hi == 0) base_hi = base + 0x400;

        port->base = base;
        rtapi_print_msg(RTAPI_MSG_INFO,
                  "Using direct parport at ioaddr=0x%x:0x%x\n", base, base_hi);

        // SPP access needs only 3 bytes, but EPP needs 8.  Allocating 8
        // is likely to always be OK, and it simplifies things (since the
        // exact allocation size is also needed at deallocation).
        port->region = rtapi_request_region(base, 8, hal_comp_name(comp_id));
        if(!port->region)
        {
            rtapi_print_msg(RTAPI_MSG_ERR,
                "PARPORT: ERROR: request_region(0x%x) failed\n", base);
            return -EBUSY;
        }

        if(base_hi != (unsigned short)-1)
        {
            port->base_hi = base_hi;
            port->region_hi =
                rtapi_request_region(base_hi, 3, hal_comp_name(comp_id));
            if(!port->region_hi)
            {
                rtapi_print_msg(RTAPI_MSG_ERR,
                    "PARPORT: ERROR: request_region(0x%x) failed\n", base_hi);
                rtapi_release_region(port->base, 8);
                return -EBUSY;
            }
        }
    }
    return 0;
}

void hal_parport_release(hal_parport_t *port)
{
    if(port->linux_dev)
    {
        rtapi_print_msg(RTAPI_MSG_INFO,
	    "PARPORT: Releasing Linux parport at ioaddr=0x%lx:0x%lx\n",
	    port->linux_dev->port->base, port->linux_dev->port->base_hi);
        parport_release(port->linux_dev);
        parport_unregister_device(port->linux_dev);
    }
    if(port->region) {
        rtapi_print_msg(RTAPI_MSG_INFO,
	    "PARPORT: Releasing I/O region ioaddr=0x%x\n", port->base);
	rtapi_release_region(port->base, 8);
    }
    if(port->region_hi) {
        rtapi_print_msg(RTAPI_MSG_INFO,
	    "PARPORT: Releasing high I/O region ioaddr=0x%x\n",
                port->base_hi);
	rtapi_release_region(port->base_hi, 3);
    }
    memset(port, 0, sizeof(hal_parport_t));
}
#endif
