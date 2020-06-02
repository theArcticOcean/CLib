#include <vtkSmartPointer.h>
#include <vtkProperty.h>
#include <vtkSelectPolyData.h>
#include <vtkSphereSource.h>
#include <vtkClipPolyData.h>
#include <vtkProperty.h>
#include <vtkPolyDataMapper.h>
#include <vtkLODActor.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkDataSet.h>
#include <vtkXMLPolyDataReader.h>
#include <vtkTextSource.h>
#include <QString>
#include <vtkCoordinate.h>
#include <vtkTransformPolyDataFilter.h>
#include <vtkPolyDataMapper2D.h>
#include <vtkActor2D.h>
#include <vtkTransform.h>
#include <vtkNamedColors.h>
#include <vtkCell.h>
#include <QVector>
#include <vtkIdList.h>
#include <vtkSphereSource.h>
#include <vtkFollower.h>
#include <vtkPolyDataConnectivityFilter.h>
#include <vtkVectorText.h>
#include <QMap>
#include <QSet>

#define vtkSPtr vtkSmartPointer
#define vtkSPtrNew(Var, Type) vtkSPtr<Type> Var = vtkSPtr<Type>::New();

int main(int, char *[])
{
    vtkSPtrNew( reader, vtkXMLPolyDataReader );
    reader->SetFileName( "../intersectCircle3.vtp" );
    reader->Update();

    auto *polyData = reader->GetOutput();

    vtkSPtrNew( connectivityF, vtkPolyDataConnectivityFilter );
    connectivityF->SetInputData( polyData );
    connectivityF->SetExtractionModeToLargestRegion();
    connectivityF->Update();

    polyData = connectivityF->GetOutput();

    vtkSPtrNew( mapper, vtkPolyDataMapper );
    mapper->SetInputData( polyData );

    vtkSPtrNew( actor, vtkActor );
    actor->SetMapper( mapper );

    vtkSPtrNew( renderer, vtkRenderer );
    renderer->AddActor(actor);
    renderer->SetBackground( 0, 0, 0 );

    vtkSPtrNew( renderWindow, vtkRenderWindow );
    renderWindow->AddRenderer( renderer );

    vtkSPtrNew( renderWindowInteractor, vtkRenderWindowInteractor );
    renderWindowInteractor->SetRenderWindow( renderWindow );

    auto selectionPoints = polyData->GetPoints();

    /*
    // ===================== create sorted points ============================
    polyData->BuildCells();
    polyData->BuildLinks();
    QVector<vtkIdType> sortedIds;

    vtkSmartPointer<vtkPoints> sortPts =
            vtkSmartPointer<vtkPoints>::New();
    vtkIdType lastCellId = 0;
    // the first line
    vtkCell *cell = polyData->GetCell( lastCellId );
    vtkIdList *ids = cell->GetPointIds();
    for( int i = 0; i < ids->GetNumberOfIds(); ++i )
    {
        vtkIdType id = ids->GetId( i );
        sortedIds.push_back( id );
    }

    QVector<bool> cellVisited;
    for( int i = 0; i < polyData->GetNumberOfCells(); ++i )
    {
        cellVisited.push_back( false );
    }

    //polyData->GetCellNeighbors( lastCellId, ids, neighborCellIds );
    //polyData->GetCellEdgeNeighbors( lastCellId, 0, 1, neighborCellIds );

    while ( sortedIds.size() < polyData->GetNumberOfPoints() )
    {
        vtkSPtrNew( neighborCellIds, vtkIdList );
        bool foundCell = false;
        int index = sortedIds.size() - 1;
        while( index >= 0 && !foundCell )
        {
            polyData->GetPointCells( sortedIds[index], neighborCellIds );
            for( int i = 0; i < neighborCellIds->GetNumberOfIds(); ++i )
            {
                vtkIdType cellId = neighborCellIds->GetId( i );
                if( cellVisited[ cellId ] )
                {
                    continue;
                }
                cout << "cellId: " << cellId << endl;
                vtkCell *cell = polyData->GetCell( cellId );
                vtkIdList *ptIds = cell->GetPointIds();
                for( int j = 0; j < ptIds->GetNumberOfIds(); ++j )
                {
                    auto ptId = ptIds->GetId( j );
                    if( !sortedIds.contains( ptId ) )
                    {
                        sortedIds.push_back( ptId );
                        cellVisited[lastCellId] = true;
                        lastCellId = cellId;
                        foundCell = true;
                        break;
                    }
                }
                if( foundCell )
                {
                    break;
                }
            }
            index--;
        }
    }
    // ===================== finished: sorted points ============================
    */

    // ===================== second method for sorting ==========================
    polyData->BuildCells();
    polyData->BuildLinks();
    QVector<vtkIdType> sortedIds;
    QMap<int, int> lineIds;

    for( int i = 0; i < polyData->GetNumberOfCells(); ++i )
    {
        vtkCell *cell = polyData->GetCell( i );
        vtkIdList *ids = cell->GetPointIds();
        auto id0 = ids->GetId( 0 );
        auto id1 = ids->GetId( 1 );
        lineIds[id0] = id1;
    }

    sortedIds.push_back( 0 );
    sortedIds.push_back( lineIds[0] );
    while( sortedIds.size() < selectionPoints->GetNumberOfPoints() )
    {
        int lastId = sortedIds[sortedIds.size() - 1];
        sortedIds.push_back( lineIds[lastId] );
    }

    QSet<int> uniIds;
    for( auto it: sortedIds )
    {
        uniIds.insert( it );
    }

    cout << "size: " << uniIds.size() << " vs " << selectionPoints->GetNumberOfPoints() << endl;
    // ===================== finished: second method ==========================

    for( int i = 0; i < sortedIds.size(); ++i )
    //for( int i = 0; i < selectionPoints->GetNumberOfPoints(); ++i )
    {
        // text 2D
        vtkSmartPointer<vtkTextSource> text2D =
                vtkSmartPointer<vtkTextSource>::New();
        text2D->SetText( QString::number( i ).toStdString().c_str() );

        vtkSmartPointer<vtkTransform> text2DTransform =
                vtkSmartPointer<vtkTransform>::New();
        double *center = selectionPoints->GetPoint( sortedIds[i] );
        //double *center = selectionPoints->GetPoint( i );
        text2DTransform->Translate( center[0], center[1], center[2] );
        text2DTransform->Scale( 0.003, 0.003, 0.003 );

        vtkSmartPointer<vtkTransformPolyDataFilter> text2DDataFilter =
                vtkSmartPointer<vtkTransformPolyDataFilter>::New();
        text2DDataFilter->SetTransform( text2DTransform );
        text2DDataFilter->SetInputConnection( text2D->GetOutputPort() );

        vtkSmartPointer<vtkCoordinate> coords =
                vtkSmartPointer<vtkCoordinate>::New();
        coords->SetCoordinateSystemToWorld();

        vtkSmartPointer<vtkPolyDataMapper2D> text2DMapper =
                vtkSmartPointer<vtkPolyDataMapper2D>::New();
        text2DMapper->SetInputConnection( text2DDataFilter->GetOutputPort() );
        text2DMapper->SetTransformCoordinate( coords );

        vtkSmartPointer<vtkActor2D> text2DActor =
                vtkSmartPointer<vtkActor2D>::New();
        text2DActor->SetMapper( text2DMapper );

        renderer->AddActor( text2DActor );

        /* vtkSmartPointer<vtkVectorText> text =
                vtkSmartPointer<vtkVectorText>::New();
        text->SetText( QString::number( i ).toStdString().c_str() );
        text->Update();
        vtkSmartPointer<vtkTransform> transform =
                vtkSmartPointer<vtkTransform>::New();
        center = selectionPoints->GetPoint( sortedIds[i] );
        transform->Translate( center[0], center[1], center[2] );
        //transform->Scale( 0.003, 0.003, 0.003 );
        transform->Update();
        vtkSmartPointer<vtkTransformPolyDataFilter> transFilter =
                vtkSmartPointer<vtkTransformPolyDataFilter>::New();
        transFilter->SetTransform( transform );
        transFilter->SetInputData( text->GetOutput() );
        transFilter->Update();
        vtkSmartPointer<vtkPolyDataMapper> mapper =
                vtkSmartPointer<vtkPolyDataMapper>::New();
        mapper->SetInputData( transFilter->GetOutput() );
        vtkSmartPointer<vtkFollower> actor = vtkSmartPointer<vtkFollower>::New();
        actor->SetCamera( renderer->GetActiveCamera() );
        actor->SetMapper( mapper );
        renderer->AddActor( actor ); */
    }

    /*cell = polyData->GetCell( 17 );
    ids = cell->GetPointIds();
    for( int i = 0; i < ids->GetNumberOfIds(); ++i )
    {
        vtkIdType id = ids->GetId( i );
        cout << id << "\t";
        double *pos = polyData->GetPoint( id );
        vtkSPtrNew( sphere, vtkSphereSource );
        sphere->SetCenter( pos );
        sphere->SetRadius( 0.03 );
        sphere->Update();
        vtkSPtrNew( mapper, vtkPolyDataMapper );
        mapper->SetInputData( sphere->GetOutput() );
        vtkSPtrNew( actor, vtkActor );
        actor->SetMapper( mapper );
        actor->GetProperty()->SetColor( 1, 0, 0 );
        renderer->AddActor( actor );
    }
    cout << endl;

    cell = polyData->GetCell( 19 );
    ids = cell->GetPointIds();
    for( int i = 0; i < ids->GetNumberOfIds(); ++i )
    {
        vtkIdType id = ids->GetId( i );
        cout << id << "\t";
        double *pos = polyData->GetPoint( id );
        vtkSPtrNew( sphere, vtkSphereSource );
        sphere->SetCenter( pos );
        sphere->SetRadius( 0.03 );
        sphere->Update();
        vtkSPtrNew( mapper, vtkPolyDataMapper );
        mapper->SetInputData( sphere->GetOutput() );
        vtkSPtrNew( actor, vtkActor );
        actor->SetMapper( mapper );
        actor->GetProperty()->SetColor( 0, 1, 0 );
        renderer->AddActor( actor );
    }
    cout << endl;

    for( int i = 0; i < polyData->GetNumberOfCells(); ++i )
    {
        vtkCell *cell = polyData->GetCell( i );
        vtkIdList *ids = cell->GetPointIds();
        cout << i << ": ";
        for( int j = 0; j < ids->GetNumberOfIds(); ++j )
        {
            auto id = ids->GetId( j );
            cout << id << "\t";
        }
        cout << endl;
    } */

    renderer->ResetCamera();
    renderWindow->Render();
    renderWindowInteractor->Start();
    return 0;
}
