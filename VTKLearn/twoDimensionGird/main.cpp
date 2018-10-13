#include <iostream>
#include <vtkSmartPointer.h>
#include <vtkSphereSource.h>
#include <vtkActor.h>
#include <vtkConeSource.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkLight.h>
#include <vtkCamera.h>
#include <vtkActor2D.h>
#include <vtkRendererCollection.h>

using namespace std;

int main()
{
    int mmSize = 1;
    vtkSmartPointer<vtkPolyDataMapper> v1mmMapper =
            vtkSmartPointer<vtkPolyDataMapper>::New();
    vtkSmartPointer<vtkPolyData> v1mmPolyData =
            vtkSmartPointer<vtkPolyData>::New();
    vtkSmartPointer<vtkCellArray> v1mmLines =
            vtkSmartPointer<vtkCellArray>::New();
    vtkSmartPointer<vtkPoints> v1mmPoints =
            vtkSmartPointer<vtkPoints>::New();

    v1mmPolyData->SetPoints( v1mmPoints );
    v1mmPolyData->SetLines( v1mmLines );
    v1mmMapper->SetInputData( v1mmPolyData );

    vtkSmartPointer<vtkActor> v1mmLinesActor =
            vtkSmartPointer<vtkActor>::New();
    v1mmLinesActor->SetMapper( v1mmMapper );
    v1mmLinesActor->GetProperty()->SetColor( 1, 0, 0 );
    v1mmLinesActor->GetProperty()->SetLineWidth( 1 );
    v1mmLinesActor->DragableOff();
    v1mmLinesActor->PickableOff();

    vtkSmartPointer<vtkRenderer> renderer =
            vtkSmartPointer<vtkRenderer>::New();
    renderer->AddActor( v1mmLinesActor );
    renderer->SetBackground( 1, 1, 1 );

    vtkSmartPointer<vtkRenderWindow> renderWindow =
            vtkSmartPointer<vtkRenderWindow>::New();
    renderWindow->AddRenderer( renderer );

    vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor =
            vtkSmartPointer<vtkRenderWindowInteractor>::New();
    renderWindowInteractor->SetRenderWindow( renderWindow );

    //--------------------start generate------------------
    v1mmPoints->Initialize();
    v1mmLines->Initialize();
    double width = renderWindow->GetScreenSize()[0];  //GetActualSize()[0];
    double height = renderWindow->GetScreenSize()[1];
    printf("w: %lf, h: %lf\n", width, height);

    double x, y;
    //--------------------vertical lines------------------
    for ( x = -width / 2; x <= width / 2; x += mmSize )
    {
        double linePoint1[3] = { x, -height/2, 0.0 };
        double linePoint2[3] = { x, height/2, 0.0 };
        vtkIdType pointId1 = v1mmPoints->InsertNextPoint(linePoint1);
        vtkIdType pointId2 = v1mmPoints->InsertNextPoint(linePoint2);
        vtkIdType lineIds[2] = { pointId1, pointId2 };
        v1mmLines->InsertNextCell(2, lineIds);
    }
    //--------------------horizontal lines----------------
    for ( y = -height / 2; y <= height / 2; y += mmSize )
    {
        double linePoint1[3] = { -width/2, y, 0.0 };
        double linePoint2[3] = { width/2, y, 0.0 };
        vtkIdType pointId1 = v1mmPoints->InsertNextPoint(linePoint1);
        vtkIdType pointId2 = v1mmPoints->InsertNextPoint(linePoint2);
        vtkIdType lineIds[2] = { pointId1, pointId2 };
        v1mmLines->InsertNextCell(2, lineIds);
    }

    v1mmPolyData->Modified();
    //--------------------     end      ------------------

    fflush( stdout );
    renderer->GetActiveCamera()->ParallelProjectionOn();
    renderer->GetActiveCamera()->Zoom( 0.05 ); //zoom out
    renderWindow->Render();
    renderWindowInteractor->SetInteractorStyle( nullptr );  //make Interactivity off
    renderWindowInteractor->Start();
    return 0;
}
