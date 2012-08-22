
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

void Moto::closeEvent( QCloseEvent * e )
{
    deleteLater();
}

void Moto::loadSettings()
{
    QSettings set( INI_FILE_NAME,  QSettings::IniFormat, this );

    // RPM gauge settings.
    qreal vmin = set.value( "rpmMin", 0.0 ).toDouble();
    qreal vmax = set.value( "rpmMax", 1500.0 ).toDouble();
    qreal step = set.value( "rpmStep", 100.0 ).toDouble();
    ui.speed_msr->setRange( vmin, vmax, step );
    ui.speed_msr_dig->setRange( vmin, vmax );
    ui.speed->setRange( vmin, vmax );

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
    	QString defStri = QString( "ErrorBit_%1" ).arg( i );
    	stri = set.value( stri, defStri ).toString();
    	m_errorCodes << stri;
    }

    // Commutation mode
    m_commutationModeNames.clear();
    m_commutationModeVals.clear();
    int index = 0;
    QString arg = QString( "commutationModeName_%1" ).arg( index );
    QString name = set.value( arg, "" ).toString();
    arg = QString( "commutationModeVal_%1" ).arg( index );
    int    val  = set.value( arg, "" ).toInt();
    while ( ( name.length() > 0 ) && ( val > 0 ) )
    {
    	m_commutationModeNames << name;
    	m_commutationModeVals  << val;
    	index ++;
        arg  = QString( "commutationModeName_%1" ).arg( index );
        name = set.value( arg, "" ).toString();
        arg = QString( "commutationModeVal_%1" ).arg( index );
        val = set.value( arg, "" ).toInt();
    }
    if ( m_commutationModeNames.size() < 1 )
    {
    	m_commutationModeNames << "2Q" << "4Q";
    	m_commutationModeVals  << 0    << 1;
    }
    ui.commutationMode->clear();
    ui.commutationMode->addItems( m_commutationModeNames );

    // Current limit
    m_currentLimitNames.clear();
    m_currentLimitVals.clear();
    index = 0;
    arg = QString( "currentLimitName_%1" ).arg( index );
    name = set.value( arg, "" ).toString();
    arg = QString( "currentLimitVal_%1" ).arg( index );
    val  = set.value( arg, -1 ).toInt();
    while ( ( name.length() > 0 ) && ( val > 0 ) )
    {
    	m_currentLimitNames << name;
    	m_currentLimitVals  << val;
    	index ++;
        arg  = QString( "currentLimitName_%1" ).arg( index );
        name = set.value( arg, "" ).toString();
        arg = QString( "currentLimitVal_%1" ).arg( index );
        val = set.value( arg, -1 ).toInt();
    }
    if ( m_currentLimitNames.size() < 1 )
    {
    	m_currentLimitNames << "10" << "15";
    	m_currentLimitVals  << 0    << 1;
    }
    ui.currentLimit->clear();
    ui.currentLimit->addItems( m_currentLimitNames );

    // Help command.
    m_helpCmd = set.value( "helpCmd", "file://help.txt" ).toString();

    // Limits
    loadLimits( set );

    // To create INI file if it doesn't exist.
    saveSettings();
}

void Moto::saveSettings()
{
    QSettings set( INI_FILE_NAME,  QSettings::IniFormat, this );
    set.setValue( "rpmMin", ui.speed_msr->minValue() );
    set.setValue( "rpmMax", ui.speed_msr->maxValue() );
    set.setValue( "rpmStep", ui.speed_msr->step() );

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

    // Commutation mode
    for ( int i=0; i<m_commutationModeNames.size(); i++ )
    {
    	QString arg = QString( "commutationModeName_%1" ).arg( i );
    	set.setValue( arg, m_commutationModeNames.at( i ) );
    	arg = QString( "commutationModeVal_%1" ).arg( i );
    	set.setValue( arg, m_commutationModeVals.at( i ) );
    }

    // Current limit
    for ( int i=0; i<m_currentLimitNames.size(); i++ )
    {
    	QString arg = QString( "currentLimitName_%1" ).arg( i );
    	set.setValue( arg, m_currentLimitNames.at( i ) );
    	arg = QString( "currentLimitVal_%1" ).arg( i );
    	set.setValue( arg, m_currentLimitVals.at( i ) );
    }

    // Help command.
    set.setValue( "helpCmd", m_helpCmd );

    // Limits
    saveLimits( set );
}

