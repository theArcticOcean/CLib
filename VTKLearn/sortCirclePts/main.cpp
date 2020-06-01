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

#define vtkSPtr vtkSmartPointer
#define vtkSPtrNew(Var, Type) vtkSPtr<Type> Var = vtkSPtr<Type>::New();

int main(int, char *[])
{
    vtkSPtrNew( reader, vtkXMLPolyDataReader );
    reader->SetFileName( "../intersectCircle.vtp" );
    reader->Update();

    auto *polyData = reader->GetOutput();

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

    // ===================== create sorted points ============================
    polyData->BuildCells();
    polyData->BuildLinks();
    QVector<vtkIdType> sortedIds;

    auto selectionPoints = polyData->GetPoints();
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

    vtkSPtrNew( neighborCellIds, vtkIdList );
    QVector<bool> cellVisited;
    for( int i = 0; i < polyData->GetNumberOfCells(); ++i )
    {
        cellVisited.push_back( false );
    }

    //polyData->GetCellNeighbors( lastCellId, ids, neighborCellIds );
    //polyData->GetCellEdgeNeighbors( lastCellId, 0, 1, neighborCellIds );

    while ( !cellVisited[lastCellId] && sortedIds.size() < polyData->GetNumberOfPoints() )
    {
        polyData->GetPointCells( sortedIds[sortedIds.size() - 1], neighborCellIds );
        cout << "lastCellId neighborCellIds: " << neighborCellIds->GetNumberOfIds() << endl;
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
                    break;
                }
            }
        }
    }
    // ===================== finished: sorted points ============================

    for( int i = 0; i < sortedIds.size(); ++i )
    {
        // text 2D
        vtkSmartPointer<vtkTextSource> text2D =
                vtkSmartPointer<vtkTextSource>::New();
        text2D->SetText( QString::number( i ).toStdString().c_str() );

        vtkSmartPointer<vtkTransform> text2DTransform =
                vtkSmartPointer<vtkTransform>::New();
        double *center = selectionPoints->GetPoint( sortedIds[i] );
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
    }

    renderer->ResetCamera();
    renderWindow->Render();
    renderWindowInteractor->Start();
    return 0;
}
