
#include <QtGui>

#include "moto.h"
#include "ctrlboard_io.h"

#include <boost/bind.hpp>
#include <boost/bind/arg.hpp>
#include <boost/bind/placeholders.hpp>

const QString Moto::INI_FILE_NAME = "settings.ini";

Moto::Moto( QWidget * parent )
: QMainWindow( parent )
{
    ui.setupUi( this );
    initGui();

    m_board = new CtrlboardIo();
}

Moto::~Moto()
{
    m_speedTimer->stop();
    m_speedTimer->deleteLater();
    m_statusTimer->stop();
    m_statusTimer->deleteLater();
    m_speedFuture.waitForFinished();
    m_statusFuture.waitForFinished();
    m_applyFuture.waitForFinished();
    delete m_board;
}

void Moto::loadSettings()
{
    QSettings set( INI_FILE_NAME,  QSettings::IniFormat, this );

    // RPM gauge settings.
    qreal vmin = set.value( "rpm_min", 0.0 ).toDouble();
    qreal vmax = set.value( "rpm_min", 1500.0 ).toDouble();
    qreal step = set.value( "rpm_step", 100.0 ).toDouble();
    ui.speed_msr->setRange( vmin, vmax, step );
    ui.speed_msr_dig->setRange( vmin, vmax );
    ui.speed->setRange( vmin * 100, vmax * 100 );

    // Direction flip.
    m_state.directionFlip         = set.value( "directionFlip", false ).toBool();

    // Parameters availability.
    m_state.throttleTypeEn        = set.value( "throttleTypeEn",        true ).toBool();
    m_state.throttleModeEn        = set.value( "throttleModeEn",        true ).toBool();
    m_state.maxThrottleCwEn       = set.value( "maxThrottleCwEn",       true ).toBool();
    m_state.maxThrottleCcwEn      = set.value( "maxThrottleCcwEn",      true ).toBool();
    m_state.maxSpeedCwEn          = set.value( "maxSpeedCwEn",          true ).toBool();
    m_state.maxSpeedCcwEn         = set.value( "maxSpeedCcwEn",         true ).toBool();
    m_state.throttleRampUpCwEn    = set.value( "throttleRampUpCwEn",    true ).toBool();
    m_state.throttleRampUpCcwEn   = set.value( "throttleRampUpCcwEn",   true ).toBool();
    m_state.throttleRampDownCwEn  = set.value( "throttleRampDownCwEn",  true ).toBool();
    m_state.throttleRampDownCcwEn = set.value( "throttleRampDownCcwEn", true ).toBool();
    m_state.commutationModeEn     = set.value( "commutationModeEn",     true ).toBool();
    m_state.throttleLockoutEn     = set.value( "throttleLockoutEn",     true ).toBool();
    m_state.stallThresholdEn      = set.value( "stallThresholdEn",      true ).toBool();
    m_state.currentLimitEn        = set.value( "currentLimitEn",        true ).toBool();
    m_state.undervoltageCtrlEn    = set.value( "undervoltageCtrlEn",    true ).toBool();
    m_state.motorOvertempEn       = set.value( "motorOvertempEn",       true ).toBool();
    m_state.controllerOvertempEn  = set.value( "controllerOvertempEn",  true ).toBool();
    m_state.passwordEn            = set.value( "passwordEn",            true ).toBool();

    // Error codes.
    m_errorCodes.clear();
    m_errorCodes.reserve( 16 );
    for ( int i=0; i<16; i++ )
    {
    	QString stri = QString( "errorCode_%1" ).arg( i );
    	QString defStri = QString( "ErrorBit#%1" ).arg( i );
    	stri = set.value( stri, defStri ).toString();
    	m_errorCodes << stri;
    }

    // Help command.
    m_helpCmd = set.value( "helpCmd", "file://./help.txt" ).toString();

    // To create INI file if it doesn't exist.
    saveSettings();
}