void Moto::loadLimits( QSettings & set )
{
    QStringList     names;
    QList<QSpinBox *> boxes;
    QList<int>      vals;
    names << "maxThrottleCwMin"       << "maxThrottleCwMax"       << "maxThrottleCwVal"
          << "maxThrottleCcwMin"      << "maxThrottleCcwMax"      << "maxThrottleCcwVal"
          << "maxSpeedCwMin"          << "maxSpeedCwMax"          << "maxSpeedCwVal"
          << "maxSpeedCcwMin"         << "maxSpeedCcwMax"         << "maxSpeedCcwVal"
          << "throttleRampUpCwMin"    << "throttleRampUpCwMax"    << "throttleRampUpCwVal"
          << "throttleRampUpCcwMin"   << "throttleRampUpCcwMax"   << "throttleRampUpCcwVal"
          << "throttleRampDownCwMin"  << "throttleRampDownCwMax"  << "throttleRampDownCwVal"
          << "throttleRampDownCcwMin" << "throttleRampDownCcwMax" << "throttleRampDownCcwVal"
          << "stallThresholdMin"      << "stallThresholdMax"      << "stallThresholdVal"
          << "undervoltageCtrlMin"    << "undervoltageCtrlMax"    << "undervoltageCtrlVal"
          << "motorOvertempMin"       << "motorOvertempMax"       << "motorOvertempVal"
          << "controllerOvertempMin"  << "cotrollerOvertempMax"   << "controllerOvertempVal";
    boxes << ui.maxThrottleCw << ui.maxThrottleCcw << ui.maxSpeedCw << ui.maxSpeedCcw
    	  << ui.throttleRampUpCw << ui.throttleRampUpCcw << ui.throttleRampDownCw << ui.throttleRampDownCcw
    	  << ui.stallThreshold << ui.undervoltageCtrl << ui.motorOvertemp << ui.controllerOvertemp;
    vals << 1 << 100 << 20
    	 << 1 << 100 << 20
    	 << 0 << 1500 << 800
    	 << 0 << 1500 << 800
    	 << 10  << 15000 << 20
    	 << 10  << 15000 << 20
    	 << 10  << 15000 << 20
    	 << 10  << 15000 << 20
    	 << 50  << 15000 << 1000
    	 << 1   << 1000  << 10
    	 << 5   << 50    << 10
    	 << 5   << 50    << 10;
    int index = 0;
    int cnt = boxes.size();
    for ( int i=0; i<cnt; i++ )
    {
    	QString from = names.at( index );
    	QString to   = names.at( index+1 );
    	QString val  = names.at( index+2 );
    	int defFrom  = vals.at( index );
    	int defTo    = vals.at( index+1 );
    	int defVal   = vals.at( index+2 );
    	index += 3;
    	int f = set.value( from, defFrom ).toInt();
    	int t = set.value( to,   defTo ).toInt();
    	int v = set.value( val,  defVal ).toInt();
    	QSpinBox * s = boxes[i];
    	s->setRange( f, t );
    	s->setValue( v );
    }

    int v = set.value( "throttleModeVal", 0 ).toInt();
    ui.throttleMode->setCurrentIndex( v );
    v = set.value( "throttleTypeVal", 0 ).toInt();
    ui.throttleType->setCurrentIndex( v );
    v = set.value( "commutationModeVal", 0 ).toInt();
    ui.commutationMode->setCurrentIndex( v );
    v = set.value( "currentLimitVal", 0 ).toInt();
    ui.currentLimit->setCurrentIndex( v );
}

