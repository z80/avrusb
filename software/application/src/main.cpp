
#include <QtGui>
#include "moto.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QCoreApplication::addLibraryPath( "./" );

    Moto * m = new Moto();
    m->show();

    int res = app.exec();
    return res;
}

