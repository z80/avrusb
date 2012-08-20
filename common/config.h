
#ifndef __CONFIG_H_
#define __CONFIG_H_

#define VERSION "0.1.0"
#define FIRMWARE "moto"

// Param IDs (in EEPROM).
#define THROTTLE_TYPE           0
#define THROTTLE_MODE           1
#define MAX_THROTTLE_CW	         2
#define MAX_THROTTLE_CCW        3
#define MAX_SPEED_CW            4
#define MAX_SPEED_CCW           6
#define THROTTLE_RAMP_UP_CW    	 8
#define THROTTLE_RAMP_UP_CCW    10
#define THROTTLE_RAMP_DOWN_CW   12
#define THROTTLE_RAMP_DOWN_CCW  14
#define COMMUTATION_MODE        16
#define THROTTLE_LOCKOUT        17
#define STALL_THRESHOLD         18
#define CURRENT_LIMIT           20
#define UNDERVOLTAGE_CTRL       22
#define MOTOR_OVERTEMP 	         24
#define CONTROLLER_OVERTEMP     26
#define PASSWORD                28

// Software ctrl params (in RAM).
#define ON_OFF              0
#define THROTTLE            1
#define SPEED               2
#define DIRECTION           4

// Ram runtime parameters, probably, for state visual control...
#define VOLTAGE             5
#define ENABLE_KEY_SEAT_1   6
#define ENABLE_KEY_SEAT_2   7
#define CONTROLLER_T        8
#define MOTOR_T             10
#define EXT_T_1             12
#define EXT_T_2             14
#define ERROR               16

// Three additional parameters from EEPROM...
#define HOURS               44
#define CYCLES              46
#define MODEL_REV           48

// Functions which could be invoked in MCU.
#define FUNC_VERSION          1 // Firmware version.
#define FUNC_FIRMWARE         2 // Firmware description.
#define FUNC_SET_PARAM        3 // Invokes param set routine.
#define FUNC_PARAM            4 // Fills queue with data to be transferred and transferres that data.
#define FUNC_SET_EEPROM_PARAM 5
#define FUNC_EEPROM_PARAM     6

#endif