void Moto::saveLimits( QSettings & set )
{
    QStringList     names;
    QList<QSpinBox *> boxes;
    names << "maxThrottleCwMin"       << "maxThrottleCwMax"       << "maxThrottleCwVal"
          << "maxThrottleCcwMin"      << "maxThrottleCcwMax"      << "maxThrottleCcwVal"
          << "maxSpeedCwMin"          << "maxSpeedCwMax"          << "maxSpeedCwVal"
          << "maxSpeedCcwMin"         << "maxSpeedCcwMax"         << "maxSpeedCcwVal"
          << "throttleRampUpCwMin"    << "throttleRampUpCwMax"    << "throttleRampUpCwVal"
          << "throttleRampUpCcwMin"   << "throttleRampUpCcwMax"   << "throttleRampUpCcwVal"
          << "throttleRampDownCwMin"  << "throttleRampDownCwMax"  << "throttleRampDownCwVal"
          << "throttleRampDownCcwMin" << "throttleRampDownCcwMax" << "throttleRampDownCcwVal"
          << "stallThresholdMin"      << "stallThresholdMax"      << "stallThresholdVal"
          << "undervoltageCtrlMin"    << "undervoltageCtrlMax"    << "undervoltageCtrlVal"
          << "motorOvertempMin"       << "motorOvertempMax"       << "motorOvertempVal"
          << "controllerOvertempMin"  << "cotrollerOvertempMax"   << "controllerOvertempVal";
    boxes << ui.maxThrottleCw << ui.maxThrottleCcw << ui.maxSpeedCw << ui.maxSpeedCcw
    	  << ui.throttleRampUpCw << ui.throttleRampUpCcw << ui.throttleRampDownCw << ui.throttleRampDownCcw
    	  << ui.stallThreshold << ui.undervoltageCtrl << ui.motorOvertemp << ui.controllerOvertemp;
    int index = 0;
    int cnt = boxes.size();
    for ( int i=0; i<cnt; i++ )
    {
    	QSpinBox * s = boxes[i];
    	int f = s->minimum();
    	int t = s->maximum();
    	int v = s->value();
    	QString from = names.at( index );
    	QString to   = names.at( index+1 );
    	QString val  = names.at( index+2 );
    	index += 3;
    	set.setValue( from, f );
    	set.setValue( to,   t );
    	set.setValue( val,  v );
    }

    int v = ui.throttleMode->currentIndex();
    set.setValue( "throttleModeVal", v );
    v = ui.throttleType->currentIndex();
    set.setValue( "throttleTypeVal", v );
    v = ui.commutationMode->currentIndex();
    set.setValue( "commutationModeVal", v );
    v = set.value( "currentLimitVal", 0 ).toInt();
    ui.currentLimit->setCurrentIndex( v );
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
    ui.motorOvertemp->setEnabled( m_state.motorOvertempEn );
    ui.controllerOvertemp->setEnabled( m_state.controllerOvertempEn );
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
    QMutexLocker lock( &m_state.mutex );

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
    ui.externalInput_1->setValue( m_state.extInp_1 );
    ui.externalInput_2->setValue( m_state.extInp_2 );

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
    //ui.commutationMode->setCurrentIndex( m_state.commutationMode );
    ui.throttleLockout->setCurrentIndex( m_state.throttleLockout ? 1 : 0 );
    ui.stallThreshold->setValue( m_state.stallThreshold );
    //ui.currentLimit->setValue( m_state.currentLimit );
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

    QMutexLocker lock( &m_state.mutex );

    m_state.speed = ui.speed->value();
    m_applyFuture = QtConcurrent::run( boost::bind( &Moto::asynchWriteSpeed, this ) );
}

void Moto::slotThrottleChanged( int value )
{
    while ( m_applyFuture.isRunning() )
        qApp->processEvents();

    QMutexLocker lock( &m_state.mutex );

    m_state.throttle = ui.throttle->value();
    m_applyFuture = QtConcurrent::run( boost::bind( &Moto::asynchWriteThrottle, this ) );
}

void Moto::slotDirectionChanged( int value )
{
    while ( m_applyFuture.isRunning() )
        qApp->processEvents();

    QMutexLocker lock( &m_state.mutex );

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

    m_state.commutationMode     = m_commutationModeVals.at( ui.commutationMode->currentIndex() );
    m_state.throttleLockout     = ( ui.throttleLockout->currentIndex() > 0 );
    m_state.stallThreshold      = ui.stallThreshold->value();
    m_state.currentLimit        = m_currentLimitVals.at( ui.currentLimit->currentIndex() );
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




