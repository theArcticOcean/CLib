#include <vtkVersion.h>
#include <vtkSmartPointer.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkPolyData.h>
#include <vtkSphereSource.h>
#include <vtkTextActor.h>
#include <vtkTextProperty.h>
#include <vtkVectorText.h>
#include <vtkProperty.h>

int main(int, char *[])
{
  // Create a sphere
  vtkSmartPointer<vtkSphereSource> sphereSource = 
    vtkSmartPointer<vtkSphereSource>::New();
  sphereSource->SetCenter ( 0.0, 0.0, 0.0 );
  sphereSource->SetRadius ( 5.0 );
  sphereSource->Update();

  // Create a mapper
  vtkSmartPointer<vtkPolyDataMapper> mapper = 
    vtkSmartPointer<vtkPolyDataMapper>::New();
#if VTK_MAJOR_VERSION <= 5
  mapper->SetInput ( sphereSource->GetOutput() );
#else
  mapper->SetInputData ( sphereSource->GetOutput() );
#endif

  // Create an actor
  vtkSmartPointer<vtkActor> actor = 
    vtkSmartPointer<vtkActor>::New();
  actor->SetMapper ( mapper );

  // Create a renderer
  vtkSmartPointer<vtkRenderer> renderer = 
    vtkSmartPointer<vtkRenderer>::New();
  renderer->SetBackground ( 1, 1, 1 ); // Set background color to white
  renderer->AddActor ( actor );

  // Create a render window
  vtkSmartPointer<vtkRenderWindow> renderWindow = 
    vtkSmartPointer<vtkRenderWindow>::New();
  renderWindow->AddRenderer ( renderer );

  // Create an interactor
  vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor = 
    vtkSmartPointer<vtkRenderWindowInteractor>::New();
  renderWindowInteractor->SetRenderWindow ( renderWindow );

  // Setup the text and add it to the renderer
  vtkSmartPointer<vtkTextActor> textActor = 
    vtkSmartPointer<vtkTextActor>::New();
  textActor->SetInput ( "Hello world" );
  // Controls the upper right corner of the Actor2D.
  // (0,0) is left-upper cornor in 2D world.
  textActor->SetPosition2 ( 10, 40 );

  textActor->GetTextProperty()->SetFontSize ( 24 );
  textActor->GetTextProperty()->SetColor ( 1.0, 0.0, 0.0 );
  renderer->AddActor2D ( textActor );

  vtkSmartPointer<vtkVectorText> text3DSource =
    vtkSmartPointer<vtkVectorText>::New();
  text3DSource->SetText( "VTK" );
  text3DSource->Update();

  vtkSmartPointer<vtkPolyDataMapper> text3DMapper =
    vtkSmartPointer<vtkPolyDataMapper>::New();
  text3DMapper->SetInputConnection(text3DSource->GetOutputPort());

  vtkSmartPointer<vtkActor> text3DActor =
    vtkSmartPointer<vtkActor>::New();
  text3DActor->SetMapper( text3DMapper );
  text3DActor->GetProperty()->SetColor(1.0, 0.0, 0.0);
  // left_bottom corner position in 3D world coordinate
  text3DActor->SetPosition( 5, 5, 0 );

  renderer->AddActor( text3DActor );

  // Render and interact
  renderWindow->Render();
  renderWindowInteractor->Start();

  return EXIT_SUCCESS;
}
