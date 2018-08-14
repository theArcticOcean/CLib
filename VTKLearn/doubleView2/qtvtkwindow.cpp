#include "qtvtkwindow.h"
#include "ui_qtvtkwindow.h"

#include <vtkRegularPolygonSource.h>
#include <vtkTriangleFilter.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkProperty.h>

QtVTKWindow::QtVTKWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QtVTKWindow)
{
    ui->setupUi(this);

    // ===========> Right item. <============

    // Source
    vtkSmartPointer<vtkRegularPolygonSource> rightPolygonSource =
            vtkSmartPointer<vtkRegularPolygonSource>::New();
    rightPolygonSource->SetNumberOfSides( 7 );
    rightPolygonSource->SetGeneratePolygon( true );
    rightPolygonSource->Update();

    // Filter
    vtkSmartPointer<vtkTriangleFilter> rightTriangleFilter =
            vtkSmartPointer<vtkTriangleFilter>::New();
    rightTriangleFilter->SetInputConnection(rightPolygonSource->GetOutputPort()); // Add lines inside it.
    rightTriangleFilter->Update();

    // Mapper
    vtkSmartPointer<vtkPolyDataMapper> rightTriangleMapper =
            vtkSmartPointer<vtkPolyDataMapper>::New();
    rightTriangleMapper->SetInputConnection( rightTriangleFilter->GetOutputPort() );

    // Actor
    vtkSmartPointer<vtkActor> rightTriangleActor =
            vtkSmartPointer<vtkActor>::New();
    rightTriangleActor->SetMapper( rightTriangleMapper );
    rightTriangleActor->GetProperty()->SetRepresentationToWireframe();

    // ===========> Right item end. <============
}

QtVTKWindow::~QtVTKWindow()
{
    delete ui;
}
