#include <iostream>
#include <vtkSmartPointer.h>
#include <vtkSphereSource.h>
#include <vtkActor.h>
#include <vtkConeSource.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkPolyDataMapper.h>
#include <vtkLineSource.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkAxesActor.h>
#include <vtkProperty.h>
#include <vtkXMLPolyDataReader.h>
#include <vtkCharArray.h>
#include <vtkPointData.h>
#include <vtkConeSource.h>
#include <vtkTransform.h>
#include <vtkColorTransferFunction.h>
#include <vtkCellData.h>
#include <vtkPoints.h>
#include <vtkCell.h>
#include <vtkCellArray.h>
#include <vtkTriangle.h>
#include <vtkLineSource.h>
#include <vtkTriangleFilter.h>

#include "../tool.h"

using namespace std;

#define vtkSPtr vtkSmartPointer
#define vtkSPtrNew(Var, Type) vtkSPtr<Type> Var = vtkSPtr<Type>::New();

const int BackCell = 0;
const int ChosenCell = 1;

void FindIndependentEdges( vtkSPtr<vtkCellArray> edges, vtkSPtr<vtkIdList> redCells, vtkSPtr<vtkPolyData> originPolyData )
{
    vtkIdType cellId;
    vtkIdType npts;
    vtkIdType p1, p2;
    vtkIdType *pts = nullptr;
    edges->Initialize();
    if( originPolyData->NeedToBuildCells() )
    {
        originPolyData->BuildLinks();
        originPolyData->BuildCells();
    }

    vtkSPtrNew( neighbors, vtkIdList );
    neighbors->Allocate(VTK_CELL_SIZE);
    vtkDataArray *cellScalars = originPolyData->GetCellData()->GetScalars();
    for( int index = 0; index < redCells->GetNumberOfIds(); ++index )
    {
        cellId = redCells->GetId( index );
        originPolyData->GetCellPoints( cellId, npts, pts );

        for (int i = 0; i < npts; i++ )
        {
            p1 = pts[ i ];
            p2 = pts[ (i+1) %npts ];

            originPolyData->GetCellEdgeNeighbors( cellId, p1, p2, neighbors );
            bool noNeighbor = true;
            for( int j = 0; j < neighbors->GetNumberOfIds(); ++j )
            {
                vtkIdType neiCellId = neighbors->GetId( j );
                double value = cellScalars->GetTuple1( neiCellId );
                if( fabs(value - ChosenCell) < 1e-6 )
                {
                    noNeighbor = false;
                    break;
                }
            }
            if( noNeighbor )
            {
                vtkIdType edgeIds[2] = { p1, p2 };
                edges->InsertNextCell( 2, edgeIds );
            }
        }
    }
}

PointStruct CalculateNormal(vtkSPtr<vtkIdList> redCells, vtkSPtr<vtkPolyData> originPolyData)
{
    PointStruct totalNormal(0, 0, 0);
    for (vtkIdType i = 0; i < redCells->GetNumberOfIds(); i++)
    {
        vtkIdType cellId = redCells->GetId(i);
        vtkIdType nPoints;
        vtkIdType* pts;
        originPolyData->GetCellPoints(cellId, nPoints, pts);
        if (nPoints != 3)
            continue;

        PointStruct pointPos[3];
        for (vtkIdType j = 0; j < nPoints; j++)
        {
            originPolyData->GetPoint(pts[j], pointPos[j].point);
        }
        PointStruct vec1 = pointPos[1] - pointPos[0];
        PointStruct vec2 = pointPos[2] - pointPos[1];
        PointStruct normal = vec1 ^ vec2;
        double area = vtkTriangle::TriangleArea(pointPos[0].point, pointPos[1].point, pointPos[2].point);
        normal.Unit();
        totalNormal += normal*area;
    }
    if( totalNormal.Length() > 0 )
    {
        totalNormal.Unit();
    }
    return totalNormal;
}

