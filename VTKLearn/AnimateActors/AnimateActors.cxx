#include "AnimateActors.h"
#include <vtkSmartPointer.h>
#include <vtkAnimationCue.h>
#include <vtkRenderer.h>
#include <vtkSphereSource.h>
#include <vtkConeSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkCommand.h>
#include <vtkAnimationScene.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkCamera.h>

int main(int argc, char *argv[])
{
  // Create the graphics structure. The renderer renders into the
  // render window.
  vtkSmartPointer<vtkRenderWindowInteractor> iren =
    vtkSmartPointer<vtkRenderWindowInteractor>::New();
  vtkSmartPointer<vtkRenderer> ren1 =
    vtkSmartPointer<vtkRenderer>::New();
  vtkSmartPointer<vtkRenderWindow> renWin =
    vtkSmartPointer<vtkRenderWindow>::New();
  //renWin->SetMultiSamples(0);
  iren->SetRenderWindow(renWin);
  renWin->AddRenderer(ren1);

  // Generate a sphere
  vtkSmartPointer<vtkSphereSource> sphereSource =
    vtkSmartPointer<vtkSphereSource>::New();
  vtkSmartPointer<vtkPolyDataMapper> sphereMapper =
    vtkSmartPointer<vtkPolyDataMapper>::New();
  sphereMapper->SetInputConnection( sphereSource->GetOutputPort());
  vtkSmartPointer<vtkActor> sphere =
    vtkSmartPointer<vtkActor>::New();
  sphere->SetMapper(sphereMapper);
  ren1->AddActor(sphere);

  // Generate a cone
  vtkSmartPointer<vtkConeSource> coneSource =
    vtkSmartPointer<vtkConeSource>::New();
  vtkSmartPointer<vtkPolyDataMapper> coneMapper =
    vtkSmartPointer<vtkPolyDataMapper>::New();
  coneMapper->SetInputConnection( coneSource->GetOutputPort());
  vtkSmartPointer<vtkActor> cone =
    vtkSmartPointer<vtkActor>::New();
  cone->SetMapper(coneMapper);
  ren1->AddActor(cone);

  // Create an Animation Scene
  vtkSmartPointer<vtkAnimationScene> scene =
    vtkSmartPointer<vtkAnimationScene>::New();
  if(argc>=2 && strcmp(argv[1],"-real")==0)
  {
    cout << "real-time mode" << endl;
    scene->SetModeToRealTime();
  }
  else
  {
    cout << "sequence mode" << endl;
    scene->SetModeToSequence();
  }
  scene->SetLoop(0);
  scene->SetFrameRate(5);   // 50 value can let us to see squence mode also plays.
  scene->SetStartTime(0);
  scene->SetEndTime(11);

  vtkSmartPointer<AnimationSceneObserver> sceneObserver =
    vtkSmartPointer<AnimationSceneObserver>::New();
  sceneObserver->SetRenderWindow(renWin);
  scene->AddObserver(vtkCommand::AnimationCueTickEvent,sceneObserver);

  // Create an Animation Cue for each actor
  vtkSmartPointer<vtkAnimationCue> cue1 =
    vtkSmartPointer<vtkAnimationCue>::New();
  cue1->SetStartTime(1);
  cue1->SetEndTime(10);
  scene->AddCue(cue1);

  vtkSmartPointer<vtkAnimationCue> cue2 =
    vtkSmartPointer<vtkAnimationCue>::New();
  cue2->SetStartTime(1);
  cue2->SetEndTime(10);
  scene->AddCue(cue2);

  // Create an ActorAnimator for each actor;
  ActorAnimator animateSphere;
  std::vector<double> endSphere(3);
    endSphere[0] = 1;
    endSphere[1] = 1;
    endSphere[2] = 1;
  animateSphere.SetEndPosition( endSphere );
  animateSphere.SetActor(sphere);
  animateSphere.AddObserversToCue(cue1);

  ActorAnimator animateCone;
  std::vector<double> endCone(3);
    endCone[0] = -1;
    endCone[1] = -1;
    endCone[2] = -1;
  animateCone.SetEndPosition(endCone);
  animateCone.SetActor(cone);
  animateCone.AddObserversToCue(cue2);

  renWin->Render();
  ren1->ResetCamera();
  // When the parameter is bigger than 1, means to close focal point.
  // When the parameter is smaller than 1, means be away form focal point.
  ren1->GetActiveCamera()->Dolly(.5);
  ren1->ResetCameraClippingRange();

  // Create Cue observer.
  scene->Play();
  //scene->Stop(); //doesn't matter

  iren->Start();
  return EXIT_SUCCESS;
}
