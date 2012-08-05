
#include <QtGui>
#include "moto.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QCoreApplication::addLibraryPath( "./" );
    QApplication::setStyle( new QPlastiqueStyle() );

    Moto * m = new Moto();
    m->show();

    int res = app.exec();
    return res;
}

