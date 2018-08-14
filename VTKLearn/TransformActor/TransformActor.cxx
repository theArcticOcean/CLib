#include <vtkSmartPointer.h>
#include <vtkConeSource.h>
#include <vtkTransform.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkSphereSource.h>

int main(int, char *[])
{
  // Create a cone
  vtkSmartPointer<vtkConeSource> coneSource1 = 
    vtkSmartPointer<vtkConeSource>::New();
  coneSource1->Update();
  
  vtkSmartPointer<vtkConeSource> coneSource2 = 
    vtkSmartPointer<vtkConeSource>::New();
  coneSource2->Update();

  vtkSmartPointer<vtkSphereSource> sphereSource =
    vtkSmartPointer<vtkSphereSource>::New();
  sphereSource->Update();
  
  vtkSmartPointer<vtkPolyDataMapper> mapper1 = 
    vtkSmartPointer<vtkPolyDataMapper>::New();
  mapper1->SetInputConnection(coneSource1->GetOutputPort());
  vtkSmartPointer<vtkActor> actor1 = 
    vtkSmartPointer<vtkActor>::New();
  actor1->SetMapper(mapper1);
  
  // Create a second, transformed cone
  vtkSmartPointer<vtkPolyDataMapper> mapper2 = 
    vtkSmartPointer<vtkPolyDataMapper>::New();
  mapper2->SetInputConnection(coneSource2->GetOutputPort());
  vtkSmartPointer<vtkActor> actor2 = 
    vtkSmartPointer<vtkActor>::New();
  actor2->SetMapper(mapper2);

  vtkSmartPointer<vtkPolyDataMapper> sphereMapper =
    vtkSmartPointer<vtkPolyDataMapper>::New();
  sphereMapper->SetInputConnection(sphereSource->GetOutputPort());
  vtkSmartPointer<vtkActor> sphereActor =
    vtkSmartPointer<vtkActor>::New();
  sphereActor->SetMapper(sphereMapper);
  
  vtkSmartPointer<vtkTransform> transform = 
    vtkSmartPointer<vtkTransform>::New();
  transform->PostMultiply(); //this is the key line, M = A*M ==> M = M * A
  //transform->RotateZ(90.0);
  double matrix[ 16 ] = {
      0.5, 0, 0, 0.5,
      0, 0.5, 0, 0.5,
      0, 0, 0.5, 0.5,
      0, 0, 0, 1    };

  transform->SetMatrix( matrix );
  //transform->RotateY(90.0);
  //transform->RotateX(90.0);

  vtkSmartPointer<vtkTransform> sphereTransform =
    vtkSmartPointer<vtkTransform>::New();
  sphereTransform->PostMultiply(); //this is the key line, M = A*M ==> M = M * A
  sphereTransform->RotateZ(90.0);

  actor2->SetUserTransform(transform);
  sphereActor->SetUserTransform( sphereTransform );
  
  // Create a renderer, render window, and interactor
  vtkSmartPointer<vtkRenderer> renderer = 
    vtkSmartPointer<vtkRenderer>::New();
  vtkSmartPointer<vtkRenderWindow> renderWindow = 
    vtkSmartPointer<vtkRenderWindow>::New();
  renderWindow->AddRenderer(renderer);
  vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor = 
    vtkSmartPointer<vtkRenderWindowInteractor>::New();
  renderWindowInteractor->SetRenderWindow(renderWindow);

  // Add the actor to the scene
  renderer->AddActor(actor1);
  renderer->AddActor(actor2);
  //renderer->AddActor( sphereActor );
  renderer->SetBackground(1,1,1); // Background color white

  // Render and interact
  renderWindow->Render();
  renderWindowInteractor->Start();

  return EXIT_SUCCESS;
}
