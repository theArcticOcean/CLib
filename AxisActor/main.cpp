#include <iostream>
#include <vtkSmartPointer.h>
#include <vtkSphereSource.h>
#include <vtkActor.h>
#include <vtkConeSource.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkLight.h>
#include <vtkCamera.h>
#include <vtkAxisActor.h>
#include <vtkActor2D.h>
#include <vtkAxesActor.h>
#include <vtkParametricTorus.h>
#include <vtkCommand.h>
#include <vtkCellPicker.h>
#include <vtkTransform.h>
#include <vtkParametricFunctionSource.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <QString>
#include <QDebug>

#include "vtkCustomStyle.h"

using namespace std;

int main()
{
    setbuf( stdout, nullptr );
    vtkPtr( cone, vtkConeSource );

    vtkPtr( coneMapper, vtkPolyDataMapper );
    coneMapper->SetInputConnection( cone->GetOutputPort() );

    vtkPtr( coneActor, vtkActor );
    coneActor->SetMapper( coneMapper );

    vtkPtr( renderer, vtkRenderer );
    renderer->SetBackground( 0, 0, 0 );

    vtkPtr( renderWindow, vtkRenderWindow );
    renderWindow->AddRenderer( renderer );

    vtkPtr( renderWindowInteractor, vtkRenderWindowInteractor );
    renderWindowInteractor->SetRenderWindow( renderWindow );

    vtkPtr( axes, vtkAxesActor );
    vtkPtr( cellPicker, vtkCellPicker );

    vtkPtr(parametricTorus, vtkParametricTorus);
    parametricTorus->SetRingRadius(1);
    parametricTorus->SetCrossSectionRadius(0.1);

    vtkPtr(parametricFunctionSource, vtkParametricFunctionSource);
    parametricFunctionSource->SetParametricFunction(parametricTorus);
    parametricFunctionSource->Update();

    vtkPtr( parametricMapper, vtkPolyDataMapper );
    parametricMapper->SetInputData( parametricFunctionSource->GetOutput() );

    vtkPtr( zRingActor, vtkActor );
    zRingActor->SetMapper( parametricMapper );
    zRingActor->GetProperty()->SetOpacity( 0.5 );

    vtkPtr( xRingActor, vtkActor );
    xRingActor->SetMapper( parametricMapper );
    xRingActor->GetProperty()->SetOpacity( 0.5 );
    xRingActor->RotateY( 90 );

    vtkPtr( yRingActor, vtkActor );
    yRingActor->SetMapper( parametricMapper );
    yRingActor->GetProperty()->SetOpacity( 0.5 );
    yRingActor->RotateX( 90 );

    renderer->AddActor( zRingActor );
    renderer->AddActor( xRingActor );
    renderer->AddActor( yRingActor );
    renderer->AddActor( axes );
    renderer->AddActor( coneActor );

    cellPicker->AddPickList( zRingActor );
    cellPicker->AddPickList( xRingActor );
    cellPicker->AddPickList( yRingActor );
    cellPicker->PickFromListOn();

    renderer->ResetCamera();

    vtkPtr( style, vtkCustomStyle );
    style->Setm_Axes( axes );
    style->Setm_ConeActor( coneActor );
    style->Setm_ZRingActor( zRingActor );
    style->Setm_CellPicker( cellPicker );
    style->Setm_Render( renderer );
    style->Setm_Interator( renderWindowInteractor );
    style->Setm_RenderWindow( renderWindow );
    style->Setm_XRingActor( xRingActor );
    style->Setm_YRingActor( yRingActor );

    renderWindowInteractor->SetInteractorStyle( style );
    renderWindow->Render();
    renderWindowInteractor->Start();
    return 0;
}
