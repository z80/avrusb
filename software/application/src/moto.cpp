
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
    loadSettings();
}



