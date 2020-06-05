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

//#include "ULineSegmentsToConnectedList.h"
#include "ConnectedEdgeFilter.hpp"

int main(int, char *[])
{
    vSPNew( reader, vtkXMLPolyDataReader );
    reader->SetFileName( "../intersectCircle3.vtp" );
    reader->Update();

    auto *polyData = reader->GetOutput();

    vSPNew( connectivityF, vtkPolyDataConnectivityFilter );
    connectivityF->SetInputData( polyData );
    connectivityF->SetExtractionModeToLargestRegion();
    connectivityF->Update();

    polyData = connectivityF->GetOutput();

    vSPNew( mapper, vtkPolyDataMapper );
    mapper->SetInputData( polyData );

    vSPNew( actor, vtkActor );
    actor->SetMapper( mapper );

    vSPNew( renderer, vtkRenderer );
    renderer->AddActor(actor);
    renderer->SetBackground( 0, 0, 0 );

    vSPNew( renderWindow, vtkRenderWindow );
    renderWindow->AddRenderer( renderer );

    vSPNew( renderWindowInteractor, vtkRenderWindowInteractor );
    renderWindowInteractor->SetRenderWindow( renderWindow );

    // ===================== connectivity filter ============================
    ConnectedEdgeFilter* connectFilter = new ConnectedEdgeFilter;
    connectFilter->Initialise( polyData->GetLines() );
    connectFilter->HandleEdges();

    vSP<vtkPoints> connectPoints = vSP<vtkPoints>::New();
    vSP<vtkIdList> longestList = connectFilter->GetLongestList();
    for (vtkIdType i = 0; i < longestList->GetNumberOfIds(); i++)
    {
        connectPoints->InsertNextPoint( polyData->GetPoint(longestList->GetId(i)) );
    }
    // ===================== Finish: connectivity filter ============================

    for( int i = 0; i < connectPoints->GetNumberOfPoints(); ++i )
    //for( int i = 0; i < selectionPoints->GetNumberOfPoints(); ++i )
    {
        // text 2D
        vtkSmartPointer<vtkTextSource> text2D =
                vtkSmartPointer<vtkTextSource>::New();
        text2D->SetText( QString::number( i ).toStdString().c_str() );

        vtkSmartPointer<vtkTransform> text2DTransform =
                vtkSmartPointer<vtkTransform>::New();
        //double *center = selectionPoints->GetPoint( sortedIds[i] );
        double *center = connectPoints->GetPoint( i );
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
