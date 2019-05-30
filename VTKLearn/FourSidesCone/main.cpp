#include <iostream>
#include <vtkSmartPointer.h>
#include <vtkSphereSource.h>
#include <vtkActor.h>
#include <vtkConeSource.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkLight.h>
#include <vtkCamera.h>
#include <vtkPolyDataAlgorithm.h>

#include "FourSidesCone.h"

using namespace std;

int main()
{
    vtkSmartPointer<FourSidesCone> cone =
            vtkSmartPointer<FourSidesCone>::New();
    cone->Update();

    vtkSmartPointer<vtkPolyDataMapper> mapper =
            vtkSmartPointer<vtkPolyDataMapper>::New();
    mapper->SetInputConnection( cone->GetOutputPort() );

    vtkSmartPointer<vtkActor> actor =
            vtkSmartPointer<vtkActor>::New();
    actor->SetMapper( mapper );

    vtkSmartPointer<vtkRenderer> renderer =
            vtkSmartPointer<vtkRenderer>::New();
    renderer->AddActor( actor );
    renderer->AddActor( cone->FetchVertexActor(0) );
    renderer->AddActor( cone->FetchVertexActor(1) );
    renderer->AddActor( cone->FetchVertexActor(2) );
    renderer->AddActor( cone->FetchVertexActor(3) );
    renderer->AddActor( cone->FetchVertexActor(4) );

    renderer->SetBackground( 0, 0, 0 );

    vtkSmartPointer<vtkRenderWindow> renderWindow =
            vtkSmartPointer<vtkRenderWindow>::New();
    renderWindow->AddRenderer( renderer );

    vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor =
            vtkSmartPointer<vtkRenderWindowInteractor>::New();
    renderWindowInteractor->SetRenderWindow( renderWindow );

    renderer->ResetCamera();
    renderWindow->Render();
    renderWindowInteractor->Start();
    return 0;
}
