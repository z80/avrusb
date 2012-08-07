
#ifndef __STATE_H_
#define __STATE_H_

#include "ctrlboard_io.h"

class State
{
public:
    State();
    ~State();

    std::string version;
    std::string firmware;

    CtrlboardIo::TMotorControl control;
    int  throttleRumpUp;
    int  throttleRumpDown;
    int  throttleRangeLow;
    int  throttleRangeHigh;
    bool throttleLockout;
    int  stallThreshold;
    CtrlboardIo::TSpeedCtrl speedCtrl;
    int  currentLimit;
    int  undervoltageCtrl;
    std::string password;

    int  throttle;
    int  speed;
    bool direction;

    int  voltage;
    bool keySeat_1;
    bool keySeat_2;
    int  controllerT;
    int  motorT;
    int  extT_1;
    int  extT_2;
    int  errorCode;

    int  hours;
    int  cycles;
    std::string modelRev;
};

#endif


