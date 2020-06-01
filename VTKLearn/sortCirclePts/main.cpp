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

#define vtkSPtr vtkSmartPointer
#define vtkSPtrNew(Var, Type) vtkSPtr<Type> Var = vtkSPtr<Type>::New();

int main(int, char *[])
{
    vtkSPtrNew( reader, vtkXMLPolyDataReader );
    reader->SetFileName( "/Users/weiyang/Desktop/intersectCircle.vtp" );
    reader->Update();

    vtkSPtrNew( mapper, vtkPolyDataMapper );
    mapper->SetInputData( reader->GetOutput() );

    vtkSPtrNew( actor, vtkActor );
    actor->SetMapper( mapper );

    vtkSPtrNew( renderer, vtkRenderer );
    renderer->AddActor(actor);
    renderer->SetBackground( 0, 0, 0 );

    vtkSPtrNew( renderWindow, vtkRenderWindow );
    renderWindow->AddRenderer( renderer );

    vtkSPtrNew( renderWindowInteractor, vtkRenderWindowInteractor );
    renderWindowInteractor->SetRenderWindow( renderWindow );

    auto selectionPoints = reader->GetOutput()->GetPoints();
    for( int i = 0; i < selectionPoints->GetNumberOfPoints(); ++i )
    {
        // text 2D
        vtkSmartPointer<vtkTextSource> text2D =
                vtkSmartPointer<vtkTextSource>::New();
        text2D->SetText( QString::number( i+1 ).toStdString().c_str() );

        vtkSmartPointer<vtkTransform> text2DTransform =
                vtkSmartPointer<vtkTransform>::New();
        double *center = selectionPoints->GetPoint( i );
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
