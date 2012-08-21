
#include "state.h"

State::State()
{
    std::string version;
    std::string firmware;

    throttleType             = CtrlboardIo::TSoftware;
    throttleMode             = CtrlboardIo::TSpeed;
    int  maxThrottleCw       = 0;
    int  maxThrottleCcw      = 0;
    int  maxSpeedCw          = 0;
    int  maxSpeedCcw         = 0;
    int  throttleRumpUpCw    = 0;
    int  throttleRumpUpCcw   = 0;
    int  throttleRumpDownCw  = 0;
    int  throttleRumpDownCcw = 0;
    int  commutationMode     = 0;
    bool throttleLockout     = false;
    int  stallThreshold      = 0;;
    int  currentLimit        = 0;
    int  undervoltageCtrl    = 0;
    int  motorOvertemp       = 0;
    int  controllerOvertemp  = 0;
    password                 = "";

    throttleTypeEn        = true;
    throttleModeEn        = true;
    maxThrottleCwEn       = true;
    maxThrottleCcwEn      = true;
    maxSpeedCwEn          = true;
    maxSpeedCcwEn         = true;
    throttleRampUpCwEn    = true;
    throttleRampUpCcwEn   = true;
    throttleRampDownCwEn  = true;
    throttleRampDownCcwEn = true;
    commutationModeEn     = true;
    throttleLockoutEn     = true;
    stallThresholdEn      = true;
    currentLimitEn        = true;
    undervoltageCtrlEn    = true;
    motorOvertempEn       = true;
    controllerOvertempEn  = true;
    passwordEn            = true;

    throttle          = 0;
    speed             = 0;
    direction         = false;
    directionFlip     = false;

    voltage           = 0;
    keySeat_1         = false;
    keySeat_2         = false;
    controllerT       = 0;
    motorT            = 0;
    extInp_1          = 0;
    extInp_2          = 0;
    errorCode         = 0;

    hours             = 0;
    cycles            = 0;
    modelRev          = "";
}

State::~State()
{
}


