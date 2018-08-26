#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkPolyData.h>
#include <vtkSphereSource.h>
#include <vtkAxesActor.h>
#include <vtkTransform.h>
#include <vtkSmartPointer.h>
#include <vtkPlaneSource.h>
#include <vtkProperty.h>

#include <ostream>
#include <iostream>
#include <unistd.h>
#include <QDebug>
using namespace std;

int main()
{
    vtkSmartPointer<vtkTransform> transform =
        vtkSmartPointer<vtkTransform>::New();
    transform->PreMultiply();

    vtkSmartPointer<vtkPlaneSource> planeSource =
            vtkSmartPointer<vtkPlaneSource>::New();
    vtkSmartPointer<vtkPolyDataMapper> planeMapper =
            vtkSmartPointer<vtkPolyDataMapper>::New();
    planeMapper->SetInputConnection(planeSource->GetOutputPort());
    vtkSmartPointer<vtkActor> planeActor =
            vtkSmartPointer<vtkActor>::New();
    planeActor->SetMapper( planeMapper );
    planeActor->GetProperty()->SetColor( 1, 0, 0 );
    planeActor->SetUserTransform(transform);

//* the three basic linear transformations
//*/
//void Translate(double x, double y, double z);
//void Rotate(double angle, double x, double y, double z);
//void Scale(double x, double y, double z);

    vtkSmartPointer<vtkTransform> transform2 =
        vtkSmartPointer<vtkTransform>::New();
    transform2->SetInput( planeActor->GetUserTransform() );
    vtkMatrix4x4 *matrix = transform2->GetMatrix();

    cout << "---------------------------------\n";
    ostream out( std::cout.rdbuf() );
    matrix->PrintSelf( out, *vtkIndent::New() );

    //transform->RotateWXYZ( 90, 1, 0, 0 );
    //transform->RotateX( 135 );
    //transform->RotateZ( 45 );
    matrix = transform2->GetMatrix();

    cout << "---------------------------------\n";
    ostream out1( std::cout.rdbuf() );
    matrix->PrintSelf( out1, *vtkIndent::New() );


    vtkSmartPointer<vtkRenderer> renderer =
        vtkSmartPointer<vtkRenderer>::New();
    vtkSmartPointer<vtkRenderWindow> renderWindow =
        vtkSmartPointer<vtkRenderWindow>::New();
    renderWindow->AddRenderer(renderer);
//    vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor =
//        vtkSmartPointer<vtkRenderWindowInteractor>::New();
//    renderWindowInteractor->SetRenderWindow(renderWindow);
    renderer->SetBackground(.1, .2, .3);

    renderer->AddActor(planeActor);
    renderer->ResetCamera();
    renderWindow->Render();
    //renderWindowInteractor->Start();

    transform2->RotateZ( 45 );
    planeActor->SetUserTransform( transform2 );

    int i;
    for (i = 0; i < 90; ++i)
    {
      // render the image
      //transform2->RotateX( 5 );
      transform2->RotateY( 5 );
      planeActor->SetUserTransform( transform2 );
      renderWindow->Render();
      usleep( 200000 );
      matrix = transform2->GetMatrix();

      qDebug() << "---------------------------------\n";
      ostream out1( std::cout.rdbuf() );
      matrix->PrintSelf( out1, *vtkIndent::New() );
    }

    return 0;
}
