#include <iostream>
#include <vtkSmartPointer.h>
#include <vtkSphereSource.h>
#include <vtkActor.h>
#include <vtkConeSource.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkPolyDataMapper.h>
#include <vtkRenderWindowInteractor.h>
#include <string>
#include <vtkSTLReader.h>

#include "UPolyDataSurfaceToPlane.h"

#define vtkSPtr vtkSmartPointer
#define vtkSPtrNew(Var, Type) vtkSPtr<Type> Var = vtkSPtr<Type>::New();

using namespace std;

int main()
{
    std::string filePath = "/home/stephen/Downloads/FinalPd.stl";
    vtkSPtrNew( reader, vtkSTLReader );
    reader->SetFileName( filePath.c_str() );
    reader->Update();

    auto data = reader->GetOutput();
    vtkSmartPointer<vtkPlane> cutPlane = vtkSmartPointer<vtkPlane>::New();
    cutPlane->SetOrigin( -1.24107, 9.79288, 20.441 );
    cutPlane->SetNormal( 0.0353126, 0.975398, -0.217605 );

    vtkSmartPointer<CUPolyDataSurfaceToPlane> surfaceToPlane = vtkSmartPointer<CUPolyDataSurfaceToPlane>::New();
    surfaceToPlane->SetInputData( data );
    surfaceToPlane->SetClipPlane( cutPlane );
    surfaceToPlane->Update();

    vtkSPtrNew( mapper, vtkPolyDataMapper );
    mapper->SetInputData( surfaceToPlane->GetOutput() ); //  data );

    vtkSPtrNew( actor, vtkActor );
    actor->SetMapper( mapper );

    vtkSPtrNew( renderer, vtkRenderer );
    renderer->AddActor(actor);
    renderer->SetBackground( 0, 0, 0 );

    vtkSPtrNew( renderWindow, vtkRenderWindow );
    renderWindow->AddRenderer( renderer );

    vtkSPtrNew( renderWindowInteractor, vtkRenderWindowInteractor );
    renderWindowInteractor->SetRenderWindow( renderWindow );

    renderer->ResetCamera();
    renderWindow->Render();
    renderWindowInteractor->Start();
    return 0;
}
