
#ifndef __CTRLBOARD_IO_H_
#define __CTRLBOARD_IO_H_

#include "lowlevel_io.h"
#include "config.h"
#include <string>

class CtrlboardIo: public LowlevelIo
{
public:
    enum TMotorControl { TCurrentLoop=0, TAnalogLevel=1, TSoftware=2 };
    enum TSpeedCtrl { TThrottle=0, TSpeed=1 };
    typedef std::basic_string< unsigned char > TIo;

    CtrlboardIo();
    ~CtrlboardIo();

    bool version( std::string & ver );
    bool firmware( std::string & fir );

    // Group 1.
    bool setMotorControl( TMotorControl val );
    bool motorControl( TMotorControl & val );

    bool setThrottleRumpUp( int val );
    bool throttleRumpUp( int & val );

    bool setThrottleRumpDown( int val );
    bool setThrottleRumpDown( int & val );

    bool setThrottleRangeLow( int val );
    bool throttleRangeLow( int & val );

    bool setThrottleRangeHigh( int val );
    bool throttleRangeHigh( int & val );

    bool setThrottleLockout( bool val );
    bool throttleLockout( bool & val );

    bool setStallThreshold( int val );
    bool stallThreshold( int & val );

    bool setThrottleSpeedCtrl( TSpeedCtrl val );
    bool throttleSpeedCtrl( TSpeedCtrl & val );

    bool setCurrentLimit( int en );
    bool throttleSpeedCtrl( int & en );

    bool setUndervoltageCtrl( int en );
    bool undervoltageCtrl( int & en );

    bool setPassword( const std::string & val );
    bool password( std::string & val );

    // Group 2.
    bool setEnabled( bool en );
    bool enabled( bool & en );

    bool setThrottle( int val );
    bool throttle( int & val );

    bool setSpeed( int val );
    bool speed( int & val );

    bool setDirection( bool cw );
    bool direction( bool & cw );

    // Group 3.
    bool voltage( int & val );
    bool enKeySeat_1( int & val );
    bool enKeySeat_2( int & val );
    bool controllerT( int & val );
    bool motorT( int & val );
    bool extT_1( int & val );
    bool extT_2( int & val );
    bool errorCode( int & val );

    // Group 3-eeprom.
    bool hours( int & val );
    bool cycles( int & val );
    bool modelRev( int & val );
};



#endif


