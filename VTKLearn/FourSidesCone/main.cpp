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

#include "WInteractorStyle.h"
#include "FourSidesCone.h"

using namespace std;

int main()
{
    setbuf( stdout, nullptr );

    vtkSmartPointer<FourSidesCone> cone =
            vtkSmartPointer<FourSidesCone>::New();
    cone->Update();

    vtkSmartPointer<vtkPolyDataMapper> mapper =
            vtkSmartPointer<vtkPolyDataMapper>::New();
    mapper->SetInputData( cone->GetOutput() );

    vtkSmartPointer<vtkActor> actor =
            vtkSmartPointer<vtkActor>::New();
    actor->SetMapper( mapper );

    vtkSmartPointer<vtkRenderer> renderer =
            vtkSmartPointer<vtkRenderer>::New();
    renderer->AddActor( actor );
    cone->Setm_ConeActor( actor );
    renderer->AddActor( cone->FetchVertexActor(0) );
    renderer->AddActor( cone->FetchVertexActor(1) );
    renderer->AddActor( cone->FetchVertexActor(2) );
    renderer->AddActor( cone->FetchVertexActor(3) );
    renderer->AddActor( cone->FetchVertexActor(4) );

    renderer->SetBackground( 0, 0, 0 );

    vtkSmartPointer<vtkRenderWindow> renderWindow =
            vtkSmartPointer<vtkRenderWindow>::New();
    renderWindow->AddRenderer( renderer );

    vSPNew(interactorStyle, WInteractorStyle);
    interactorStyle->Setm_Cone( cone );
    interactorStyle->Setm_Render( renderer );
    interactorStyle->Setm_RenderWindow( renderWindow );

    vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor =
            vtkSmartPointer<vtkRenderWindowInteractor>::New();
    renderWindowInteractor->SetInteractorStyle( interactorStyle );
    renderWindowInteractor->SetRenderWindow( renderWindow );

    renderer->ResetCamera();
    renderWindow->Render();
    renderWindowInteractor->Start();
    return 0;
}
