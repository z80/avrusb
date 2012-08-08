
#include "intro.h"

const QString Intro::g_filename = ":/images/intro.png";

Intro::Intro( QWidget * master, int timeout )
    : QWidget( 0 ), 
      m_master( master )
{
    m_master->hide();
    
    m_pixmap = QPixmap( g_filename );
    m_mask   = m_pixmap.mask();
    resize( m_pixmap.size() );
    setMask( m_mask );
    QRect r = QApplication::desktop()->screenGeometry( QCursor::pos() );
    move( r.left() + (r.width() - m_pixmap.width()) / 2,
          r.top() + (r.height() - m_pixmap.height()) / 2 );
    show();
    
    QTimer::singleShot( timeout, this, SLOT(slotTimeout()) );
}

Intro::~Intro()
{
}

void Intro::paintEvent( QPaintEvent * event )
{
	QPainter painter( this );
    painter.drawPixmap( 0, 0,
	                    m_pixmap.scaled( size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation ) );
}

void Intro::resizeEvent(QResizeEvent *event)
{
    setMask( m_mask );
}

void Intro::slotTimeout()
{
    m_master->show();
    this->deleteLater();
}



