#include <vtkIntArray.h>
#include <vtkCellData.h>
#include <vtkTriangle.h>
#include <vtkCellArray.h>
#include <vtkPoints.h>
#include <vtkPolyData.h>
#include <vtkUnstructuredGrid.h>
#include <vtkPointData.h>
#include <vtkSmartPointer.h>
#include <vtkActor.h>
#include <vtkRenderer.h>
#include <vtkThreshold.h>
#include <vtkPolyDataMapper.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkDoubleArray.h>
#include <vtkDataSetSurfaceFilter.h>

#include "tool.h"

#define vSP vtkSmartPointer
#define vSPNew(Var, Type)    vSP<Type> Var = vSP<Type>::New();

vSPNew( polys, vtkCellArray );

void InsertCell( int size, const int *pts )
{
    if( 3 == size )
    {
        vtkIdType pt[3] = { pts[0], pts[1], pts[2] };
        polys->InsertNextCell( 3, pt );
    }
    else if(4 == size)
    {
        vtkIdType pt1[3] = { pts[0], pts[1], pts[2] };
        polys->InsertNextCell( 3, pt1 );

        vtkIdType pt2[3] = { pts[2], pts[3], pts[0] };
        polys->InsertNextCell( 3, pt2 );
    }
}

int main(int, char *[])
{
    // points
    vSPNew( points, vtkPoints );
    points->InsertNextPoint(0,0,0);
    points->InsertNextPoint(1,0,0);
    points->InsertNextPoint(0,0,-1);

    points->InsertNextPoint(0,1,0);
    points->InsertNextPoint(1,1,0);
    points->InsertNextPoint(0,1,-1);

    points->InsertNextPoint(0,2,0);
    points->InsertNextPoint(1,2,0);
    points->InsertNextPoint(0,2,-1);

    // polys.
    int pts[4];
    pts[0] = 0; pts[1] = 1; pts[2] = 2;
    InsertCell( 3, pts );

    pts[0] = 0; pts[1] = 1; pts[2] = 4; pts[3] = 3;
    InsertCell( 4, pts );

    pts[0] = 1; pts[1] = 2; pts[2] = 5; pts[3] = 4;
    InsertCell( 4, pts );

    pts[0] = 2; pts[1] = 0; pts[2] = 3; pts[3] = 5;
    InsertCell( 4, pts );

    pts[0] = 3; pts[1] = 4; pts[2] = 7; pts[3] = 6;
    InsertCell( 4, pts );

    pts[0] = 4; pts[1] = 5; pts[2] = 8; pts[3] = 7;
    InsertCell( 4, pts );

    pts[0] = 5; pts[1] = 3; pts[2] = 6; pts[3] = 8;
    InsertCell( 4, pts );

    pts[0] = 6; pts[1] = 7; pts[2] = 8;
    InsertCell( 3, pts );

    // polydata
    vSPNew( polydata, vtkPolyData );
    polydata->SetPoints( points );
    polydata->SetPolys( polys );

    vSPNew( YValues, vtkDoubleArray );
    YValues->SetNumberOfComponents( 1 );
    YValues->SetName( "ZValue" );
    for( int i = 0; i < points->GetNumberOfPoints(); ++i )
    {
        PointStruct pt( points->GetPoint( i ) );
        YValues->InsertNextValue( pt[1] );
    }
    polydata->GetPointData()->AddArray( YValues );

    vSPNew( threshold, vtkThreshold );
    threshold->SetInputData( polydata );
    threshold->ThresholdByUpper( 0.5 );
    threshold->SetInputArrayToProcess(0, 0, 0,
                                      vtkDataObject::FIELD_ASSOCIATION_POINTS, "ZValue");
    threshold->Update();
    vtkUnstructuredGrid* thresholdedPolydata = threshold->GetOutput();

    vSPNew( surfaceFilter, vtkDataSetSurfaceFilter );
    surfaceFilter->SetInputData( thresholdedPolydata );
    surfaceFilter->Update();

    vtkPolyData* filterPolydata = surfaceFilter->GetOutput();

    vSPNew( polydataMapper,vtkPolyDataMapper );
    polydataMapper->SetInputData( filterPolydata );
    polydataMapper->Update();

    vSPNew( actor, vtkActor );
    actor->SetMapper( polydataMapper );

    vSPNew( renderer, vtkRenderer );
    renderer->AddActor( actor );
    renderer->SetBackground( 0, 0, 0 );

    vSPNew( renderWindow, vtkRenderWindow );
    renderWindow->AddRenderer( renderer );

    vSPNew( renderWindowInteractor, vtkRenderWindowInteractor );
    renderWindowInteractor->SetRenderWindow( renderWindow );

    renderer->ResetCamera();
    renderWindow->Render();
    renderWindowInteractor->Start();
    return EXIT_SUCCESS;
}
