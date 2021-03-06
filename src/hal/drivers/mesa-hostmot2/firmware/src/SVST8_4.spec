config_pins = 72

module_id =  """
(
(WatchDogTag,	x"00",	ClockLowTag,	x"01",	WatchDogTimeAddr&PadT,	WatchDogNumRegs,	x"00",	WatchDogMPBitMask),
(IOPortTag,	x"00",	ClockLowTag,	x"03",	PortAddr&PadT,		IOPortNumRegs,		x"00",	IOPortMPBitMask),
(QcountTag,	x"02",	ClockLowTag,	x"08",	QcounterAddr&PadT,	QCounterNumRegs,	x"00",	QCounterMPBitMask),
(PWMTag,	x"00",	ClockHighTag,	x"08",	PWMValAddr&PadT,	PWMNumRegs,		x"00",	PWMMPBitMask),
(StepGenTag,	x"00",	ClockLowTag,	x"04",	StepGenRateAddr&PadT,	StepGenNumRegs,		x"00",	StepGenMPBitMask),
(LEDTag,	x"00",	ClockLowTag,	x"01",	LEDAddr&PadT,		LEDNumRegs,		x"00",	LEDMPBitMask),
(NullTag,	x"00",	NullTag,	x"00",	NullAddr&PadT,		x"00",			x"00",	x"00000000"),
(NullTag,	x"00",	NullTag,	x"00",	NullAddr&PadT,		x"00",			x"00",	x"00000000"),
(NullTag,	x"00",	NullTag,	x"00",	NullAddr&PadT,		x"00",			x"00",	x"00000000"),
(NullTag,	x"00",	NullTag,	x"00",	NullAddr&PadT,		x"00",			x"00",	x"00000000"),
(NullTag,	x"00",	NullTag,	x"00",	NullAddr&PadT,		x"00",			x"00",	x"00000000"),
(NullTag,	x"00",	NullTag,	x"00",	NullAddr&PadT,		x"00",			x"00",	x"00000000"),
(NullTag,	x"00",	NullTag,	x"00",	NullAddr&PadT,		x"00",			x"00",	x"00000000"),
(NullTag,	x"00",	NullTag,	x"00",	NullAddr&PadT,		x"00",			x"00",	x"00000000"),
(NullTag,	x"00",	NullTag,	x"00",	NullAddr&PadT,		x"00",			x"00",	x"00000000"),
(NullTag,	x"00",	NullTag,	x"00",	NullAddr&PadT,		x"00",			x"00",	x"00000000"),
(NullTag,	x"00",	NullTag,	x"00",	NullAddr&PadT,		x"00",			x"00",	x"00000000"),
(NullTag,	x"00",	NullTag,	x"00",	NullAddr&PadT,		x"00",			x"00",	x"00000000"),
(NullTag,	x"00",	NullTag,	x"00",	NullAddr&PadT,		x"00",			x"00",	x"00000000"),
(NullTag,	x"00",	NullTag,	x"00",	NullAddr&PadT,		x"00",			x"00",	x"00000000"),
(NullTag,	x"00",	NullTag,	x"00",	NullAddr&PadT,		x"00",			x"00",	x"00000000"),
(NullTag,	x"00",	NullTag,	x"00",	NullAddr&PadT,		x"00",			x"00",	x"00000000"),
(NullTag,	x"00",	NullTag,	x"00",	NullAddr&PadT,		x"00",			x"00",	x"00000000"),
(NullTag,	x"00",	NullTag,	x"00",	NullAddr&PadT,		x"00",			x"00",	x"00000000"),
(NullTag,	x"00",	NullTag,	x"00",	NullAddr&PadT,		x"00",			x"00",	x"00000000"),
(NullTag,	x"00",	NullTag,	x"00",	NullAddr&PadT,		x"00",			x"00",	x"00000000"),
(NullTag,	x"00",	NullTag,	x"00",	NullAddr&PadT,		x"00",			x"00",	x"00000000"),
(NullTag,	x"00",	NullTag,	x"00",	NullAddr&PadT,		x"00",			x"00",	x"00000000"),
(NullTag,	x"00",	NullTag,	x"00",	NullAddr&PadT,		x"00",			x"00",	x"00000000"),
(NullTag,	x"00",	NullTag,	x"00",	NullAddr&PadT,		x"00",			x"00",	x"00000000"),
(NullTag,	x"00",	NullTag,	x"00",	NullAddr&PadT,		x"00",			x"00",	x"00000000"),
(NullTag,	x"00",	NullTag,	x"00",	NullAddr&PadT,		x"00",			x"00",	x"00000000")
)
"""

