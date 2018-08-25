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

#include <ostream>
#include <iostream>
using namespace std;

int main()
{
    vtkSmartPointer<vtkTransform> transform =
        vtkSmartPointer<vtkTransform>::New();
    transform->PreMultiply();

    vtkSmartPointer<vtkAxesActor> axes =
        vtkSmartPointer<vtkAxesActor>::New();
    axes->SetUserTransform(transform);

//* the three basic linear transformations
//*/
//void Translate(double x, double y, double z);
//void Rotate(double angle, double x, double y, double z);
//void Scale(double x, double y, double z);

    vtkSmartPointer<vtkTransform> transform2 =
        vtkSmartPointer<vtkTransform>::New();
    transform2->SetInput( axes->GetUserTransform() );
    vtkMatrix4x4 *matrix = transform2->GetMatrix();

    cout << "---------------------------------\n";
    ostream out( std::cout.rdbuf() );
    matrix->PrintSelf( out, *vtkIndent::New() );

    //transform->RotateWXYZ( 90, 1, 0, 0 );
    matrix = transform2->GetMatrix();

    cout << "---------------------------------\n";
    ostream out1( std::cout.rdbuf() );
    matrix->PrintSelf( out1, *vtkIndent::New() );


    vtkSmartPointer<vtkRenderer> renderer =
        vtkSmartPointer<vtkRenderer>::New();
    vtkSmartPointer<vtkRenderWindow> renderWindow =
        vtkSmartPointer<vtkRenderWindow>::New();
    renderWindow->AddRenderer(renderer);
    vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor =
        vtkSmartPointer<vtkRenderWindowInteractor>::New();
    renderWindowInteractor->SetRenderWindow(renderWindow);
    renderer->SetBackground(.1, .2, .3);

    renderer->AddActor(axes);
    renderer->ResetCamera();
    renderWindow->Render();
    renderWindowInteractor->Start();

    return 0;
}
