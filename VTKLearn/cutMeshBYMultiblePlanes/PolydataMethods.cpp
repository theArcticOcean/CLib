#include "PolydataMethods.h"
#include "point.hpp"

#include <vtkCellData.h>

#include <vtkPolyDataConnectivityFilter.h>
#include <vtkTriangleFilter.h>
#include <vtkLinearSubdivisionFilter.h>
#include <vtkCellLocator.h>
#include <vtkGenericCell.h>

void CPolydataMethods::IncreaseDensity(vtkSmartPointer<vtkPolyData> pld, const double maxEdgeLen)
{
    vtkSmartPointer<vtkPolyData> tmpData = vtkSmartPointer<vtkPolyData>::New();
    tmpData->DeepCopy( pld );

    // fix issue: Dataset is non-manifold and cannot be subdivided.
    vtkSmartPointer<vtkPolyDataConnectivityFilter> connectivityFilter = vtkSmartPointer<vtkPolyDataConnectivityFilter>::New();
    connectivityFilter->SetInputData( tmpData );
    connectivityFilter->SetExtractionModeToLargestRegion();
    connectivityFilter->Update();

    tmpData = connectivityFilter->GetOutput();
    // Finish: Dataset is non-manifold and cannot be subdivided.

    vtkSmartPointer<vtkTriangleFilter> triangleFilter = vtkSmartPointer<vtkTriangleFilter>::New();
    triangleFilter->SetInputData( tmpData );
    triangleFilter->PassLinesOff();
    triangleFilter->PassVertsOff();
    triangleFilter->Update();

    pld->DeepCopy( triangleFilter->GetOutput() );
    if( pld->NeedToBuildCells() )
    {
        pld->BuildCells();
    }
    pld->BuildLinks();

    auto JudgeNeedHandle = [](vtkPolyData *pld, const double maxEdgeLen) -> bool{
        bool toHandle = false;
        for( int i = 0; i < pld->GetNumberOfCells(); ++i )
        {
            vtkCell *cell = pld->GetCell( i );
            vtkPoints *points = cell->GetPoints();
            for( int j = 0; j < points->GetNumberOfPoints(); ++j )
            {
                Point pt0( points->GetPoint( j ) );
                Point pt1( points->GetPoint( (j+1)%points->GetNumberOfPoints() ) );
                if( (pt0-pt1).Length() > maxEdgeLen )
                {
                    toHandle = true;
                    //LOG( INFO, "toHandle is true" );
                    break;
                }
            }
        }
        return toHandle;
    };

    int times = 0;
    while( JudgeNeedHandle( pld, maxEdgeLen ) )
    {
        vtkSmartPointer<vtkPolyData> data = vtkSmartPointer<vtkPolyData>::New();
        data->DeepCopy( pld );

        vtkSmartPointer<vtkLinearSubdivisionFilter> subdivisionFilter = vtkSmartPointer<vtkLinearSubdivisionFilter>::New();
        subdivisionFilter->SetGlobalWarningDisplay( 1 );
        subdivisionFilter->SetInputData( data );
        subdivisionFilter->SetNumberOfSubdivisions( 1 );
        subdivisionFilter->Update();

        pld->DeepCopy( subdivisionFilter->GetOutput() );
        times++;
    }
}





