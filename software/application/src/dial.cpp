
#include <qpainter.h>
#include <qwt_dial_needle.h>
#include "dial.h"

Dial::Dial( QWidget * parent )
: QwtDial( parent ),
  d_label( "[a.u.]" )
{
    setWrapping( false );
    setReadOnly( true );

    setOrigin( 135.0 );
    setScaleArc( 0.0, 270.0 );
    scaleDraw()->setSpacing( 8 );

    QwtDialSimpleNeedle * needle = new QwtDialSimpleNeedle(
            QwtDialSimpleNeedle::Arrow, true, Qt::red, 
            QColor( Qt::gray ).light( 130 ) );
    setNeedle( needle );

    setScaleComponents( 
        QwtAbstractScaleDraw::Ticks | QwtAbstractScaleDraw::Labels );
    setScaleTicks( 0, 4, 8 );
}

void Dial::setLabel( const QString & label )
{
    d_label = label;
    update();
}

QString Dial::label() const
{
    return d_label;
}

void Dial::drawScaleContents(QPainter *painter,
    const QPointF &center, double radius) const
{
    QRectF rect( 0.0, 0.0, 2.0 * radius, 2.0 * radius - 10.0 );
    rect.moveCenter(center);

    const QColor color = palette().color( QPalette::Text );
    painter->setPen(color);

    const int flags = Qt::AlignBottom | Qt::AlignHCenter;
    painter->drawText( rect, flags, d_label );
}





