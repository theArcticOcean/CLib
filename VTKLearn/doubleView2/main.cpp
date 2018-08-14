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

int main(int, char *[])
{
    vtkSmartPointer<vtkNamedColors> winBackColor =
            vtkSmartPointer<vtkNamedColors>::New();

    // Define viewport ranges
    // (xmin, ymin, xmax, ymax) means left-top and right-bottom points
    double leftViewport[4] = { 0.0, 0.0, 0.5, 1.0 };
    double rightViewport[4] = { 0.5, 0.0, 1.0, 1.0 };

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

    // Renderer
    vtkSmartPointer<vtkRenderer> rightRenderer =
            vtkSmartPointer<vtkRenderer>::New();
    rightRenderer->SetViewport( rightViewport );
    rightRenderer->SetBackground( winBackColor->GetColor3d( "DeepSkyBlue" ).GetData() ); // rgb
    rightRenderer->AddActor( rightTriangleActor );
    rightRenderer->ResetCamera();
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
    leftRenderer->SetViewport( leftViewport );
    leftRenderer->SetBackground( winBackColor->GetColor3d( "LightCyan" ).GetData() );
    leftRenderer->AddActor( leftTriangleActor );
    leftRenderer->ResetCamera();

    // ===========> Left item end. <============

    // A renderWindow
    vtkSmartPointer<vtkRenderWindow> renderWindow =
            vtkSmartPointer<vtkRenderWindow>::New();
    renderWindow->SetSize( 600, 300 );
    renderWindow->AddRenderer( rightRenderer );
    renderWindow->AddRenderer( leftRenderer );

    // An interactor
    vtkSmartPointer<vtkRenderWindowInteractor> interactor =
            vtkSmartPointer<vtkRenderWindowInteractor>::New();
    interactor->SetRenderWindow( renderWindow );
    renderWindow->Render();

    // A camera to see two views.
    vtkCamera *aCamera = vtkCamera::New();
    aCamera->SetViewUp (0, 0, 0);
    aCamera->SetPosition (0, 0, 3);
    aCamera->SetFocalPoint (0, 0, 0);
    leftRenderer->SetActiveCamera( aCamera );
    rightRenderer->SetActiveCamera( aCamera );

    interactor->Start();

    return EXIT_SUCCESS;
}
