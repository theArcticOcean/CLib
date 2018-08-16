/**********************************************************************************************
*
*   Filename: main.cpp
*
*   Author: theArcticOcean - wei_yang1994@163.com
*
**********************************************************************************************/

#include <iostream>
#include <vtkSmartPointer.h>
#include <vtkConeSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkRenderWindow.h>
#include <vtkProperty.h>
#include <vtkCamera.h>

using namespace std;

int main()
{
    vtkSmartPointer<vtkConeSource> coneSource =
      vtkSmartPointer<vtkConeSource>::New();
    coneSource->SetCenter( 0.0, 0.0, 0.0 );
    coneSource->SetHeight( 3 );
    coneSource->SetRadius( 1 );
    coneSource->Update();

    vtkSmartPointer<vtkPolyDataMapper> mapper =
      vtkSmartPointer<vtkPolyDataMapper>::New();
    mapper->SetInputConnection(coneSource->GetOutputPort());

    vtkSmartPointer<vtkActor> actor =
      vtkSmartPointer<vtkActor>::New();
    actor->SetMapper(mapper);

    vtkSmartPointer<vtkRenderer> renderer =
      vtkSmartPointer<vtkRenderer>::New();

    vtkCamera *camera = renderer->GetActiveCamera();
    camera->SetPosition( 5, 5, 5 );
    camera->SetFocalPoint( 0, 0, 0 );
    camera->SetViewUp( 0, -1, 0 );
    camera->Azimuth( 180 );
    camera->Elevation( 0 ); //仰角

    vtkSmartPointer<vtkRenderWindow> renderWindow =
      vtkSmartPointer<vtkRenderWindow>::New();

    renderWindow->AddRenderer(renderer);

    // An interactor
    vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor =
      vtkSmartPointer<vtkRenderWindowInteractor>::New();
    renderWindowInteractor->SetRenderWindow(renderWindow);

    // Add the actors to the scene
    renderer->AddActor( actor );
    renderer->SetBackground(1,1,1); // Background color white

    // Render an image (lights and cameras are created automatically)
    renderWindow->Render();

    // Begin mouse interaction
    renderWindowInteractor->Start();
    return 0;
}