int main()
{
    vtkSPtrNew( sphereSource, vtkSphereSource );
    sphereSource->SetThetaResolution( 50 );
    sphereSource->SetPhiResolution( 50 );
    sphereSource->Update();

    auto *pd = sphereSource->GetOutput();
    pd->BuildCells();
    pd->BuildLinks();

    // =============== find a red area ===============
    vtkSPtrNew( visitedCellIds, vtkIdList );
    visitedCellIds->InsertNextId( 0 );
    std::vector<vtkIdType> layerCellIds;
    layerCellIds.push_back( 0 );
    int iterCount = 8;
    while( iterCount-- )
    {
        std::vector<vtkIdType> currentCellIds;
        for( int i = 0; i < layerCellIds.size(); ++i )
        {
            vtkIdType cellId = layerCellIds[i];
            vtkIdType nPoints;
            vtkIdType *pts;
            pd->GetCellPoints(cellId, nPoints, pts);

            for( int j = 0; j < nPoints; ++j )
            {
                vtkSPtrNew( neiCellIds, vtkIdList );
                pd->GetCellEdgeNeighbors( cellId, pts[j], pts[(j+1)%nPoints], neiCellIds );

                for( int k = 0; k < neiCellIds->GetNumberOfIds(); ++k )
                {
                    cellId = neiCellIds->GetId( k );
                    if( -1 == visitedCellIds->IsId( cellId ) )
                    {
                        visitedCellIds->InsertUniqueId( cellId );
                        currentCellIds.push_back( cellId );
                    }
                }
            }
        }
        layerCellIds.swap( currentCellIds );
    }
    cout << "visitedCellIds count: " << visitedCellIds->GetNumberOfIds() << std::endl;

    // ================= use cell scalar to draw it ===================
    vtkSPtrNew( cellTypes, vtkCharArray );
    cellTypes->SetNumberOfComponents( 1 );
    for( int i = 0; i < pd->GetNumberOfCells(); ++i )
    {
        cellTypes->InsertNextValue( BackCell );
    }
    for( int i = 0; i < visitedCellIds->GetNumberOfIds(); ++i )
    {
        auto cellId = visitedCellIds->GetId( i );
        cellTypes->InsertValue( cellId, ChosenCell );
    }

    pd->GetCellData()->SetScalars( cellTypes );
    pd->GetCellData()->Modified();

    vtkSPtrNew( sphereMapper, vtkPolyDataMapper );
    sphereMapper->SetInputData( pd );
    sphereMapper->SetScalarModeToUseCellData();
    sphereMapper->SetColorModeToMapScalars();

    vtkSPtrNew( lut, vtkColorTransferFunction );
    lut->AddRGBPoint( BackCell, 1, 1, 1 );
    lut->AddRGBPoint( ChosenCell, 1, 0, 0 );
    sphereMapper->SetLookupTable( lut );
    // ================= finish: draw =====================

    vtkSPtrNew( actor, vtkActor );
    actor->SetMapper( sphereMapper );

    // ================= show boundary ===================
    vtkSPtrNew( edges, vtkCellArray );
    FindIndependentEdges( edges, visitedCellIds, pd );
    vtkSPtrNew( boundaryPd, vtkPolyData );
    boundaryPd->SetPoints( pd->GetPoints() );
    boundaryPd->SetLines( edges );
    //boundaryPd->SetPolys( edges );
    vtkSPtrNew( boundaryMapper, vtkPolyDataMapper );
    boundaryMapper->SetInputData( boundaryPd );
    vtkSPtrNew( boundrayActor, vtkActor );
    boundrayActor->SetMapper( boundaryMapper );
    boundrayActor->GetProperty()->SetColor( 0, 0, 1 );
    boundrayActor->GetProperty()->SetLineWidth( 5 );

    // ================= show normal ===================
    PointStruct normal = CalculateNormal( visitedCellIds, pd );
    vtkSPtrNew( boundaryPtIds, vtkIdList );
    boundaryPd->BuildCells();
    boundaryPd->BuildLinks();
    for( int i = 0; i < boundaryPd->GetNumberOfCells(); ++i )
    {
        vtkIdType nPts;
        vtkIdType *pts;
        boundaryPd->GetCellPoints( i, nPts, pts );
        for( int j = 0; j < nPts; ++j )
        {
            if( -1 == boundaryPtIds->IsId( pts[j] ) )
            {
                boundaryPtIds->InsertUniqueId( pts[j] );
            }
        }
    }
    PointStruct origin( 0, 0, 0 );

    for( int i = 0; i < boundaryPtIds->GetNumberOfIds(); ++i )
    {
        PointStruct pt( boundaryPd->GetPoint( boundaryPtIds->GetId( i ) ) );
        origin = origin + pt;
    }
    origin /= boundaryPtIds->GetNumberOfIds();

    vtkSPtrNew( line, vtkLineSource );
    line->SetPoint1( origin.point );
    line->SetPoint2( (origin + normal).point );
    line->Update();
    vtkSPtrNew( lineMapper, vtkPolyDataMapper );
    lineMapper->SetInputData( line->GetOutput() );
    vtkSPtrNew( lineActor, vtkActor );
    lineActor->SetMapper( lineMapper );
    lineActor->GetProperty()->SetColor( 0, 1, 0 );
    lineActor->GetProperty()->SetLineWidth( 5 );

    vtkSPtrNew( renderer, vtkRenderer );
    renderer->AddActor( actor );
    renderer->AddActor( boundrayActor );
    renderer->AddActor( lineActor );
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
