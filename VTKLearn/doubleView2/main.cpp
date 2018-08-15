#include <vtkSmartPointer.h>
#include <vtkProperty.h>
#include <vtkPolyData.h>
#include <vtkTriangleFilter.h>
#include <vtkRegularPolygonSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkCamera.h>
#include <vtkNamedColors.h>
#include <vtkConeSource.h>

#include <QApplication>

#include "qtvtkwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QtVTKWindow win;
    win.show();

    return app.exec();
}
