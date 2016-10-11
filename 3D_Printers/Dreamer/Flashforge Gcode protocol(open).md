# FlashForge Gcode Protocol v1.04 (Partial) #

#Induction
This document describles the GCode protocol used in Flashfoge Dreamer 3D Printer.PC and mobie devices can control the printer by GCode. 

**Please ignore the "~" symbol at the head of the command line.**

#Supported G Codes
##G1 - Linear interpolation
Move to the specified position at the current or specified feedrate.

Parameters

    X: (code, optional) If present, new X axis position, in mm
    Y: (code, optional) If present, new Y axis position, in mm
    Z: (code, optional) If present, new Z axis position, in mm
    E: (code, optional) If present, new A/B (depending on internal state machine) axis position, in mm 
    F: (code, optional) Feedrate, in mm/min

Example

	~G1 X10 Y20 Z30 E1.0 F3000
	~G1 Y40
Reply

	 ok

##G4 - dwell
Tells the machine to pause for a certain amount of time.

Parameters

	P: dwell time， in ms
	S: dwell time， in s
	
Example

	~G4 P10000
	~G4 S10

Reply

	ok

##G28 - Home
Move to the home position.

Parameters

   	X: (flag, optional) If present, home the x axis. 
    Y: (flag, optional) If present, home the y axis. 
    Z: (flag, optional) If present, home the z axis. 
	Default for all axes.

Example

	~G28
	~G28 X Y

Reply

	ok

##G90 - Set to Absolute Positioning
All coordinates from now on are absolute relative to the origin of the machine.

Parameters

	None

Example

	~G90

Reply

	ok

##G91 - Set to Relative Positioning
All coordinates from now on are relative to the last position.

Parameters

	None

Example

	~G91

Reply

	ok

##G92 - Set Position
Sets the position of the bot.

Parameters

    X: (code, optional) If present, new X axis position, in mm
    Y: (code, optional) If present, new Y axis position, in mm
    Z: (code, optional) If present, new Z axis position, in mm
    E: (code, optional) If present, new A/B (depending on internal state machine) axis position, in mm 

Example

	~G92 E0
	~G92 X10 Y20 Z5

Reply

	 ok

#Supported M Code (Unbuffered Commands)
##M105 - Get Extruder and HBP Temperature
Query the current temperature of the nozzle and bottom plate.

Example

	Send：~M105
	Reply(Single)：	T0: 25/220 B:25/100
		 		     ok
    Reply(Dual)：	T0: 25/220  T1: 25/220  B:25/100
				     ok

##M114 - Get Current Position
Get Current Position.

Example

	Send： ~M114
	Reply： X:10 Y:10 Z:10 A:5 B:0
			ok

##M115 - Get Machine Information
Query the machine information，including type, SN, Size, tool count and so on.

Example

	Send： ~M115
	Reply： Machine Type: Flashforge Dreamer
			Machine Name: My Dreamer
		    Firmware: V1.40 20140520
		    SN: 2324-1341-3453
		    X: 230  Y: 150  Z: 140
		    Tool Count: 2
            ok

##M119 - Get Machine Status
Query the current status of the machine, including endstops and move mode.

Example

	Send： ~M119
	Reply： Endstop: X-max: 0 Y-max: 0 Z-min: 1
           MachineStatus: READY
		   MoveMode: READY
		   ok

##M112 - Emergency Stop
Emergency Stop， Command buffer will be empty.

Reply
	
	ok

#Supported M Code（Buffered Commands)
##M6 - Wait For Toolhead
Instruct the machine to wait for the toolhead to reach its target temperature.

Parameters	

	T: The extruder to wait for， T0(Right extruder) or T1(Left extruder)
	S: (code, option) If present, sets the time limit that we wait for, in s （Default value is 600s）
Example

	~M6 T0

Reply

	 ok

##M7 - Wait For Platform
Instruct the machine to wait for the platform to reach its target temperature

Parameters	

	S:  (code, option) If present, sets the time limit that we wait for, in s （Default value is 600s）
Example

	~M7

Reply

	 ok

##M17 - Enable Axes Stepper Motor
Instruct the machine to enable the stepper motors for the specifed axes.

Parameters

	X: (flag, optional) If present, enable the X axis stepper motor
    Y: (flag, optional) If present, enable the Y axis stepper motor
    Z: (flag, optional) If present, enable the Z axis stepper motor
    A: (flag, optional) If present, enable the A axis stepper motor
    B: (flag, optional) If present, enable the B axis stepper motor
	E: (flag, optional) If present, enable the A & B axis stepper motor
	Default for all axes.

Example 

	~M17

Reply

	 ok

##M18 - Disable Axes Stepper Motor
Instruct the machine to disable the stepper motors for the specifed axes.

Parameters

 	X: (flag, optional) If present, disable the X axis stepper motor
    Y: (flag, optional) If present, disable the Y axis stepper motor
    Z: (flag, optional) If present, disable the Z axis stepper motor
    A: (flag, optional) If present, disable the A axis stepper motor
    B: (flag, optional) If present, disable the B axis stepper motor
	E: (flag, optional) If present, disable the A & B axis stepper motor
	Default for all axes.

Example 

	~M17

Reply

	 ok

##M104 - Set toolhead temperature
Set the target temperature for the current toolhead

Parameters

    S: (code) Temperature to set the toolhead to, in degrees C
    T: (code) The toolhead to heat, T0 or T1.

Example
	
	 ~M104 S220 T0

Reply

	 ok

##M140 - Set build platform temperature
Sets the target temperature for the current build platform

Parameters

	S: (code) Temperature to set the platform to, in degrees C

Example
	
 	~M140 S100

Reply
	
	 ok

##M106 - Enable Cooling Fan
Enable Cooling Fan.

Parameters

	None

Exampel
	
	 ~M106

Reply
	
	ok

##M107 - Disable Cooling Fan
Disable cooling fan.

Parameters

	None

Exampel
	
	 ~M107

Reply
	
	ok

##M108 - Tool Change
Instructs the machine to change its toolhead.

Parameters

	 T: (code) The toolhead for the machine to switch to, T0 or T1

Example

	~M108 T0

Reply

	 ok

##M132 - Load current home position from EEPROM
Recalls current home position from the EEPROM and waits for the buffer to empty.

Parameters

    X:  (flag, optional) If present, loads the X offset from the EEPROM
    Y:  (flag, optional) If present, loads the Y offset from the EEPROM
    Z:  (flag, optional) If present, loads the Z offset from the EEPROM
    A:  (flag, optional) If present, loads the A offset from the EEPROM
    B:  (flag, optional) If present, loads the B offset from the EEPROM

Example

	 ~M132 X Y Z A B

Reply

	 ok

##M907 - Set digital potentiometer value
Set the digital potentiometer value for the given axes. This is used to configure the current applied to each stepper axis. The value is specified as a value from 0-127; the mapping from current to potentimeter value is machine specific.

Parameters

    X: (code, optional) If present, X axis potentimeter value
    Y: (code, optional) If present, Y axis potentimeter value
    Z: (code, optional) If present, Z axis potentimeter value
    A: (code, optional) If present, A axis potentimeter value
    B: (code, optional) If present, B axis potentimeter value

Example

	 ~M907 X100 Y100 Z40 A100 B80	

Reply

	 ok