void Moto::saveSettings()
{
    QSettings set( INI_FILE_NAME,  QSettings::IniFormat, this );
    set.setValue( "rpm_min", ui.speed_msr->minValue() );
    set.setValue( "rpm_min", ui.speed_msr->maxValue() );
    set.setValue( "rpm_step", ui.speed_msr->step() );

    // Direction flip.
    set.setValue( "directionFlip", m_state.directionFlip );

    // Parameters availability.
    set.setValue( "throttleTypeEn",        m_state.throttleTypeEn );
    set.setValue( "throttleModeEn",        m_state.throttleModeEn );
    set.setValue( "maxThrottleCwEn",       m_state.maxThrottleCwEn );
    set.setValue( "maxThrottleCcwEn",      m_state.maxThrottleCcwEn );
    set.setValue( "maxSpeedCwEn",          m_state.maxSpeedCwEn );
    set.setValue( "maxSpeedCcwEn",         m_state.maxSpeedCcwEn );
    set.setValue( "throttleRampUpCwEn",    m_state.throttleRampUpCwEn );
    set.setValue( "throttleRampUpCcwEn",   m_state.throttleRampUpCcwEn );
    set.setValue( "throttleRampDownCwEn",  m_state.throttleRampDownCwEn );
    set.setValue( "throttleRampDownCcwEn", m_state.throttleRampDownCcwEn );
    set.setValue( "commutationModeEn",     m_state.commutationModeEn );
    set.setValue( "throttleLockoutEn",     m_state.throttleLockoutEn );
    set.setValue( "stallThresholdEn",      m_state.stallThresholdEn );
    set.setValue( "currentLimitEn",        m_state.currentLimitEn );
    set.setValue( "undervoltageCtrlEn",    m_state.undervoltageCtrlEn );
    set.setValue( "motorOvertempEn",       m_state.motorOvertempEn );
    set.setValue( "controllerOvertempEn",  m_state.controllerOvertempEn );
    set.setValue( "passwordEn",            m_state.passwordEn );

    // Error codes.
    for ( int i=0; i<m_errorCodes.size(); i++ )
    {
    	QString arg = QString( "errorCode_%1" ).arg( i );
    	QString val = m_errorCodes.at( i );
    	set.setValue( arg, val );
    }

    // Help command.
    set.setValue( "helpCmd", m_helpCmd );
}

void Moto::initGui()
{
	ui.statusbar->showMessage( "Press F1 for help" );

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
    connect( ui.help,      SIGNAL(triggered()),              this, SLOT(slotHelp()) );

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
    ui.throttleType->setEnabled( false );
    ui.throttleMode->setEnabled( false );

    ui.maxThrottleCw->setEnabled( false );
    ui.maxThrottleCcw->setEnabled( false );
    ui.maxSpeedCw->setEnabled( false );
    ui.maxSpeedCcw->setEnabled( false );
    ui.throttleRampUpCw->setEnabled( false );
    ui.throttleRampUpCcw->setEnabled( false );
    ui.throttleRampDownCw->setEnabled( false );
    ui.throttleRampDownCcw->setEnabled( false );

    ui.commutationMode->setEnabled( false );
    ui.throttleLockout->setEnabled( false );
    ui.stallThreshold->setEnabled( false );

    ui.currentLimit->setEnabled( false );
    ui.undervoltageCtrl->setEnabled( false );
    ui.motorOvertemp->setEnabled( false );
    ui.controllerOvertemp->setEnabled( false );
    ui.password->setEnabled( false );
    ui.apply->setEnabled( false );
}

void Moto::unlockConfig()
{
    ui.throttleType->setEnabled( m_state.throttleTypeEn );
    ui.throttleMode->setEnabled( m_state.throttleModeEn );

    ui.maxThrottleCw->setEnabled( m_state.maxThrottleCwEn );
    ui.maxThrottleCcw->setEnabled( m_state.maxThrottleCcwEn );
    ui.maxSpeedCw->setEnabled( m_state.maxSpeedCwEn );
    ui.maxSpeedCcw->setEnabled( m_state.maxSpeedCcwEn );
    ui.throttleRampUpCw->setEnabled( m_state.throttleRampUpCwEn );
    ui.throttleRampUpCcw->setEnabled( m_state.throttleRampUpCcwEn );
    ui.throttleRampDownCw->setEnabled( m_state.throttleRampDownCwEn );
    ui.throttleRampDownCcw->setEnabled( m_state.throttleRampDownCcw );

    ui.commutationMode->setEnabled( m_state.commutationModeEn );
    ui.throttleLockout->setEnabled( m_state.throttleLockoutEn );
    ui.stallThreshold->setEnabled( m_state.stallThresholdEn );
    ui.currentLimit->setEnabled( m_state.currentLimitEn );
    ui.undervoltageCtrl->setEnabled( m_state.undervoltageCtrlEn );
    ui.password->setEnabled( m_state.passwordEn );
    ui.apply->setEnabled( true );
}

void Moto::adjustControls()
{
    bool speed = ( m_state.throttleMode == CtrlboardIo::TSpeed );
    ui.speed->setEnabled( speed );
    ui.throttle->setEnabled( !speed );
}

