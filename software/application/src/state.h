
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

    CtrlboardIo::TThrottleType throttleType;
    CtrlboardIo::TThrottleMode throttleMode;
    int  maxThrottleCw;
    int  maxThrottleCcw;
    int  maxSpeedCw;
    int  maxSpeedCcw;
    int  throttleRampUpCw;
    int  throttleRampUpCcw;
    int  throttleRampDownCw;
    int  throttleRampDownCcw;
    int  commutationMode;
    bool throttleLockout;
    int  stallThreshold;
    int  currentLimit;
    int  undervoltageCtrl;
    int  motorOvertemp;
    int  controllerOvertemp;
    std::string password;

    bool throttleTypeEn,
         throttleModeEn,
         maxThrottleCwEn,
         maxThrottleCcwEn,
         maxSpeedCwEn,
         maxSpeedCcwEn,
         throttleRampUpCwEn,
         throttleRampUpCcwEn,
         throttleRampDownCwEn,
         throttleRampDownCcwEn,
         commutationModeEn,
         throttleLockoutEn,
         stallThresholdEn,
         currentLimitEn,
         undervoltageCtrlEn,
         motorOvertempEn,
         controllerOvertempEn,
         passwordEn;

    int  throttle;
    int  speed;
    bool direction,
         directionFlip;

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


