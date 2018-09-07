#include <iostream>
#include <vtkSmartPointer.h>
#include <vtkPolyData.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkCamera.h>
#include <vtkConeSource.h>
#include <vtkImageData.h>
#include <vtkProp3DButtonRepresentation.h>
#include <ostream>
#include <vtkLinearTransform.h>
#include <vtkMatrix4x4.h>
#include <vtkTransform.h>
#include <QDebug>

#include "PointStruct.hpp"
#include "adjustInteractorStyle.h"
#include "Plane.h"
#include "DragCone.h"

using namespace std;

int main()
{
    Plane plane;
    DragCone dragCone;

    dragCone.fetchPlane( &plane );

    // Renderer
    vtkSmartPointer<vtkRenderer> renderer =
            vtkSmartPointer<vtkRenderer>::New();
    renderer->SetBackground(.4, .5, .6); // rgb

    // Camera
    vtkCamera *camera = renderer->GetActiveCamera();
    camera->Dolly( 0.3 );

    renderer->AddActor( plane.planeActor );
    renderer->AddActor( dragCone.dragConeActor );
    renderer->ResetCamera();
    // When we change our view range, we need to update clipping range.
    renderer->ResetCameraClippingRange();

    vtkSmartPointer<vtkRenderWindow> renderWindow =
            vtkSmartPointer<vtkRenderWindow>::New();
    renderWindow->AddRenderer(renderer);
    plane.renderWindow = dragCone.renderWindow = renderWindow;

    vtkSmartPointer<vtkRenderWindowInteractor> interactor =
            vtkSmartPointer<vtkRenderWindowInteractor>::New();
    interactor->SetRenderWindow(renderWindow);
    renderWindow->Render();

    vtkSmartPointer<AdjustInteractorStyle> interactorStyle =
            vtkSmartPointer<AdjustInteractorStyle>::New();
    interactorStyle->fetchPlane( &plane );
    interactorStyle->fetchDragCone( &dragCone );
    interactor->SetInteractorStyle( interactorStyle );

    dragCone.setPosition();
    plane.showFourPinnacles();

    interactor->Start();

    return 0;
}
