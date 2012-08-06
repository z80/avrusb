
#include <QtGui>
#include "moto.h"
#include "ctrlboard_io.h"

int main(int argc, char *argv[])
{
    //QApplication app(argc, argv);
    //QCoreApplication::addLibraryPath( "./" );
    //QApplication::setStyle( new QPlastiqueStyle() );

    //Moto * m = new Moto();
    //m->show();

    CtrlboardIo * io = new CtrlboardIo();
    bool res = io->open();
    if ( res )
    {

        io->close();
    }
    delete io;
    return 0;

    //int res = app.exec();
    //return res;
}

