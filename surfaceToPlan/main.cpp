#include <vtkSmartPointer.h>
#include <vtkSphereSource.h>
#include <vtkActor.h>
#include <vtkConeSource.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkPolyDataMapper.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkSTLReader.h>
#include <vtkTransform.h>
#include <vtkPlaneSource.h>
#include <string>
#include <iostream>
#include <vtkProperty.h>

#include "tool.h"
#include "UPolyDataSurfaceToPlane.h"

#define vtkSPtr vtkSmartPointer
#define vtkSPtrNew(Var, Type) vtkSPtr<Type> Var = vtkSPtr<Type>::New();

using namespace std;

void ShowBigPlane(vtkSPtr<vtkRenderer> renderer, vtkSPtr<vtkPlane> plane)
{
    vtkSmartPointer<vtkPlaneSource> planeSource =
        vtkSmartPointer<vtkPlaneSource>::New();
    planeSource->SetCenter( plane->GetOrigin() );
    planeSource->SetNormal( plane->GetNormal() );
    planeSource->Update();

    vtkSmartPointer<vtkPolyDataMapper> planeMapper =
        vtkSmartPointer<vtkPolyDataMapper>::New();
    planeMapper->SetInputData( planeSource->GetOutput() );
    planeMapper->Update();

    int scaleValue = 40;
    vtkSmartPointer<vtkActor> planeActor =
        vtkSmartPointer<vtkActor>::New();
    planeActor->SetMapper( planeMapper );
    planeActor->GetProperty()->SetColor( 0, 1, 1 );

    PointStruct originPt( plane->GetOrigin() );
    vtkSPtrNew( trans, vtkTransform );
    trans->Translate( originPt.point );
    trans->Scale( scaleValue, scaleValue, scaleValue );
    trans->Translate( (-originPt).point );
    planeActor->SetUserTransform( trans );

    renderer->AddActor( planeActor );
}

int main()
{
    std::string filePath = "origin_surface.stl";
    vtkSPtrNew( reader, vtkSTLReader );
    reader->SetFileName( filePath.c_str() );
    reader->Update();

    auto data = reader->GetOutput();

    PointStruct origin( -1.78958, -22.5648, 20.2123 );
    PointStruct normal( 0.00056659, -0.10854, -0.994092 );

    vtkSmartPointer<vtkPlane> cutPlane = vtkSmartPointer<vtkPlane>::New();
    cutPlane->SetOrigin( origin.point );
    cutPlane->SetNormal( normal.point );

    vtkSmartPointer<CUPolyDataSurfaceToPlane> surfaceToPlane = vtkSmartPointer<CUPolyDataSurfaceToPlane>::New();
    surfaceToPlane->SetInputData( data );
    surfaceToPlane->SetClipPlane( cutPlane );
    surfaceToPlane->Update();

    vtkSPtrNew( mapper, vtkPolyDataMapper );
    mapper->SetInputData( surfaceToPlane->GetOutput() );

    vtkSPtrNew( actor, vtkActor );
    actor->SetMapper( mapper );

    vtkSPtrNew( mapper0, vtkPolyDataMapper );
    mapper0->SetInputData( data );

    vtkSPtrNew( actor0, vtkActor );
    actor0->SetMapper( mapper0 );
    actor0->GetProperty()->SetColor( 1, 0, 0 );

    vtkSPtrNew( renderer, vtkRenderer );
    renderer->AddActor( actor );
    //renderer->AddActor( actor0 );
    renderer->SetBackground( 0, 0, 0 );

    ShowBigPlane( renderer, cutPlane );

    vtkSPtrNew( renderWindow, vtkRenderWindow );
    renderWindow->AddRenderer( renderer );

    vtkSPtrNew( renderWindowInteractor, vtkRenderWindowInteractor );
    renderWindowInteractor->SetRenderWindow( renderWindow );

    renderer->ResetCamera();
    renderWindow->Render();
    renderWindowInteractor->Start();
    return 0;
}
