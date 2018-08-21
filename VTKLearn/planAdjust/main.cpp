#include <iostream>
#include <vtkSmartPointer.h>
#include <vtkProperty.h>
#include <vtkPolyData.h>
#include <vtkPlaneSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkCamera.h>
#include <vtkButtonWidget.h>
#include <vtkConeSource.h>
#include <vtkImageData.h>
#include <vtkProp3DButtonRepresentation.h>
#include <ostream>

#include "adjustInteractorStyle.h"

using namespace std;

int main()
{
    // Source
    vtkSmartPointer<vtkPlaneSource> planeSource =
            vtkSmartPointer<vtkPlaneSource>::New();
    planeSource->SetResolution( 10, 10 );
    planeSource->SetCenter( 0, 0, 0 );
    planeSource->SetOrigin( -5, -5, 0);
    planeSource->SetPoint1( 5, -5, 0 );
    planeSource->SetPoint2( -5, 5, 0 );
    planeSource->Update();

    ostream out( std::cout.rdbuf() );
    planeSource->PrintSelf( out, *vtkIndent::New() );

    // Mapper
    vtkSmartPointer<vtkPolyDataMapper> planeMapper =
            vtkSmartPointer<vtkPolyDataMapper>::New();
    planeMapper->SetInputConnection(planeSource->GetOutputPort());

    // Actor
    vtkSmartPointer<vtkActor> planeActor =
            vtkSmartPointer<vtkActor>::New();
    planeActor->SetMapper(planeMapper);
    planeActor->GetProperty()->SetColor( 1, 0, 0 );
    //planeActor->GetProperty()->SetRepresentationToWireframe();

    // Setup renderers
    vtkSmartPointer<vtkRenderer> renderer =
            vtkSmartPointer<vtkRenderer>::New();
    renderer->SetBackground(.4, .5, .6); // rgb

    renderer->AddActor(planeActor);
    renderer->ResetCamera();

    vtkCamera *camera = renderer->GetActiveCamera();
    camera->SetPosition( 5 , 5 , 2 );
    camera->SetFocalPoint( 0, 0, 0 );
    camera->Dolly( 0.3 );
    // When we change our view range, we need to update clipping range.
    renderer->ResetCameraClippingRange();

    // There will be one render window
    vtkSmartPointer<vtkRenderWindow> renderWindow =
            vtkSmartPointer<vtkRenderWindow>::New();
    renderWindow->AddRenderer(renderer);

    // And one interactor
    vtkSmartPointer<vtkRenderWindowInteractor> interactor =
            vtkSmartPointer<vtkRenderWindowInteractor>::New();
    interactor->SetRenderWindow(renderWindow);
    renderWindow->Render();

    // ===> top cone
    vtkSmartPointer<vtkConeSource> coneSource1 =
            vtkSmartPointer<vtkConeSource>::New();
    coneSource1->SetResolution( 10 );
    coneSource1->SetCenter( planeSource->GetXResolution() / 2, planeSource->GetYResolution() / 2, 1 );
    coneSource1->SetDirection( 0, 0, 1 );
    coneSource1->SetHeight( 2 );
    coneSource1->SetRadius( 1 );

    vtkSmartPointer<vtkPolyDataMapper> coneMapper1 =
            vtkSmartPointer<vtkPolyDataMapper>::New();
    coneMapper1->SetInputConnection( coneSource1->GetOutputPort() );

    vtkSmartPointer<vtkActor> coneActor1 =
            vtkSmartPointer<vtkActor>::New();
    coneActor1->SetMapper( coneMapper1 );
    coneActor1->GetProperty()->SetColor( 0, 1, 0 );

    vtkSmartPointer<vtkButtonWidget> buttonWidget1 =
        vtkSmartPointer<vtkButtonWidget>::New();

    vtkSmartPointer<vtkProp3DButtonRepresentation> buttonRepresentation1 =
        vtkSmartPointer<vtkProp3DButtonRepresentation>::New();
    buttonRepresentation1->SetVisibility( true );
    buttonRepresentation1->SetNumberOfStates( 1 );
    buttonRepresentation1->SetButtonProp(0, coneActor1);

    buttonWidget1->SetRepresentation( buttonRepresentation1 );
    buttonWidget1->SetInteractor( interactor );
    buttonWidget1->On();
    // ===> top cone end.

    // ===> bottom cone
    vtkSmartPointer<vtkConeSource> coneSource2 =
            vtkSmartPointer<vtkConeSource>::New();
    coneSource2->SetResolution( 10 );
    coneSource2->SetCenter( planeSource->GetXResolution() / 2, planeSource->GetYResolution() / 2, -1 );
    coneSource2->SetDirection( 0, 0, -1 );
    coneSource2->SetHeight( 2 );
    coneSource2->SetRadius( 1 );

    vtkSmartPointer<vtkPolyDataMapper> coneMapper2 =
            vtkSmartPointer<vtkPolyDataMapper>::New();
    coneMapper2->SetInputConnection( coneSource2->GetOutputPort() );

    vtkSmartPointer<vtkActor> coneActor2 =
            vtkSmartPointer<vtkActor>::New();
    coneActor2->SetMapper( coneMapper2 );
    coneActor2->GetProperty()->SetColor( 0, 1, 0 );

    vtkSmartPointer<vtkButtonWidget> buttonWidget2 =
        vtkSmartPointer<vtkButtonWidget>::New();

    vtkSmartPointer<vtkProp3DButtonRepresentation> buttonRepresentation2 =
        vtkSmartPointer<vtkProp3DButtonRepresentation>::New();
    buttonRepresentation2->SetVisibility( true );
    buttonRepresentation2->SetNumberOfStates( 1 );
    buttonRepresentation2->SetButtonProp(0, coneActor2);

    buttonWidget2->SetRepresentation( buttonRepresentation2 );
    buttonWidget2->SetInteractor( interactor );
    buttonWidget2->On();
    // ===> bottom cone end.

    vtkSmartPointer<AdjustInteractorStyle> interactorStyle =
            vtkSmartPointer<AdjustInteractorStyle>::New();
    interactor->SetInteractorStyle( interactorStyle );

    buttonWidget1->SetCurrentRenderer( renderer );
    buttonWidget2->SetCurrentRenderer( renderer );
    buttonWidget1->SetInteractor( interactor );
    buttonWidget1->GetInteractor()->SetInteractorStyle( interactorStyle );
    buttonWidget2->GetInteractor()->SetInteractorStyle( interactorStyle );

    std::cout << "--------------------------\n";
    buttonWidget1->PrintSelf( out, *vtkIndent::New() );
    interactor->Start();

    return 0;
}
