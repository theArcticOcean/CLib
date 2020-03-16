#include <iostream>
#include <vtkSmartPointer.h>
#include <vtkSphereSource.h>
#include <vtkActor.h>
#include <vtkConeSource.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkPolyDataMapper.h>
#include <vtkSphereSource.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkProperty.h>
#include <vtkLineSource.h>
#include <vtkSTLReader.h>
#include <vtkPlaneSource.h>
#include <vtkCubeSource.h>
#include <vtkIdList.h>
#include <vtkCharArray.h>
#include <vtkCamera.h>
#include <vtkSTLWriter.h>
#include <vtkClipPolyData.h>
#include <vtkColorTransferFunction.h>
#include <vtkSTLReader.h>
#include <vtkPointData.h>
#include <vtkCutter.h>
#include <set>
#include <vtkPlane.h>

#include "ConnectedEdgeFilter.hpp"
#include "../share/tool.h"

using namespace std;

void FindIndependentEdges( vtkSPtr<vtkCellArray> edges, vtkSPtr<vtkPolyData> originPolyData )
{
    vtkIdType cellId;
    vtkIdType npts;
    vtkIdType p1, p2;
    vtkIdType *pts = nullptr;
    edges->Initialize();
    originPolyData->BuildLinks();
    vtkSPtrNew( neighbors, vtkIdList );
    neighbors->Allocate(VTK_CELL_SIZE);
    vtkCellArray *polys = originPolyData->GetPolys();
    for (cellId=0, polys->InitTraversal();
         polys->GetNextCell(npts,pts);
         cellId++)
    {
        for (int i = 0; i < npts; i++ )
        {
            p1 = pts[ i ];
            p2 = pts[ (i+1) %npts ];

            originPolyData->GetCellEdgeNeighbors( cellId, p1, p2, neighbors );
            vtkIdType numNei = neighbors->GetNumberOfIds();
            if (numNei < 1)
            {
                vtkIdType edgeIds[2] = { p1, p2 };
                edges->InsertNextCell( 2, edgeIds );
            }
            else
            {
                continue;
            }
        }
    }
}

