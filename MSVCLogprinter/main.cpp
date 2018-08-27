#include "logprinter.h"

#include <QApplication>

int main( int argc, char *argv[] )
{
    QApplication app( argc, argv );
    LogInit();
    print( "[%s, %d]\n hello\n", __FILE__, __LINE__ );
    return app.exec();
}
