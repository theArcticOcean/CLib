#include <iostream>
#include <QApplication>

#include "widget.h"
using namespace std;

int main(int argc, char *argv[])
{
    QApplication app( argc, argv );
    Widget w;
    w.show();
    return app.exec();
}
