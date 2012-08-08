
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
    m_future.waitForFinished();
    delete m_board;
}

void Moto::loadSettings()
{
    QSettings set( INI_FILE_NAME,  QSettings::IniFormat, this );
    qreal vmin = set.value( "rpm_min", 0.0 ).toDouble();
    qreal vmax = set.value( "rpm_min", 1500.0 ).toDouble();
    qreal step = set.value( "rpm_step", 100.0 ).toDouble();
    ui.speed_msr->setRange( vmin, vmax, step );
    ui.speed->setRange( vmin, vmax );
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
    slotClosed();

    loadSettings();

    m_timer = new QTimer();
    connect( m_timer, SIGNAL(timeout()), this, SLOT(slotTimeout()) );
    m_timer->setInterval( 1000 );
    m_timer->start();

    connect( this, SIGNAL(sigConfig()), this, SLOT(slotConfig()) );
    connect( this, SIGNAL(sigStatus()), this, SLOT(slotStatus()) );
    connect( this, SIGNAL(sigOpened()), this, SLOT(slotOpened()) );
    connect( this, SIGNAL(sigClosed()), this, SLOT(slotClosed()) );

    // Binding GUI controls.
    connect( ui.throttle,  SIGNAL(valueChanged(int)),        this, SLOT(slotThrottleChanged(int)) );
    connect( ui.speed,     SIGNAL(valueChanged(int)),        this, SLOT(slotSpeedChanged(int)) );
    connect( ui.direction, SIGNAL(currentIndexChanged(int)), this, SLOT(slotDirectionChanged(int)) );
}

void Moto::writeConfig()
{
    if ( m_future.isRunning() )
        qApp->processEvents();
    m_future = QtConcurrent::run( boost::bind( &Moto::asynchWriteConfig, this ) );
}

void Moto::slotTimeout()
{
    if ( m_future.isRunning() )
        return;
    m_future = QtConcurrent::run( boost::bind( &Moto::asynchReadStatus, this ) );
}

void Moto::slotConfig()
{
    // Filling GUI with data.
}

void Moto::slotStatus()
{
    // Filling GUI with data.
    ui.speed_msr->setValue( m_state.speed / 100 );
    ui.speed_msr_dig->setValue( m_state.speed / 100 );

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
    while ( m_future.isRunning() )
        qApp->processEvents();
    m_state.speed = ui.speed->value();
    m_future = QtConcurrent::run( boost::bind( &Moto::asynchWriteSpeed, this ) );
}

void Moto::slotThrottleChanged( int value )
{
    while ( m_future.isRunning() )
        qApp->processEvents();
    m_state.throttle = ui.throttle->value();
    m_future = QtConcurrent::run( boost::bind( &Moto::asynchWriteThrottle, this ) );
}

void Moto::slotDirectionChanged( int value )
{
    while ( m_future.isRunning() )
        qApp->processEvents();
    m_state.direction = ( ui.direction->currentIndex() > 0 );
    m_future = QtConcurrent::run( boost::bind( &Moto::asynchWriteDirection, this ) );
}



