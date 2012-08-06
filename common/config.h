
#ifndef __CONFIG_H_
#define __CONFIG_H_

#define VERSION "0.0.1"
#define FIRMWARE "moto"

// Param IDs.
#define MOTOR_CONTROL       0
#define THROTTLE_RUMP_UP    1
#define THROTTLE_RUMP_DOWN  3
#define THROTTLE_RANGE_LOW  5
#define THROTTLE_RANGE_HIGH 6
#define THROTTLE_LOCKOUT    7
#define STALL_THRESHOLD     8
#define THROTTLE_SPEED_CTRL 9
#define CURRENT_LIMIT       10
#define UNDERVOLTAGE_CTRL   12
#define PASSWORD            14

#define ON_OFF              0
#define THROTTLE            1
#define SPEED               2
#define DIRECTION           4

#define VOLTAGE             16
#define ENABLE_KEY_SEAT_1   17
#define ENABLE_KEY_SEAT_2   18
#define CONTROLLER_T        19
#define MOTOR_T             20
#define EXT_T_1             21
#define EXT_T_2             22
#define ERROR               23

#define HOURS               24
#define CYCLES              25
#define MODEL_REV           26

#define FUNC_VERSION          1 // Firmware version.
#define FUNC_FIRMWARE         2 // Firmware description.
#define FUNC_SET_PARAM        3 // Invokes param set routine.
#define FUNC_PARAM            4 // Fills queue with data to be transferred and transferres that data.
#define FUNC_SET_EEPROM_PARAM 5
#define FUNC_EEPROM_PARAM     6

#endif


