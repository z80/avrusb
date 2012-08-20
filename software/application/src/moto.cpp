
#include <QtGui>

#include "moto.h"
#include "ctrlboard_io.h"

#include <boost/bind.hpp>
#include <boost/bind/arg.hpp>
#include <boost/bind/placeholders.hpp>

const QString Moto::INI_FILE_NAME = "settings.ini";

Moto::Moto( QWidget * parent )
: QWidget( parent )
{
    ui.setupUi( this );
    initGui();

    m_board = new CtrlboardIo();
}

Moto::~Moto()
{
    m_speedTimer->stop();
    m_speedTimer->deleteLater();
    m_speedFuture.waitForFinished();
    m_statusFuture.waitForFinished();
    m_applyFuture.waitForFinished();
    delete m_board;
}

void Moto::loadSettings()
{
    QSettings set( INI_FILE_NAME,  QSettings::IniFormat, this );
    qreal vmin = set.value( "rpm_min", 0.0 ).toDouble();
    qreal vmax = set.value( "rpm_min", 1500.0 ).toDouble();
    qreal step = set.value( "rpm_step", 100.0 ).toDouble();
    ui.speed_msr->setRange( vmin, vmax, step );
    ui.speed_msr_dig->setRange( vmin, vmax );
    ui.speed->setRange( vmin * 100, vmax * 100 );
}

void Moto::saveSettings()
{
    QSettings set( INI_FILE_NAME,  QSettings::IniFormat, this );
    set.setValue( "rpm_min", ui.speed_msr->minValue() );
    set.setValue( "rpm_min", ui.speed_msr->maxValue() );
    set.setValue( "rpm_step", ui.speed_msr->step() );
}

void Moto::initGui()
{
    ui.speed_msr->setLabel( "x100" );
    ui.software->setEnabled( false );
    slotClosed();
    lockConfig();

    loadSettings();

    connect( this, SIGNAL(sigSpeed()),  this, SLOT(slotSpeed()) );
    connect( this, SIGNAL(sigStatus()), this, SLOT(slotStatus()) );
    connect( this, SIGNAL(sigConfig()), this, SLOT(slotConfig()) );
    connect( this, SIGNAL(sigOpened()), this, SLOT(slotOpened()) );
    connect( this, SIGNAL(sigClosed()), this, SLOT(slotClosed()) );

    // Binding GUI controls.
    connect( ui.throttle,  SIGNAL(valueChanged(int)),        this, SLOT(slotThrottleChanged(int)) );
    connect( ui.speed,     SIGNAL(valueChanged(int)),        this, SLOT(slotSpeedChanged(int)) );
    connect( ui.direction, SIGNAL(currentIndexChanged(int)), this, SLOT(slotDirectionChanged(int)) );
    connect( ui.unlock,    SIGNAL(clicked()),                this, SLOT(slotUnlock()) );
    connect( ui.apply,     SIGNAL(clicked()),                this, SLOT(slotApply()) );

    m_speedTimer  = new QTimer();
    m_statusTimer = new QTimer();
    connect( m_speedTimer,  SIGNAL(timeout()), this, SLOT(slotSpeedTimeout()) );
    connect( m_statusTimer, SIGNAL(timeout()), this, SLOT(slotStatusTimeout()) );
    m_speedTimer->setInterval( 100 );
    m_statusTimer->setInterval( 1000 );
    m_speedTimer->start();
    m_statusTimer->start();
}

void Moto::writeConfig()
{
    if ( m_applyFuture.isRunning() )
        qApp->processEvents();
    m_applyFuture = QtConcurrent::run( boost::bind( &Moto::asynchWriteConfig, this ) );
}

void Moto::lockConfig()
{
    ui.control->setEnabled( false );
    ui.throttleRampUpCw->setEnabled( false );
    ui.throttleRampUpCcw->setEnabled( false );
    ui.throttleRampDownCw->setEnabled( false );
    ui.throttleRampDownCcw->setEnabled( false );

    ui.throttleLockout->setEnabled( false );
    ui.stallThreshold->setEnabled( false );
    ui.speedCtrl->setEnabled( false );
    ui.currentLimit->setEnabled( false );
    ui.undervoltageCtrl->setEnabled( false );
    ui.password->setEnabled( false );
    ui.apply->setEnabled( false );
}

void Moto::unlockConfig()
{
    ui.control->setEnabled( true );
    ui.throttleRampUpCw->setEnabled( true );
    ui.throttleRampUpCcw->setEnabled( true );
    ui.throttleRampDownCw->setEnabled( true );
    ui.throttleRampDownCcw->setEnabled( true );

    ui.throttleLockout->setEnabled( true );
    ui.stallThreshold->setEnabled( true );
    ui.speedCtrl->setEnabled( true );
    ui.currentLimit->setEnabled( true );
    ui.undervoltageCtrl->setEnabled( true );
    ui.password->setEnabled( true );
    ui.apply->setEnabled( true );
}

void Moto::adjustControls()
{
    bool speed = ( m_state.speedCtrl == CtrlboardIo::TSpeed );
    ui.speed->setEnabled( speed );
    ui.throttle->setEnabled( !speed );
}

void Moto::slotSpeedTimeout()
{
    if ( m_speedFuture.isRunning() )
        return;
    m_speedFuture = QtConcurrent::run( boost::bind( &Moto::asynchReadStatus, this ) );
}

