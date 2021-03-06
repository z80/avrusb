
#include "moto.h"
#include "ctrlboard_io.h"

const int Moto::SLEEP = 0;
const int Moto::TRIES = 8;

class Sleep: public QThread
{
public:
	static void msleep( int ms = 10 )
	{
		QThread::msleep( ms );
	}
};

void Moto::asynchReadSpeed()
{
    if ( m_board->isOpen() )
    {
    	m_state.mutex.lock();
    	int throttle   = m_state.throttle;
    	int speed      = m_state.speed;
    	bool direction = m_state.direction;
    	m_state.mutex.unlock();

        bool res = m_board->throttle( throttle );
        if ( !res )
            goto LBL_SPEED_CLOSED;
        Sleep::msleep( SLEEP );

        res = m_board->speed( speed );
        if ( !res )
            goto LBL_SPEED_CLOSED;
        Sleep::msleep( SLEEP );

        res = m_board->direction( direction );
        if ( !res )
            goto LBL_SPEED_CLOSED;
        Sleep::msleep( SLEEP );

        emit sigSpeed();
    }

    return;
LBL_SPEED_CLOSED:
    emit sigClosed();
}

void Moto::asynchReadStatus()
{
    if ( !m_board->isOpen() )
    {
        bool res = m_board->open();
        if ( res )
        {
        	// Dmitry said, do not read configuration.
        	// Just load it from INI file.
            //asynchReadConfig();
            emit sigOpened();
        }
    }
    else
    {
        bool res = m_board->voltage( m_state.voltage );
        if ( !res )
            goto LBL_STATUS_CLOSED;
        Sleep::msleep( SLEEP );

        res = m_board->enKeySeat_1( m_state.keySeat_1 );
        if ( !res )
            goto LBL_STATUS_CLOSED;
        Sleep::msleep( SLEEP );

        res = m_board->enKeySeat_2( m_state.keySeat_2 );
        if ( !res )
            goto LBL_STATUS_CLOSED;
        Sleep::msleep( SLEEP );

        res = m_board->controllerT( m_state.controllerT );
        if ( !res )
            goto LBL_STATUS_CLOSED;
        Sleep::msleep( SLEEP );

        res = m_board->motorT( m_state.motorT );
        if ( !res )
            goto LBL_STATUS_CLOSED;
        Sleep::msleep( SLEEP );

        res = m_board->extInp_1( m_state.extInp_1 );
        if ( !res )
            goto LBL_STATUS_CLOSED;
        Sleep::msleep( SLEEP );

        res = m_board->extInp_2( m_state.extInp_2 );
        if ( !res )
            goto LBL_STATUS_CLOSED;
        Sleep::msleep( SLEEP );

        res = m_board->errorCode( m_state.errorCode );
        if ( !res )
            goto LBL_STATUS_CLOSED;
        Sleep::msleep( SLEEP );

        res = m_board->hours( m_state.hours );
        if ( !res )
            goto LBL_STATUS_CLOSED;
        Sleep::msleep( SLEEP );

        res = m_board->cycles( m_state.cycles );
        if ( !res )
            goto LBL_STATUS_CLOSED;
        Sleep::msleep( SLEEP );

        res = m_board->modelRev( m_state.modelRev );
        if ( !res )
            goto LBL_STATUS_CLOSED;
        Sleep::msleep( SLEEP );

        emit sigStatus();
    }

    return;
LBL_STATUS_CLOSED:
    emit sigClosed();
}

