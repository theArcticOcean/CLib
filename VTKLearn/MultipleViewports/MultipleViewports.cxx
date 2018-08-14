#include <vtkSphereSource.h>
#include <vtkProperty.h>
#include <vtkPolyData.h>
#include <vtkSmartPointer.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>
#include <unistd.h>

int main(int, char *[])
{
  vtkSmartPointer<vtkRenderWindow> renderWindow =
    vtkSmartPointer<vtkRenderWindow>::New();

  vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor =
    vtkSmartPointer<vtkRenderWindowInteractor>::New();

  renderWindowInteractor->SetRenderWindow(renderWindow);

  // Define viewport ranges
  double position[4] = { 0.05, 0.05, .35, .35 };
  double offset[9][2] ={ { 0, 0 }, { 0, .3 }, { 0, .6 },
                         {.3, 0 }, { .3,.3 }, { .3,.6 },
                         { .6,0 }, { .6,.3 }, { .6,.6 } };

  for(unsigned i = 0; i < 9; i++)
  {
    vtkSmartPointer<vtkRenderer> renderer =
      vtkSmartPointer<vtkRenderer>::New();

    renderWindow->AddRenderer(renderer);
    renderer->SetViewport( position[0] + offset[i][0], position[1] + offset[i][1] ,
                           position[2] + offset[i][0], position[3] + offset[i][1] );
    char buff[105];
    sprintf( buff, "( %lf, %lf, %lf, %lf )\n", position[0] + offset[i][0], position[1] + offset[i][1] ,
            position[2] + offset[i][0], position[3] + offset[i][1] );
    write( STDOUT_FILENO, buff, strlen(buff) + 1 );

    // Create a sphere
    vtkSmartPointer<vtkSphereSource> sphereSource =
      vtkSmartPointer<vtkSphereSource>::New();
    sphereSource->SetCenter(0.0, 0.0, 0.0);
    sphereSource->SetRadius(5);
    sphereSource->Update();

    // Create a mapper and actor
    vtkSmartPointer<vtkPolyDataMapper> mapper =
      vtkSmartPointer<vtkPolyDataMapper>::New();
    mapper->SetInputConnection(sphereSource->GetOutputPort());
    vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
    actor->SetMapper(mapper);
    renderer->AddActor(actor);
    renderer->ResetCamera();

    renderWindow->Render();
    renderWindow->SetWindowName("Multiple ViewPorts");
  }

  renderWindowInteractor->Start();

  return EXIT_SUCCESS;
}
