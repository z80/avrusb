
#ifndef __INTRO_H_
#define __INTRO_H_

#include <QtGui>

class Intro: public QWidget
{
    Q_OBJECT
public:
    Intro( QWidget * master, int timeout = 3000 );
    virtual ~Intro();

protected:
    void paintEvent(QPaintEvent *event);
    void resizeEvent(QResizeEvent *event);

private:
    QPixmap m_pixmap;
    QBitmap m_mask;
    QWidget * m_master;
    static const QString g_filename;
private slots:
    void slotTimeout();
};

#endif



