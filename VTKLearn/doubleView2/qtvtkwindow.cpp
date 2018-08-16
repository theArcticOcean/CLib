#include "qtvtkwindow.h"
#include "ui_qtvtkwindow.h"

#include <vtkRegularPolygonSource.h>
#include <vtkTriangleFilter.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkProperty.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkConeSource.h>
#include <vtkCommand.h>

QtVTKWindow::QtVTKWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QtVTKWindow)
{
    ui->setupUi(this);
    winBackColor = vtkSmartPointer<vtkNamedColors>::New();
    rightInteractorStyle = vtkSmartPointer<DoubleViewInteractorStyle>::New();
    leftInteractorStyle = vtkSmartPointer<DoubleViewInteractorStyle>::New();

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

    // Render
    vtkSmartPointer<vtkRenderer> rightRender =
            vtkSmartPointer<vtkRenderer>::New();
    rightRender->AddActor( rightTriangleActor );
    rightRender->SetBackground( winBackColor->GetColor3d( "DeepSkyBlue" ).GetData() );

    // RenderWindow
    vtkSmartPointer<vtkRenderWindow> rightRenderWindow =
            vtkSmartPointer<vtkRenderWindow>::New();
    rightRenderWindow->AddRenderer( rightRender );

    ui->rightQvtkWidget->SetRenderWindow( rightRenderWindow );
    ui->rightQvtkWidget->GetInteractor()->SetInteractorStyle( rightInteractorStyle );
    // ===========> Right item end. <============

    // ===========> Left item. <============
    // Source
    vtkSmartPointer<vtkConeSource> leftConeSource =
            vtkSmartPointer<vtkConeSource>::New();
    leftConeSource->SetCenter( 0, 0, 0 );
    leftConeSource->Update();

    // Mapper
    vtkSmartPointer<vtkPolyDataMapper> leftTriangleMapper =
            vtkSmartPointer<vtkPolyDataMapper>::New();
    leftTriangleMapper->SetInputConnection( leftConeSource->GetOutputPort() );

    // Actor
    vtkSmartPointer<vtkActor> leftTriangleActor =
            vtkSmartPointer<vtkActor>::New();
    leftTriangleActor->SetMapper( leftTriangleMapper );
    leftTriangleActor->GetProperty()->SetRepresentationToSurface();

    // Renderer
    vtkSmartPointer<vtkRenderer> leftRenderer =
            vtkSmartPointer<vtkRenderer>::New();
    leftRenderer->SetBackground( winBackColor->GetColor3d( "LightCyan" ).GetData() );
    leftRenderer->AddActor( leftTriangleActor );

    // RenderWindow
    vtkSmartPointer<vtkRenderWindow> leftRenderWindow =
            vtkSmartPointer<vtkRenderWindow>::New();
    leftRenderWindow->AddRenderer( leftRenderer );
    ui->leftQvtkWidget->SetRenderWindow( leftRenderWindow );
    leftInteractorStyle->SetCurrentRenderer( leftRenderer );
    rightInteractorStyle->SetCurrentRenderer( rightRender );
    ui->leftQvtkWidget->GetInteractor()->SetInteractorStyle( leftInteractorStyle );

    leftInteractorStyle->fetchStyle( rightInteractorStyle.Get() );
    rightInteractorStyle->fetchStyle( leftInteractorStyle.Get() );
    ui->leftQvtkWidget->setEnabled( true );
    ui->rightQvtkWidget->setEnabled( true );

    qDebug( "rightStyle is %p, leftStyle is %p",
            rightInteractorStyle.Get(),
            leftInteractorStyle.Get() );
    // ===========> Left item end. <============
}

QtVTKWindow::~QtVTKWindow()
{
    delete ui;
}
