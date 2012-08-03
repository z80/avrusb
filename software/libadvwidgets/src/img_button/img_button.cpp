
#include "img_button.h"

QCache< qint64, QPixmap > CImgButton::m_picsSz;
QHash< QString, QPixmap > CImgButton::m_picsOrig;

bool m_resourcesInitialized = false;

CImgButton::CImgButton( QWidget * p )
    : QAbstractButton( p )
{
    m_hovered = false;
    if ( !m_resourcesInitialized )
    {
        m_resourcesInitialized = true;
        Q_INIT_RESOURCE( img_button );
    }
}

CImgButton::~CImgButton()
{
    
}

void  CImgButton::paintEvent( QPaintEvent * e )
{
    QPainter p( this );
    if ( !underMouse() )
    {
        if ( isDown() || isChecked() )
        {
            const QPixmap & pm = image( m_cName, m_cKey );
            p.drawPixmap( QPoint( 0, 0 ), pm );
        }
        else
        {
            const QPixmap & pm = image( m_uName, m_uKey );
            p.drawPixmap( QPoint( 0, 0 ), pm );
        }
    }
    else
    {
        if ( m_hovered )
        {
            if ( isDown() || isChecked() )
            {
                const QPixmap & pm = image( m_chName, m_chKey );
                p.drawPixmap( QPoint( 0, 0 ), pm );
            }
            else
            {
                const QPixmap & pm = image( m_uhName, m_uhKey );
                p.drawPixmap( QPoint( 0, 0 ), pm );
            }
        }
        else
        {
            if ( isDown() || isChecked() )
            {
                const QPixmap & pm = image( m_cName, m_cKey );
                p.drawPixmap( QPoint( 0, 0 ), pm );
            }
            else
            {
                const QPixmap & pm = image( m_uName, m_uKey );
                p.drawPixmap( QPoint( 0, 0 ), pm );
            }
            // А поверх рисуем прямоугольник.
            QPen   pen   = QPen( QColor( 170, 170, 210, 150 ) );
            QBrush brush = QBrush( QColor( 170, 170, 210, 150 ) );
            p.setPen( pen );
            p.setBrush( brush );
            p.drawRect( QRect( 0, 0, size().width(), size().height() ) );
        }
    }
}

void CImgButton::enterEvent( QEvent * e )
{
    repaint();
}

void CImgButton::leaveEvent( QEvent * e )
{
    repaint();
}

QSize CImgButton::sizeHint() const
{
    qint64 key = m_uKey;
    QSize sz = m_picsOrig[ m_uName ].size();
    return sz;
    //return QAbstractButton::sizeHint();
}

bool  CImgButton::hitButton( const QPoint & pos ) const
{
    const QRect & r = QRect( 0, 0, size().width(), size().height() );
    return r.contains( pos );
}

void CImgButton::addImage( const QString & fileName )
{
    if ( !m_picsOrig.contains( fileName ) )
    {
        QPixmap p = QPixmap( fileName );
        m_picsOrig[ fileName ] = p;
    }
}

void CImgButton::setPictures( const QString & unchecked, const QString & checked )
{
    m_picsSz.clear();
    addImage( unchecked );
    m_uName = unchecked;
    addImage( checked );
    m_cName = checked;
}

void CImgButton::setPicturesHovered( const QString & unchecked, const QString & checked )
{
    m_picsSz.clear();
    addImage( unchecked );
    m_uhName = unchecked;
    addImage( checked );
    m_chName = checked;
    m_hovered = true;
}

const QPixmap & CImgButton::scaleImage( const QString & name ) const
{
    QPixmap * scaled = new QPixmap();
    *scaled = m_picsOrig[ name ].scaled( size().width(), 
                                         size().height(), 
                                         Qt::IgnoreAspectRatio, 
                                         Qt::SmoothTransformation );
    m_picsSz.insert( scaled->cacheKey(), scaled );
    return *m_picsSz[ scaled->cacheKey() ];
}

const QPixmap & CImgButton::image( const QString & name, qint64 & cacheKey ) const
{
    if ( m_picsSz.contains( cacheKey ) )
        return *m_picsSz[ cacheKey ];
    const QPixmap & pm = scaleImage( name );
    cacheKey = pm.cacheKey();
    return pm;
}