pin_desc = """
(
-- Base-func  sec-unit sec-func sec-pin
IOPortTag & x"01" & QCountTag & x"02",
IOPortTag & x"01" & QCountTag & x"01",
IOPortTag & x"00" & QCountTag & x"02",
IOPortTag & x"00" & QCountTag & x"01",
IOPortTag & x"01" & QCountTag & x"03",
IOPortTag & x"00" & QCountTag & x"03",
IOPortTag & x"01" & PWMTag & x"81",
IOPortTag & x"00" & PWMTag & x"81",
IOPortTag & x"01" & PWMTag & x"82",
IOPortTag & x"00" & PWMTag & x"82",
IOPortTag & x"01" & PWMTag & x"83",
IOPortTag & x"00" & PWMTag & x"83",
IOPortTag & x"03" & QCountTag & x"02",
IOPortTag & x"03" & QCountTag & x"01",
IOPortTag & x"02" & QCountTag & x"02",
IOPortTag & x"02" & QCountTag & x"01",
IOPortTag & x"03" & QCountTag & x"03",
IOPortTag & x"02" & QCountTag & x"03",
IOPortTag & x"03" & PWMTag & x"81",
IOPortTag & x"02" & PWMTag & x"81",
IOPortTag & x"03" & PWMTag & x"82",
IOPortTag & x"02" & PWMTag & x"82",
IOPortTag & x"03" & PWMTag & x"83",
IOPortTag & x"02" & PWMTag & x"83",

IOPortTag & x"05" & QCountTag & x"02",
IOPortTag & x"05" & QCountTag & x"01",
IOPortTag & x"04" & QCountTag & x"02",
IOPortTag & x"04" & QCountTag & x"01",
IOPortTag & x"05" & QCountTag & x"03",
IOPortTag & x"04" & QCountTag & x"03",
IOPortTag & x"05" & PWMTag & x"81",
IOPortTag & x"04" & PWMTag & x"81",
IOPortTag & x"05" & PWMTag & x"82",
IOPortTag & x"04" & PWMTag & x"82",
IOPortTag & x"05" & PWMTag & x"83",
IOPortTag & x"04" & PWMTag & x"83",
IOPortTag & x"07" & QCountTag & x"02",
IOPortTag & x"07" & QCountTag & x"01",
IOPortTag & x"06" & QCountTag & x"02",
IOPortTag & x"06" & QCountTag & x"01",
IOPortTag & x"07" & QCountTag & x"03",
IOPortTag & x"06" & QCountTag & x"03",
IOPortTag & x"07" & PWMTag & x"81",
IOPortTag & x"06" & PWMTag & x"81",
IOPortTag & x"07" & PWMTag & x"82",
IOPortTag & x"06" & PWMTag & x"82",
IOPortTag & x"07" & PWMTag & x"83",
IOPortTag & x"06" & PWMTag & x"83",

IOPortTag & x"00" & StepGenTag & x"81",
IOPortTag & x"00" & StepGenTag & x"82",
IOPortTag & x"00" & StepGenTag & x"83",
IOPortTag & x"00" & StepGenTag & x"84",
IOPortTag & x"00" & StepGenTag & x"85",
IOPortTag & x"00" & StepGenTag & x"86",
IOPortTag & x"01" & StepGenTag & x"81",
IOPortTag & x"01" & StepGenTag & x"82",
IOPortTag & x"01" & StepGenTag & x"83",
IOPortTag & x"01" & StepGenTag & x"84",
IOPortTag & x"01" & StepGenTag & x"85",
IOPortTag & x"01" & StepGenTag & x"86",
IOPortTag & x"02" & StepGenTag & x"81",
IOPortTag & x"02" & StepGenTag & x"82",
IOPortTag & x"02" & StepGenTag & x"83",
IOPortTag & x"02" & StepGenTag & x"84",
IOPortTag & x"02" & StepGenTag & x"85",
IOPortTag & x"02" & StepGenTag & x"86",
IOPortTag & x"03" & StepGenTag & x"81",
IOPortTag & x"03" & StepGenTag & x"82",
IOPortTag & x"03" & StepGenTag & x"83",
IOPortTag & x"03" & StepGenTag & x"84",
IOPortTag & x"03" & StepGenTag & x"85",
IOPortTag & x"03" & StepGenTag & x"86",

emptypin,emptypin,emptypin,emptypin,emptypin,emptypin,emptypin,emptypin,
emptypin,emptypin,emptypin,emptypin,emptypin,emptypin,emptypin,emptypin,
emptypin,emptypin,emptypin,emptypin,emptypin,emptypin,emptypin,emptypin,
emptypin,emptypin,emptypin,emptypin,emptypin,emptypin,emptypin,emptypin,
emptypin,emptypin,emptypin,emptypin,emptypin,emptypin,emptypin,emptypin,
emptypin,emptypin,emptypin,emptypin,emptypin,emptypin,emptypin,emptypin,
emptypin,emptypin,emptypin,emptypin,emptypin,emptypin,emptypin,emptypin
)
"""
