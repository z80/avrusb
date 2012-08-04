
#ifndef __MOTO_H_
#define __MOTO_H_

#include "ui_moto.h"

class Moto: public QWidget
{
    Q_OBJECT
public:
    Moto( QWidget * parent = 0 );
    virtual ~Moto();

private:
    Ui_Moto ui;
};



#endif



