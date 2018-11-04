#include "widget.h"
#include <QApplication>

#include <qsurfaceformat.h>
#include <QVTKOpenGLWidget.h>

int main(int argc, char *argv[])
{
    // needed to ensure appropriate OpenGL context is created for VTK rendering.
    QSurfaceFormat::setDefaultFormat(QVTKOpenGLWidget::defaultFormat());
    setbuf( stdout, NULL );

    QApplication a(argc, argv);
    Widget w;
    w.show();
    return a.exec();
}
