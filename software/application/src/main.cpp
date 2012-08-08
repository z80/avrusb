
#include <QtGui>
#include "intro.h"
#include "moto.h"
#include "ctrlboard_io.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QCoreApplication::addLibraryPath( "./" );
    QApplication::setStyle( new QPlastiqueStyle() );

    Moto * m = new Moto();
    Intro * i = new Intro( m, 3000 );

    //CtrlboardIo * io = new CtrlboardIo();
    //bool res = io->open();
    //if ( res )
    //{
    //    std::string v;
    //    res = io->version( v );
    //    io->setMotorControl( CtrlboardIo::TSoftware );
    //    CtrlboardIo::TMotorControl ctrl;
    //    io->motorControl( ctrl );
    //    io->setThrottleRumpUp( 1234 );
    //    int val;
    //    io->throttleRumpUp( val );
    //    io->close();
    //}
    //delete io;
    //return 0;

    int res = app.exec();
    return res;
}