void Moto::slotStatusTimeout()
{
    if ( m_statusFuture.isRunning() )
        return;
    m_statusFuture = QtConcurrent::run( boost::bind( &Moto::asynchReadStatus, this ) );
}

void Moto::slotSpeed()
{
    // Filling GUI with data.
    ui.speed_msr->setValue( m_state.speed );
    ui.speed_msr_dig->setValue( m_state.speed );

    ui.throttleMsr->setValue( m_state.throttle );

    QString dir;
    if ( m_state.directionFlip )
        dir = QString( "%1" ).arg( m_state.direction ? "CW" : "CCW" );
    else
    	dir = QString( "%1" ).arg( m_state.direction ? "CCW" : "CW" );
    ui.directionMsr->setText( dir );
}

void Moto::slotStatus()
{
    // Filling GUI with data.
    ui.speed_msr->setValue( m_state.speed );
    ui.speed_msr_dig->setValue( m_state.speed );

    ui.throttleMsr->setValue( m_state.throttle );

    ui.directionMsr->setCurrentIndex( m_state.direction ? 1 : 0 );

    ui.voltage->setValue( m_state.voltage );

    ui.enKeySeat_1->setCurrentIndex( m_state.keySeat_1 ? 1 : 0 );
    ui.enKeySeat_2->setCurrentIndex( m_state.keySeat_2 ? 1 : 0 );

    ui.controllerT->setValue( m_state.controllerT );
    ui.motorT->setValue( m_state.motorT );
    ui.externalT_1->setValue( m_state.extT_1 );
    ui.externalT_2->setValue( m_state.extT_2 );

    ui.errorMsg->setText( QString( "%1" ).arg( m_state.errorCode, 8, 2, QLatin1Char( '0' ) ) );

    ui.hours->setValue( m_state.hours );
    ui.cycles->setValue( m_state.cycles );
}

void Moto::slotConfig()
{
    // Filling GUI with data.
    ui.control->setCurrentIndex( static_cast<int>( m_state.control ) );
    ui.speedCtrl->setCurrentIndex( static_cast<int>( m_state.speedCtrl ) );
    ui.throttleRumpUp->setValue( m_state.throttleRumpUp );
    ui.throttleRumpDown->setValue( m_state.throttleRumpDown );
    ui.throttleRangeFrom->setValue( m_state.throttleRangeLow );
    ui.throttleRangeTo->setValue( m_state.throttleRangeHigh );
    ui.throttleLockout->setCurrentIndex( m_state.throttleLockout ? 1 : 0 );
    ui.stallThreshold->setValue( m_state.stallThreshold );
    ui.currentLimit->setValue( m_state.currentLimit );
    ui.undervoltageCtrl->setValue( m_state.undervoltageCtrl );
    ui.password->setText( QString::fromStdString( m_state.password ) );

    ui.software->setEnabled( ( m_state.control == CtrlboardIo::TSoftware ) );

    adjustControls();
}

void Moto::slotOpened()
{
    // Unlock GUI.
    setEnabled( true );
}

void Moto::slotClosed()
{
    // Lock GUI.
    setEnabled( false );
}

void Moto::slotSpeedChanged( int value )
{
    while ( m_applyFuture.isRunning() )
        qApp->processEvents();
    m_state.speed = ui.speed->value();
    m_applyFuture = QtConcurrent::run( boost::bind( &Moto::asynchWriteSpeed, this ) );
}

void Moto::slotThrottleChanged( int value )
{
    while ( m_applyFuture.isRunning() )
        qApp->processEvents();
    m_state.throttle = ui.throttle->value();
    m_applyFuture = QtConcurrent::run( boost::bind( &Moto::asynchWriteThrottle, this ) );
}

void Moto::slotDirectionChanged( int value )
{
    while ( m_applyFuture.isRunning() )
        qApp->processEvents();
    m_state.direction = ( ui.direction->currentIndex() > 0 );
    m_applyFuture = QtConcurrent::run( boost::bind( &Moto::asynchWriteDirection, this ) );
}

void Moto::slotUnlock()
{
    unlockConfig();
}

void Moto::slotApply()
{
    while ( m_applyFuture.isRunning() )
        qApp->processEvents();

    m_state.control           = static_cast<CtrlboardIo::TMotorControl>( ui.control->currentIndex() );
    m_state.throttleRumpUp    = ui.throttleRumpUp->value();
    m_state.throttleRumpDown  = ui.throttleRumpDown->value();
    m_state.throttleRangeLow  = ui.throttleRangeFrom->value();
    m_state.throttleRangeHigh = ui.throttleRangeTo->value();
    m_state.throttleLockout   = ( ui.throttleLockout->currentIndex() > 0 );
    m_state.stallThreshold    = ui.stallThreshold->value();
    m_state.speedCtrl         = static_cast<CtrlboardIo::TSpeedCtrl>( ui.speedCtrl->currentIndex() );
    m_state.currentLimit      = ui.currentLimit->value();
    m_state.undervoltageCtrl  = ui.undervoltageCtrl->value();
    m_state.password          = QString( ui.password->text() ).toStdString();

    m_applyFuture = QtConcurrent::run( boost::bind( &Moto::asynchWriteConfig, this ) );

    lockConfig();
}




