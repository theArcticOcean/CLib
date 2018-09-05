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
#include <vtkNamedColors.h>

#include <ostream>
#include <iostream>
#include <unistd.h>
#include <QDebug>

using namespace std;


//vtkNamedColors:   https://en.wikipedia.org/wiki/Web_colors


int main()
{
    vtkSmartPointer<vtkNamedColors> namedColors =
            vtkSmartPointer<vtkNamedColors>::New();

    vtkSmartPointer<vtkPlaneSource> planeSource =
            vtkSmartPointer<vtkPlaneSource>::New();

    vtkSmartPointer<vtkPolyDataMapper> planeMapper =
            vtkSmartPointer<vtkPolyDataMapper>::New();
    planeMapper->SetInputConnection(planeSource->GetOutputPort());

    vtkSmartPointer<vtkActor> planeActor =
            vtkSmartPointer<vtkActor>::New();
    planeActor->SetMapper( planeMapper );
    planeActor->GetProperty()->SetColor( namedColors->GetColor3d("Peacock").GetData() );

    vtkSmartPointer<vtkRenderer> renderer =
        vtkSmartPointer<vtkRenderer>::New();

    vtkSmartPointer<vtkRenderWindow> renderWindow =
        vtkSmartPointer<vtkRenderWindow>::New();
    renderWindow->AddRenderer(renderer);
    renderer->SetBackground(.1, .2, .3);

    renderer->AddActor(planeActor);
    renderer->ResetCamera();
    renderWindow->Render();

    vtkSmartPointer<vtkRenderWindowInteractor> renderWindowIterator =
            vtkSmartPointer<vtkRenderWindowInteractor>::New();
    renderWindowIterator->SetRenderWindow( renderWindow );
    renderWindowIterator->Start();
    return 0;
}
