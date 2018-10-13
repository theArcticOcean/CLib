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

int main(int argc, char *argv[])
{   
    vtkSmartPointer<vtkPolyData> planePld =
            vtkSmartPointer<vtkPolyData>::New();

    vtkSmartPointer<vtkPoints> planePoints =
            vtkSmartPointer<vtkPoints>::New();

    vtkSmartPointer<vtkCellArray> planeCells =
            vtkSmartPointer<vtkCellArray>::New();

    vtkSmartPointer<vtkPolyDataMapper> planeMapper =
            vtkSmartPointer<vtkPolyDataMapper>::New();

    vtkSmartPointer<vtkActor> planeActor =
            vtkSmartPointer<vtkActor>::New();

    vtkSmartPointer<vtkRenderer> renderer =
            vtkSmartPointer<vtkRenderer>::New();
    renderer->SetBackground( 1, 1, 1 );

    vtkSmartPointer<vtkRenderWindow> renderWindow =
            vtkSmartPointer<vtkRenderWindow>::New();
    renderWindow->AddRenderer( renderer );

    //-----------start create plane------------------
    planePoints->SetNumberOfPoints( 4 );
    double width = renderWindow->GetScreenSize()[0] / 2.0;
    double height = renderWindow->GetScreenSize()[1] / 2.0;
    planePoints->InsertPoint(0, width, height, 0);
    planePoints->InsertPoint(1, -width, height, 0);
    planePoints->InsertPoint(2, width, -height, 0);
    planePoints->InsertPoint(3, -width, -height, 0);
    planePld->SetPoints( planePoints );
    vtkIdType cellId1[3] = { 1, 2, 3 };
    vtkIdType cellId2[3] = { 1, 2, 0 };
    //Create a cell by specifying the number of points and an array of pointid's.  Return the cell id of the cell.
    planeCells->InsertNextCell( 3, cellId1 );
    planeCells->InsertNextCell( 3, cellId2 );
    planePld->SetPolys( planeCells );
    planeMapper->SetInputData( planePld );
    planeActor->SetMapper( planeMapper );
    planeActor->GetProperty()->SetOpacity( 0.5 );
    planeActor->GetProperty()->SetColor( 0.4, 0.2, 0.7 );

    renderer->AddActor( planeActor );
    //-----------create plane finished------------------

    vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor =
            vtkSmartPointer<vtkRenderWindowInteractor>::New();
    renderWindowInteractor->SetRenderWindow( renderWindow );

    renderer->GetActiveCamera()->ParallelProjectionOn();
    renderer->ResetCamera();
    renderWindow->Render();
    renderWindowInteractor->Start();

    return 0;
}