int main()
{
    setbuf( stdout, nullptr );
    vtkSPtrNew( reader, vtkSTLReader );
    reader->SetFileName( "/Users/weiyang/Desktop/u.stl" );
    reader->Update();

    vtkPolyData *polyData = reader->GetOutput();

    // Clip the top to make arch unclosed.
    vtkSPtrNew( plane, vtkPlane );
    polyData->ComputeBounds();
    double bounds[6];
    polyData->GetBounds( bounds );
    PointStruct origin( polyData->GetCenter() );
    origin[1] += (bounds[3] - bounds[2] ) / 2 - 2;
    plane->SetOrigin( origin.point );
    plane->SetNormal( 0, 1, 0 );
    vtkSPtrNew( clipper, vtkClipPolyData );
    clipper->SetInputData( polyData );
    clipper->SetClipFunction( plane );
    clipper->InsideOutOn();
    clipper->Update();

    polyData = clipper->GetOutput();

    vtkSPtrNew( mapper, vtkPolyDataMapper );
    mapper->SetInputData( polyData );

    vtkSPtrNew( actor, vtkActor );
    actor->SetMapper( mapper );

    // find all lines which has not neighbors
    vtkSPtrNew( boundaryArray, vtkCellArray );
    FindIndependentEdges( boundaryArray, polyData );

    // Define viewport ranges
    // (xmin, ymin, xmax, ymax) left-top and right-bottom point
    double leftViewport[4] = {0.0, 0.0, 0.5, 1.0};
    double rightViewport[4] = {0.5, 0.0, 1.0, 1.0};

    // Setup renderers
    vtkSmartPointer<vtkRenderer> leftRenderer =
    vtkSmartPointer<vtkRenderer>::New();
    leftRenderer->SetViewport( leftViewport );
    leftRenderer->SetBackground( .6, .5, .4 );

    vtkSmartPointer<vtkRenderer> rightRenderer =
    vtkSmartPointer<vtkRenderer>::New();
    rightRenderer->SetViewport( rightViewport );
    rightRenderer->SetBackground( .4, .5, .6);

    leftRenderer->AddActor( actor );
    leftRenderer->SetBackground( 0, 0, 0 );

    ConnectedEdgeFilter *connectFilter = new ConnectedEdgeFilter();
    connectFilter->Initialise( boundaryArray );
    connectFilter->HandleEdges();

    vtkSPtr<vtkIdList> longestList = connectFilter->GetLongestList();
    std::cout << "longestList->GetNumberOfIds: " << longestList->GetNumberOfIds() << std::endl;

    vtkSPtrNew( edge, vtkPolyData );
    vtkSPtrNew( points, vtkPoints );
    vtkSPtrNew( lines, vtkCellArray );
    const int ptsCount = longestList->GetNumberOfIds();
    vtkIdType pts1[ptsCount];
    bool valid = true;
    for( int i = 0; i < longestList->GetNumberOfIds(); ++i )
    {
        vtkIdType id = longestList->GetId( i );
        if( id > polyData->GetNumberOfPoints() )
        {
            std::cout << "i: "<< i << ", bad id: " << id << std::endl;
            valid = false;
        }
        PointStruct pt( polyData->GetPoint( id ) );
        points->InsertNextPoint( pt.point );
        pts1[i] = i;
    }
    if( valid )
    {
        edge->SetPoints( points );
        lines->InsertNextCell( ptsCount, pts1 );
        edge->SetPolys( lines );
        edge->BuildCells();
        edge->BuildLinks();

        vtkSPtrNew( edgeMapper, vtkPolyDataMapper );
        edgeMapper->SetInputData( edge );

        vtkSPtrNew( edgeActor, vtkActor );
        edgeActor->SetMapper( edgeMapper );
        edgeActor->GetProperty()->SetColor( 1, 0, 0 );
        edgeActor->GetProperty()->SetLineWidth( 10 );

        rightRenderer->AddActor( edgeActor );
    }

    /*check all segments that contains longestList
    *
    for( int i = 0; i < boundaryArray->GetNumberOfCells(); ++i )
    {
        vtkSPtrNew( pts, vtkIdList );
        boundaryArray->GetCell( i, pts );
        std::cout << "GetNumberOfIds: " << pts->GetNumberOfIds() << std::endl;
        vtkSPtrNew( edge, vtkPolyData );
        vtkSPtrNew( points, vtkPoints );
        vtkSPtrNew( lines, vtkCellArray );
        const int ptsCount = pts->GetNumberOfIds();
        vtkIdType pts1[ptsCount];
        bool valid = true;
        for( int j = 0; j < pts->GetNumberOfIds(); ++j )
        {
            vtkIdType id = pts->GetId( j );
            if( id > polyData->GetNumberOfPoints() )
            {
                std::cout << "i: "<< j << ", bad id: " << id << std::endl;
                valid = false;
                break;
            }
            points->InsertNextPoint( polyData->GetPoint( id ) );
            pts1[j] = j;
        }
        if( !valid )
        {
            continue;
        }
        edge->SetPoints( points );
        lines->InsertNextCell( ptsCount, pts1 );
        edge->SetPolys( lines );

        vtkSPtrNew( edgeMapper, vtkPolyDataMapper );
        edgeMapper->SetInputData( edge );

        vtkSPtrNew( edgeActor, vtkActor );
        edgeActor->SetMapper( edgeMapper );
        edgeActor->GetProperty()->SetColor( 1, 0, 0 );

        rightRenderer->AddActor( edgeActor );
    } */

    std::cout << polyData->GetNumberOfPoints() << " : " << longestList->GetNumberOfIds() << std::endl;

    vtkSPtrNew( renderWindow, vtkRenderWindow );
    renderWindow->AddRenderer( leftRenderer );
    renderWindow->AddRenderer( rightRenderer );

    vtkSPtrNew( renderWindowInteractor, vtkRenderWindowInteractor );
    renderWindowInteractor->SetRenderWindow( renderWindow );

    vtkSPtrNew( camera, vtkCamera );
    leftRenderer->SetActiveCamera( camera );
    rightRenderer->SetActiveCamera( camera );
    leftRenderer->ResetCamera();
    rightRenderer->ResetCamera();

    renderWindow->SetSize(600, 300);
    renderWindow->Render();
    renderWindowInteractor->Start();

    delete connectFilter;
    return 0;
}
