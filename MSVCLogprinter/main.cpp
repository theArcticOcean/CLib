#include "logprinter.h"

#include <QApplication>

int main( int argc, char *argv[] )
{
    QApplication app( argc, argv );
    LogInit();
    printer( "时间.\n" );
    QString str = "good night";
    printer("%s\n", str.toStdString().c_str() );
    return app.exec();
}
