
#include "state.h"

State::State()
{
    std::string version;
    std::string firmware;

    control = CtrlboardIo::TSoftware;
    throttleRumpUp    = 0;
    throttleRumpDown  = 0;
    throttleRangeLow  = 0;
    throttleRangeHigh = 0;
    throttleLockout   = false;
    stallThreshold    = 0;
    speedCtrl         = CtrlboardIo::TSpeed;
    currentLimit      = 0;
    undervoltageCtrl  = 0;
    password          = "";

    throttle          = 0;
    speed             = 0;
    direction         = false;

    voltage           = 0;
    keySeat_1         = false;
    keySeat_2         = false;
    controllerT       = 0;
    motorT            = 0;
    extT_1            = 0;
    extT_2            = 0;
    errorCode         = 0;

    hours             = 0;
    cycles            = 0;
    modelRev          = "";
}

State::~State()
{
}


