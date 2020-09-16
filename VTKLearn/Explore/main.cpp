#include <iostream>
#include <vtkSmartPointer.h>
#include <vtkSphereSource.h>
#include <vtkActor.h>
#include <vtkConeSource.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkPolyDataMapper.h>
#include <vtkLineSource.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkAxesActor.h>
#include <vtkProperty.h>
#include <vtkXMLPolyDataReader.h>
#include <vtkCharArray.h>
#include <vtkPointData.h>
#include <vtkConeSource.h>
#include <vtkTransform.h>
#include <vtkColorTransferFunction.h>
#include <vtkCellData.h>
#include <vtkPoints.h>
#include <vtkCell.h>
#include <vtkCellArray.h>
#include <vtkTriangle.h>
#include <vtkLineSource.h>
#include <vtkTriangleFilter.h>
#include <vtkXMLPolyDataReader.h>

#include "../tool.h"

using namespace std;

#define vtkSPtr vtkSmartPointer
#define vtkSPtrNew(Var, Type) vtkSPtr<Type> Var = vtkSPtr<Type>::New();

int main()
{
    vtkSPtrNew( reader, vtkXMLPolyDataReader );
    reader->SetFileName( "../items.vtp" );
    reader->Update();
    vtkSPtrNew( mapper, vtkPolyDataMapper );
    mapper->SetInputData( reader->GetOutput() );
    vtkSPtrNew( actor, vtkActor );
    actor->SetMapper( mapper );

    vtkSPtrNew( renderer, vtkRenderer );
    renderer->AddActor( actor );
    renderer->SetBackground( 0, 0, 0 );

    vtkSPtrNew( renderWindow, vtkRenderWindow );
    renderWindow->AddRenderer( renderer );

    vtkSPtrNew( renderWindowInteractor, vtkRenderWindowInteractor );
    renderWindowInteractor->SetRenderWindow( renderWindow );

    renderer->ResetCamera();
    renderWindow->Render();
    renderWindowInteractor->Start();
    return 0;
}