void Moto::asynchReadConfig()
{
    if ( m_board->isOpen() )
    {
        bool res = m_board->throttleType( m_state.throttleType );
        if ( !res )
            goto LBL_READ_CONFIG_CLOSED;
        Sleep::msleep( SLEEP );

        res = m_board->throttleMode( m_state.throttleMode );
        if ( !res )
            goto LBL_READ_CONFIG_CLOSED;
        Sleep::msleep( SLEEP );

        res = m_board->maxThrottleCw( m_state.maxThrottleCw );
        if ( !res )
            goto LBL_READ_CONFIG_CLOSED;
        Sleep::msleep( SLEEP );

        res = m_board->maxThrottleCcw( m_state.maxThrottleCcw );
        if ( !res )
            goto LBL_READ_CONFIG_CLOSED;
        Sleep::msleep( SLEEP );

        res = m_board->maxSpeedCw( m_state.maxSpeedCw );
        if ( !res )
            goto LBL_READ_CONFIG_CLOSED;
        Sleep::msleep( SLEEP );

        res = m_board->maxSpeedCcw( m_state.maxSpeedCcw );
        if ( !res )
            goto LBL_READ_CONFIG_CLOSED;
        Sleep::msleep( SLEEP );

        res = m_board->throttleRampUpCw( m_state.throttleRampUpCw );
        if ( !res )
            goto LBL_READ_CONFIG_CLOSED;
        Sleep::msleep( SLEEP );

        res = m_board->throttleRampUpCcw( m_state.throttleRampUpCcw );
        if ( !res )
            goto LBL_READ_CONFIG_CLOSED;
        Sleep::msleep( SLEEP );

        res = m_board->throttleRampDownCw( m_state.throttleRampDownCw );
        if ( !res )
            goto LBL_READ_CONFIG_CLOSED;
        Sleep::msleep( SLEEP );

        res = m_board->throttleRampDownCcw( m_state.throttleRampDownCcw );
        if ( !res )
            goto LBL_READ_CONFIG_CLOSED;
        Sleep::msleep( SLEEP );

        res = m_board->commutationMode( m_state.commutationMode );
        if ( !res )
            goto LBL_READ_CONFIG_CLOSED;
        Sleep::msleep( SLEEP );

        res = m_board->throttleLockout( m_state.throttleLockout );
        if ( !res )
            goto LBL_READ_CONFIG_CLOSED;
        Sleep::msleep( SLEEP );

        res = m_board->stallThreshold( m_state.stallThreshold );
        if ( !res )
            goto LBL_READ_CONFIG_CLOSED;
        Sleep::msleep( SLEEP );

        res = m_board->currentLimit( m_state.currentLimit );
        if ( !res )
            goto LBL_READ_CONFIG_CLOSED;
        Sleep::msleep( SLEEP );

        res = m_board->undervoltageCtrl( m_state.undervoltageCtrl );
        if ( !res )
            goto LBL_READ_CONFIG_CLOSED;
        Sleep::msleep( SLEEP );

        res = m_board->motorOvertemp( m_state.motorOvertemp );
        if ( !res )
            goto LBL_READ_CONFIG_CLOSED;
        Sleep::msleep( SLEEP );

        res = m_board->controllerOvertemp( m_state.controllerOvertemp );
        if ( !res )
            goto LBL_READ_CONFIG_CLOSED;
        Sleep::msleep( SLEEP );

        res = m_board->password( m_state.password );
        if ( !res )
            goto LBL_READ_CONFIG_CLOSED;
        Sleep::msleep( SLEEP );

        emit sigConfig();
    }
    // If not opened don't emit sigClosed().
    return;

LBL_READ_CONFIG_CLOSED:
    emit sigClosed();
}

