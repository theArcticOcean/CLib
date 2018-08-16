#include <iostream>
#include <vtkSmartPointer.h>
#include <vtkSphereSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkRenderWindow.h>
#include <vtkProperty.h>

using namespace std;

/*
 * Z: 向屏幕外为正。
 * Y: 窗口顶部到顶部为正方向
 * X:
 */
int main()
{
    // Sphere 1
    vtkSmartPointer<vtkSphereSource> sphereSource1 =
      vtkSmartPointer<vtkSphereSource>::New();
    sphereSource1->SetCenter(0.0, 0.0, 0.0);
    sphereSource1->SetRadius(1.0);
    sphereSource1->Update();

    // Mapper1
    vtkSmartPointer<vtkPolyDataMapper> mapper1 =
      vtkSmartPointer<vtkPolyDataMapper>::New();
    mapper1->SetInputConnection(sphereSource1->GetOutputPort());

    // Actor 1
    vtkSmartPointer<vtkActor> actor1 =
      vtkSmartPointer<vtkActor>::New();
    actor1->SetMapper(mapper1);

    // Sphere 2
    vtkSmartPointer<vtkSphereSource> sphereSource2 =
      vtkSmartPointer<vtkSphereSource>::New();
    sphereSource2->SetCenter(2.0, 0.0, 0.0);
    sphereSource2->SetRadius(1.0);
    sphereSource2->Update();

    // Mapper 2
    vtkSmartPointer<vtkPolyDataMapper> mapper2 =
      vtkSmartPointer<vtkPolyDataMapper>::New();
    mapper2->SetInputConnection(sphereSource2->GetOutputPort());

    // Actor 2
    vtkSmartPointer<vtkActor> actor2 =
      vtkSmartPointer<vtkActor>::New();
    actor2->GetProperty()->SetColor( 1, 0, 0 );
    actor2->SetMapper(mapper2);

    // A renderer and render window
    vtkSmartPointer<vtkRenderer> renderer =
      vtkSmartPointer<vtkRenderer>::New();
    vtkSmartPointer<vtkRenderWindow> renderWindow =
      vtkSmartPointer<vtkRenderWindow>::New();
    renderWindow->AddRenderer(renderer);

    // An interactor
    vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor =
      vtkSmartPointer<vtkRenderWindowInteractor>::New();
    renderWindowInteractor->SetRenderWindow(renderWindow);

    // Add the actors to the scene
    renderer->AddActor(actor1);
    renderer->AddActor(actor2);
    renderer->SetBackground(1,1,1); // Background color white

    // Render an image (lights and cameras are created automatically)
    renderWindow->Render();

    vtkSmartPointer<vtkInteractorStyleTrackballCamera> style =
      vtkSmartPointer<vtkInteractorStyleTrackballCamera>::New();

    //renderWindowInteractor->SetInteractorStyle( NULL );  // this will cancel Interactor
    renderWindowInteractor->SetInteractorStyle( style );

    // Begin mouse interaction
    renderWindowInteractor->Start();
    return 0;
}
