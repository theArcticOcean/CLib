#include "logprinter.h"

#include <QApplication>

int main( int argc, char *argv[] )
{
    QApplication app( argc, argv );
    LogInit();
    printer( "hello world.\n" );
    return app.exec();
}
