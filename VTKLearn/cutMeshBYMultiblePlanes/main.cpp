#include <iostream>
#include <vtkSmartPointer.h>
#include <vtkSphereSource.h>
#include <vtkActor.h>
#include <vtkConeSource.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkPolyDataMapper.h>
#include <vtkPlaneCollection.h>
#include <vtkClipClosedSurface.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkClipPolyData.h>
#include <vtkImplicitBoolean.h>
#include <vtkPlanes.h>
#include <vtkDataArray.h>
#include <vtkDoubleArray.h>
#include <vtkProperty.h>
#include <vtkPolyLine.h>

#include "multiClipPolyData.h"
#include "point.hpp"
#include "PolydataMethods.h"

using namespace std;

#define vtkSPtr vtkSmartPointer
#define vtkSPtrNew(Var, Type) vtkSPtr<Type> Var = vtkSPtr<Type>::New();

int main()
{
    vtkSPtrNew( cone, vtkConeSource );
    cone->Update();

    vtkSPtrNew( originData, vtkPolyData );
    originData->DeepCopy( cone->GetOutput() );

    CPolydataMethods::IncreaseDensity( originData, 0.1 );

    vtkSPtrNew( cutPlane0, vtkPlane );
    cutPlane0->SetOrigin( 0, 0, 0 );
    cutPlane0->SetNormal( -1, 0, 0 );
    vtkSPtrNew( cutPlane1, vtkPlane );
    cutPlane1->SetOrigin( 0, 0, 0 );
    cutPlane1->SetNormal( 0, -1, 0 );

//    vtkSPtrNew( cutPlanes, vtkPlanes );
//    vtkSPtrNew( cutPts, vtkPoints );
//    cutPts->InsertNextPoint( 0, 0, 0 );
//    cutPts->InsertNextPoint( 0, 0.5, 0 );
////    cutPts->InsertNextPoint( 1, 0, 0 );

//    vtkSPtrNew( normals, vtkDoubleArray );
//    normals->SetNumberOfComponents( 3 );
//    normals->InsertTuple3( 0, -1, 0, 0 );
//    normals->InsertTuple3( 1, 0, -1, 0 );
////    normals->InsertTuple3( 2, 0, 1, 0 );
//    cutPlanes->SetPoints( cutPts );
//    cutPlanes->SetNormals( normals );

//    vtkSPtrNew( lines, vtkPolyLine );
//    std::vector<Point> pts{ Point(0,1,0), Point(0,0,0), Point(1,0,0) };
//    for( auto pt: pts )
//    {
//        int polyLineId = lines->GetPoints()->InsertNextPoint( pt.point );
//        lines->GetPointIds()->InsertNextId(polyLineId);
//    }

//    vtkSPtrNew( planes, CUPolyPlanes );
//    Point vec( -1, -1, 0 );
//    planes->SetExtrusionDirection(vec.point);
//    planes->SetPolyLine( lines );
//    planes->SetClosed( false );
//    planes->ComputeConvexForAll();

//    vtkSPtrNew( clipper, vtkClipPolyData );
//    clipper->SetInputData( cone->GetOutput() );
//    clipper->SetClipFunction( cutPlanes );
//    clipper->Update();

    vtkSPtrNew( clipper, multiClipPolyData );
    clipper->SetInputData( originData );
    clipper->AddClipFunction( cutPlane0 );
    clipper->AddClipFunction( cutPlane1 );
    clipper->Update();

    vtkSPtrNew( mapper, vtkPolyDataMapper );
    mapper->SetInputConnection( clipper->GetOutputPort() );

    vtkSPtrNew( actor, vtkActor );
    actor->SetMapper( mapper );

    vtkSPtrNew( mapper0, vtkPolyDataMapper );
    mapper0->SetInputData( originData );

    vtkSPtrNew( actor0, vtkActor );
    actor0->SetMapper( mapper0 );
    actor0->GetProperty()->SetColor( 1, 0, 1 );

    vtkSPtrNew( renderer, vtkRenderer );
    //renderer->AddActor(actor);
    renderer->AddActor(actor0);
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
