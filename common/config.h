
#ifndef __CONFIG_H_
#define __CONFIG_H_

// Param IDs.
#define MOTOR_CONTROL       1
#define THROTTLE_RUMP_UP    2
#define THROTTLE_RUMP_DOWN  3
#define THROTTLE_RANGE_LOW  4
#define THROTTLE_RANGE_HIGH 5
#define THROTTLE_LOCKOUT    6
#define STALL_THRESHOLD     7
#define THROTTLE_SPEED_CTRL 8
#define CURRENT_LIMIT       9
#define UNDERVOLTAGE_CTRL   10
#define PASSWORD            11

#define ON_OFF              12
#define THROTTLE            13
#define SPEED               14
#define DIRECTION           15

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

#define FUNC_VERSION        0 // Just firmware version.
#define FUNC_SET_PARAM      1 // Invokes param set routine.
#define FUNC_PARAM          2 // Fills queue with data to be transferred and transferres that data.
#define FUNC_QUEUE          3 // To set queue ptr and read values bigger then 8 bytes.


#endif


