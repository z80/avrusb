
#include "moto.h"
#include "ctrlboard_io.h"

void Moto::asynchReadStatus()
{
    if ( !m_board->isOpen() )
    {
        bool res = m_board->open();
        if ( res )
        {
            asynchReadConfig();
            emit sigOpened();
        }
    }
    else
    {
        bool res = m_board->throttle( m_state.throttle );
        if ( !res )
            goto LBL_STATUS_CLOSED;

        res = m_board->speed( m_state.speed );
        if ( !res )
            goto LBL_STATUS_CLOSED;

        res = m_board->direction( m_state.direction );
        if ( !res )
            goto LBL_STATUS_CLOSED;

        res = m_board->voltage( m_state.voltage );
        if ( !res )
            goto LBL_STATUS_CLOSED;

        res = m_board->enKeySeat_1( m_state.keySeat_1 );
        if ( !res )
            goto LBL_STATUS_CLOSED;

        res = m_board->enKeySeat_2( m_state.keySeat_2 );
        if ( !res )
            goto LBL_STATUS_CLOSED;

        res = m_board->controllerT( m_state.controllerT );
        if ( !res )
            goto LBL_STATUS_CLOSED;

        res = m_board->motorT( m_state.motorT );
        if ( !res )
            goto LBL_STATUS_CLOSED;

        res = m_board->extT_1( m_state.extT_1 );
        if ( !res )
            goto LBL_STATUS_CLOSED;

        res = m_board->extT_2( m_state.extT_2 );
        if ( !res )
            goto LBL_STATUS_CLOSED;

        res = m_board->errorCode( m_state.errorCode );
        if ( !res )
            goto LBL_STATUS_CLOSED;

        res = m_board->hours( m_state.hours );
        if ( !res )
            goto LBL_STATUS_CLOSED;

        res = m_board->cycles( m_state.cycles );
        if ( !res )
            goto LBL_STATUS_CLOSED;

        res = m_board->modelRev( m_state.modelRev );
        if ( !res )
            goto LBL_STATUS_CLOSED;

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
        bool res = m_board->motorControl( m_state.control );
        if ( !res )
            goto LBL_READ_CONFIG_CLOSED;

        res = m_board->throttleRumpUp( m_state.throttleRumpUp );
        if ( !res )
            goto LBL_READ_CONFIG_CLOSED;

        res = m_board->throttleRumpDown( m_state.throttleRumpDown );
        if ( !res )
            goto LBL_READ_CONFIG_CLOSED;

        res = m_board->throttleRangeLow( m_state.throttleRangeLow );
        if ( !res )
            goto LBL_READ_CONFIG_CLOSED;

        res = m_board->throttleRangeHigh( m_state.throttleRangeHigh );
        if ( !res )
            goto LBL_READ_CONFIG_CLOSED;

        res = m_board->throttleLockout( m_state.throttleLockout );
        if ( !res )
            goto LBL_READ_CONFIG_CLOSED;

        res = m_board->stallThreshold( m_state.stallThreshold );
        if ( !res )
            goto LBL_READ_CONFIG_CLOSED;

        res = m_board->throttleSpeedCtrl( m_state.speedCtrl );
        if ( !res )
            goto LBL_READ_CONFIG_CLOSED;

        res = m_board->currentLimit( m_state.currentLimit );
        if ( !res )
            goto LBL_READ_CONFIG_CLOSED;

        res = m_board->undervoltageCtrl( m_state.undervoltageCtrl );
        if ( !res )
            goto LBL_READ_CONFIG_CLOSED;

        res = m_board->password( m_state.password );
        if ( !res )
            goto LBL_READ_CONFIG_CLOSED;

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
        bool res = m_board->setMotorControl( m_state.control );
        if ( !res )
            goto LBL_WRITE_CONFIG_CLOSED;

        res = m_board->setThrottleRumpUp( m_state.throttleRumpUp );
        if ( !res )
            goto LBL_WRITE_CONFIG_CLOSED;

        res = m_board->setThrottleRumpDown( m_state.throttleRumpDown );
        if ( !res )
            goto LBL_WRITE_CONFIG_CLOSED;

        res = m_board->setThrottleRangeLow( m_state.throttleRangeLow );
        if ( !res )
            goto LBL_WRITE_CONFIG_CLOSED;

        res = m_board->setThrottleRangeHigh( m_state.throttleRangeHigh );
        if ( !res )
            goto LBL_WRITE_CONFIG_CLOSED;

        res = m_board->setThrottleLockout( m_state.throttleRangeHigh );
        if ( !res )
            goto LBL_WRITE_CONFIG_CLOSED;

        res = m_board->setStallThreshold( m_state.stallThreshold );
        if ( !res )
            goto LBL_WRITE_CONFIG_CLOSED;

        res = m_board->setThrottleSpeedCtrl( m_state.speedCtrl );
        if ( !res )
            goto LBL_WRITE_CONFIG_CLOSED;

        res = m_board->setCurrentLimit( m_state.currentLimit );
        if ( !res )
            goto LBL_WRITE_CONFIG_CLOSED;

        res = m_board->setUndervoltageCtrl( m_state.undervoltageCtrl );
        if ( !res )
            goto LBL_WRITE_CONFIG_CLOSED;

        res = m_board->setPassword( m_state.password );
        if ( !res )
            goto LBL_WRITE_CONFIG_CLOSED;
    }
    return;

LBL_WRITE_CONFIG_CLOSED:
    emit sigClosed();
}

void Moto::asynchWriteThrottle()
{
    m_board->setThrottle( m_state.throttle );
}

void Moto::asynchWriteSpeed()
{
	m_board->setSpeed( m_state.speed );
}

void Moto::asynchWriteDirection()
{
	m_board->setDirection( m_state.direction );
}




