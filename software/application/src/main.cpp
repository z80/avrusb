
#include <QtGui>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QCoreApplication::addLibraryPath( "./" );

    int res = app.exec();
    return res;
}