void Moto::slotSpeedTimeout()
{
    if ( m_speedFuture.isRunning() )
        return;
    m_speedFuture = QtConcurrent::run( boost::bind( &Moto::asynchReadSpeed, this ) );
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
    ui.voltage->setValue( m_state.voltage );

    ui.enKeySeat_1->setText( m_state.keySeat_1 ? "On" : "Off" );
    ui.enKeySeat_2->setText( m_state.keySeat_2 ? "On" : "Off" );

    ui.controllerT->setValue( m_state.controllerT );
    ui.motorT->setValue( m_state.motorT );
    ui.externalT_1->setValue( m_state.extT_1 );
    ui.externalT_2->setValue( m_state.extT_2 );

    ui.errorMsg->clear();
    if ( m_state.errorCode == 0 )
    	ui.errorMsg->setPlainText( "No error" );
    else
    {
    	QString stri = "";
    	for ( int i=0; i<m_errorCodes.size(); i++ )
    	{
    		if ( m_state.errorCode & ( 1 << i ) )
    		{
    			QString arg = QString( "%1%2" )
    					  .arg( ( stri.size() > 0 ) ? ", " : "" )
    					  .arg( m_errorCodes.at( i ) );
    			stri.append( arg );
    		}
    	}
    	ui.errorMsg->setPlainText( stri );
    }

    ui.hours->setValue( m_state.hours );
    ui.cycles->setValue( m_state.cycles );
}

void Moto::slotConfig()
{
    // Filling GUI with data.
    ui.throttleType->setCurrentIndex( static_cast<int>( m_state.throttleType ) );
    ui.throttleMode->setCurrentIndex( static_cast<int>( m_state.throttleMode ) );
    ui.maxThrottleCw->setValue( m_state.maxThrottleCw );
    ui.maxThrottleCcw->setValue( m_state.maxThrottleCcw );
    ui.maxSpeedCw->setValue( m_state.maxSpeedCw );
    ui.maxSpeedCcw->setValue( m_state.maxSpeedCcw );
    ui.throttleRampUpCw->setValue( m_state.throttleRampUpCw );
    ui.throttleRampUpCcw->setValue( m_state.throttleRampUpCcw );
    ui.throttleRampDownCw->setValue( m_state.throttleRampDownCw );
    ui.throttleRampDownCcw->setValue( m_state.throttleRampDownCcw );
    ui.commutationMode->setCurrentIndex( m_state.commutationMode );
    ui.throttleLockout->setCurrentIndex( m_state.throttleLockout ? 1 : 0 );
    ui.stallThreshold->setValue( m_state.stallThreshold );
    ui.currentLimit->setValue( m_state.currentLimit );
    ui.undervoltageCtrl->setValue( m_state.undervoltageCtrl );
    ui.motorOvertemp->setValue( m_state.motorOvertemp );
    ui.controllerOvertemp->setValue( m_state.controllerOvertemp );
    ui.password->setText( QString::fromStdString( m_state.password ) );

    ui.software->setEnabled( ( m_state.throttleType == CtrlboardIo::TSoftware ) );

    adjustControls();
}

void Moto::slotOpened()
{
    // Unlock GUI.
    ui.centralwidget->setEnabled( true );
}

void Moto::slotClosed()
{
    // Lock GUI.
	ui.centralwidget->setEnabled( false );
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
    if ( m_state.directionFlip )
    	m_state.direction = !m_state.direction;
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

    m_state.throttleType        = static_cast<CtrlboardIo::TThrottleType>( ui.throttleType->currentIndex() );
    m_state.throttleMode        = static_cast<CtrlboardIo::TThrottleMode>( ui.throttleMode->currentIndex() );

    m_state.maxThrottleCw       = ui.maxThrottleCw->value();
    m_state.maxThrottleCcw      = ui.maxThrottleCcw->value();
    m_state.maxSpeedCw          = ui.maxSpeedCw->value();
    m_state.maxSpeedCcw         = ui.maxSpeedCcw->value();
    m_state.throttleRampUpCw    = ui.throttleRampUpCw->value();
    m_state.throttleRampUpCcw   = ui.throttleRampUpCcw->value();
    m_state.throttleRampDownCw  = ui.throttleRampDownCw->value();
    m_state.throttleRampDownCcw = ui.throttleRampDownCcw->value();

    m_state.commutationMode     = ui.commutationMode->currentIndex();
    m_state.throttleLockout     = ( ui.throttleLockout->currentIndex() > 0 );
    m_state.stallThreshold      = ui.stallThreshold->value();
    m_state.currentLimit        = ui.currentLimit->value();
    m_state.undervoltageCtrl    = ui.undervoltageCtrl->value();
    m_state.motorOvertemp       = ui.motorOvertemp->value();
    m_state.controllerOvertemp  = ui.controllerOvertemp->value();
    m_state.password            = QString( ui.password->text() ).toStdString();

    m_applyFuture = QtConcurrent::run( boost::bind( &Moto::asynchWriteConfig, this ) );

    lockConfig();
}

void Moto::slotHelp()
{
	qDebug() << m_helpCmd;
	QDesktopServices::openUrl( m_helpCmd );
}




