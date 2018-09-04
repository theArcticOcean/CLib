#include "MyStyle.h"
#include <vtkInteractorObserver.h>
#include <vtkCellPicker.h>
#include <vtkSmartPointer.h>
#include <vtkObjectFactory.h>
#include <vtkRenderWindowInteractor.h>

vtkStandardNewMacro(MyStyle);

void MyStyle::OnLeftButtonDown()
{
    // Get the location of the click (in window coordinates)
    int* pos = this->GetInteractor()->GetEventPosition();

    vtkSmartPointer<vtkCellPicker> picker =
      vtkSmartPointer<vtkCellPicker>::New();
    picker->SetTolerance(0.0005);

    // Pick from this location.
    picker->Pick(pos[0], pos[1], 0, this->GetDefaultRenderer());
    double* worldPosition = picker->GetPickPosition();

    printf( "click: %lf %lf %lf\n",  worldPosition[0], worldPosition[1], worldPosition[2] );
    vtkInteractorStyleTrackballCamera::OnLeftButtonDown();
}
