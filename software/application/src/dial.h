
#ifndef __DIAL_H_
#define __DIAL_H_

#include <qstring.h>
#include <qwt_dial.h>

class Dial: public QwtDial
{
public:
    Dial( QWidget * parent = 0 );

    void setLabel( const QString & );
    QString label() const;

protected:
    virtual void drawScaleContents(QPainter *painter,
        const QPointF &center, double radius) const;

private:
    QString d_label;
};

#endif




