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
#include <vtkCellData.h>

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
    auto selectionPoints = polyData->GetPoints();
//    cout << polyData->GetNumberOfCells() << endl;
//    cout << polyData->GetNumberOfLines() << endl;
//    cout << polyData->GetNumberOfPieces() << endl;
//    cout << polyData->GetNumberOfPolys() << endl;
//    cout << polyData->GetNumberOfStrips() << endl;
//    cout << polyData->GetNumberOfVerts() << endl;

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

    // make same order.
    QMap<int, int> backupMap;
    if( uniIds.size() != selectionPoints->GetNumberOfPoints() )
    {
        QMapIterator<int, int> it( lineIds );
        while ( it.hasNext() )
        {
            it.next();
            if( !uniIds.contains( it.key() ) || !uniIds.contains( it.value() ) )
            {
                backupMap[it.key()] = it.value();
                lineIds[it.key()] = -1;
            }
        }
        cout << "size: " << backupMap.size() << " vs " << lineIds.size() << endl;
        QMap<int, int>::iterator ite = lineIds.begin();
        while ( ite != lineIds.end() )
        {
            if( ite.value() == -1 )
            {
                ite = lineIds.erase( ite );
            }
            else
            {
                ++ite;
            }
        }

        it = QMapIterator<int, int>( backupMap );
        while ( it.hasNext() )
        {
            it.next();
            lineIds[ it.value() ] = it.key();
        }
    }

    // create new sorted list.
    sortedIds.clear();
    sortedIds.push_back( 0 );
    sortedIds.push_back( lineIds[0] );
    while( sortedIds.size() < selectionPoints->GetNumberOfPoints() )
    {
        int lastId = sortedIds[sortedIds.size() - 1];
        sortedIds.push_back( lineIds[lastId] );
    }
    cout << "sortedIds count: " << sortedIds.size() << endl;

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
    }

    renderer->ResetCamera();
    renderWindow->Render();
    renderWindowInteractor->Start();
    return 0;
}
