
#ifndef __IMG_BUTTON_H_
#define __IMG_BUTTON_H_

#include <QtGui>

class CImgButton: public QAbstractButton
{
    Q_OBJECT
public:
    CImgButton( QWidget * p = 0 );
    virtual ~CImgButton();
protected:
    void  paintEvent( QPaintEvent * e );
    void  enterEvent( QEvent * e );
    void  leaveEvent( QEvent * e );
    QSize sizeHint() const;
    bool  hitButton( const QPoint & pos ) const;
public:    
    // Добавление картинок в m_picsOrig.
    static void addImage( const QString & fileName );
    // Добавление для кнопки как минимум пары картинок.
    void setPictures( const QString & unchecked, const QString & checked );
    void setPicturesHovered( const QString & unchecked, const QString & checked );
private:
    const QPixmap & scaleImage( const QString & name ) const;
    const QPixmap & image( const QString & name, qint64 & cacheKey ) const;
    // Картинки с изменённым размером.
    static QCache< qint64, QPixmap > m_picsSz;
    // Картинки оригинального размера.
    static QHash< QString, QPixmap > m_picsOrig;
    bool m_hovered;
    QString m_cName, 
            m_uName, 
            m_chName, 
            m_uhName;
    qint64 m_cKey, 
           m_uKey, 
           m_chKey, 
           m_uhKey;
};

#endif