void Moto::asynchWriteConfig()
{
    if ( m_board->isOpen() )
    {
        bool res;

        if ( m_state.throttleTypeEn )
        {
        	res = m_board->setThrottleType( m_state.throttleType );
        	if ( !res )
        		goto LBL_WRITE_CONFIG_CLOSED;
        	Sleep::msleep( SLEEP );
        }

        if ( m_state.throttleModeEn )
        {
        	res = m_board->setThrottleMode( m_state.throttleMode );
        	if ( !res )
        		goto LBL_WRITE_CONFIG_CLOSED;
        	Sleep::msleep( SLEEP );
        }

        if ( m_state.maxThrottleCwEn )
        {
        	res = m_board->setMaxThrottleCw( m_state.maxThrottleCw );
        	if ( !res )
        		goto LBL_WRITE_CONFIG_CLOSED;
        	Sleep::msleep( SLEEP );
        }

        if ( m_state.maxThrottleCcwEn )
        {
        	res = m_board->setMaxThrottleCcw( m_state.maxThrottleCcw );
        	if ( !res )
        		goto LBL_WRITE_CONFIG_CLOSED;
        	Sleep::msleep( SLEEP );
        }

        if ( m_state.maxSpeedCcwEn )
        {
        	res = m_board->setMaxSpeedCw( m_state.maxSpeedCw );
        	if ( !res )
        		goto LBL_WRITE_CONFIG_CLOSED;
        	Sleep::msleep( SLEEP );
        }

        if ( m_state.maxSpeedCcwEn )
        {
        	res = m_board->setMaxSpeedCcw( m_state.maxSpeedCcw );
        	if ( !res )
        		goto LBL_WRITE_CONFIG_CLOSED;
        	Sleep::msleep( SLEEP );
        }

        if ( m_state.throttleRampUpCwEn )
        {
        	res = m_board->setThrottleRampUpCw( m_state.throttleRampUpCw );
        	if ( !res )
        		goto LBL_WRITE_CONFIG_CLOSED;
        	Sleep::msleep( SLEEP );
        }

        if ( m_state.throttleRampUpCcwEn )
        {
        	res = m_board->setThrottleRampUpCcw( m_state.throttleRampUpCcw );
        	if ( !res )
        		goto LBL_WRITE_CONFIG_CLOSED;
        	Sleep::msleep( SLEEP );
        }

        if ( m_state.throttleRampDownCwEn )
        {
        	res = m_board->setThrottleRampDownCw( m_state.throttleRampDownCw );
        	if ( !res )
        		goto LBL_WRITE_CONFIG_CLOSED;
        	Sleep::msleep( SLEEP );
        }

        if ( m_state.throttleRampDownCcwEn )
        {
        	res = m_board->setThrottleRampDownCcw( m_state.throttleRampDownCcw );
        	if ( !res )
        		goto LBL_WRITE_CONFIG_CLOSED;
        	Sleep::msleep( SLEEP );
        }

        if ( m_state.commutationModeEn )
        {
        	res = m_board->setCommutationMode( m_state.commutationMode );
        	if ( !res )
        		goto LBL_WRITE_CONFIG_CLOSED;
        	Sleep::msleep( SLEEP );
        }

        if ( m_state.throttleLockoutEn )
        {
        	res = m_board->setThrottleLockout( m_state.throttleLockout );
        	if ( !res )
        		goto LBL_WRITE_CONFIG_CLOSED;
        	Sleep::msleep( SLEEP );
        }

        if ( m_state.stallThresholdEn )
        {
        	res = m_board->setStallThreshold( m_state.stallThreshold );
        	if ( !res )
        		goto LBL_WRITE_CONFIG_CLOSED;
        	Sleep::msleep( SLEEP );
        }

        if ( m_state.currentLimitEn )
        {
        	res = m_board->setCurrentLimit( m_state.currentLimit );
        	if ( !res )
        		goto LBL_WRITE_CONFIG_CLOSED;
        	Sleep::msleep( SLEEP );
        }

        if ( m_state.undervoltageCtrlEn )
        {
        	res = m_board->setUndervoltageCtrl( m_state.undervoltageCtrl );
        	if ( !res )
        		goto LBL_WRITE_CONFIG_CLOSED;
        	Sleep::msleep( SLEEP );
        }

        if ( m_state.motorOvertempEn )
        {
        	res = m_board->setMotorOvertemp( m_state.motorOvertemp );
        	if ( !res )
        		goto LBL_WRITE_CONFIG_CLOSED;
        	Sleep::msleep( SLEEP );
        }

        if ( m_state.controllerOvertempEn )
        {
        	res = m_board->setControllerOvertemp( m_state.controllerOvertemp );
        	if ( !res )
        		goto LBL_WRITE_CONFIG_CLOSED;
        	Sleep::msleep( SLEEP );
        }

        if ( m_state.passwordEn )
        {
        	res = m_board->setPassword( m_state.password );
        	if ( !res )
        		goto LBL_WRITE_CONFIG_CLOSED;
        	Sleep::msleep( SLEEP );
        }

        // To update GUI call as config was just loaded.
        emit sigConfig();
    }
    return;

LBL_WRITE_CONFIG_CLOSED:
    emit sigClosed();
}

void Moto::asynchWriteThrottle()
{
	for ( int i=0; i<TRIES; i++ )
	{
		if ( m_board->setThrottle( m_state.throttle ) )
			break;
	}
}

void Moto::asynchWriteSpeed()
{
	for ( int i=0; i<TRIES; i++ )
	{
		if ( m_board->setSpeed( m_state.speed ) )
			break;
	}
}

void Moto::asynchWriteDirection()
{
	for ( int i=0; i<TRIES; i++ )
	{
		if ( m_board->setDirection( m_state.direction ) )
			break;
	}
}




