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
#include <vtkTextActor.h>
#include <QDebug>
#include <vtkCaptionActor2D.h>
#include <vtkTextProperty.h>
#include <vtkClipPolyData.h>
#include <vtkTransform.h>
#include <vtkPlane.h>
#include <vtkTransformFilter.h>
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
    axes->SetTotalLength( 1, 1, 1 ); // change length of three axis
    vtkPtr( cellPicker, vtkCellPicker );

    vtkPtr(parametricTorus, vtkParametricTorus);
    parametricTorus->SetRingRadius(1);
    parametricTorus->SetCrossSectionRadius(0.1);

    vtkPtr(parametricFunctionSource, vtkParametricFunctionSource);
    parametricFunctionSource->SetParametricFunction(parametricTorus);
    parametricFunctionSource->Update();

    vtkPtr( clipPlane, vtkPlane );
    clipPlane->SetOrigin( 0, 0, 0 );
    clipPlane->SetNormal( 0, 0, 1 );

    // ===== Z polydata and actor =====
    vtkPtr( zRingClipper, vtkClipPolyData );
    zRingClipper->SetInputData( parametricFunctionSource->GetOutput() );
    zRingClipper->SetClipFunction( clipPlane );
    //zRingClipper->InsideOutOn();

    vtkPtr( zRingMapper, vtkPolyDataMapper );
    zRingMapper->SetInputConnection( zRingClipper->GetOutputPort() );

    vtkPtr( zRingActor, vtkActor );
    zRingActor->SetMapper( zRingMapper );
    zRingActor->GetProperty()->SetOpacity( 0.5 );
    zRingActor->GetProperty()->SetColor( 1, 0, 0 );

    // ===== X polydata and actor =====
    vtkPtr( xTransformFilter, vtkTransformFilter );
    xTransformFilter->SetInputData( parametricFunctionSource->GetOutput() );

    vtkPtr( transform, vtkTransform );
    transform->RotateY( 90 );
    xTransformFilter->SetTransform( transform );
    xTransformFilter->Update();

    vtkPtr( xRingClipper, vtkClipPolyData );
    xRingClipper->SetInputData( xTransformFilter->GetOutput() );
    xRingClipper->SetClipFunction( clipPlane );
    //xRingClipper->InsideOutOn();
    xRingClipper->Update();
    qDebug() << "xRingClipper: " << xRingClipper->GetClippedOutput()->GetNumberOfCells();

    vtkPtr( xRingMapper, vtkPolyDataMapper );
    xRingMapper->SetInputConnection( xRingClipper->GetOutputPort() );

    vtkPtr( xRingActor, vtkActor );
    xRingActor->SetMapper( xRingMapper );
    xRingActor->GetProperty()->SetOpacity( 0.5 );
    xRingActor->GetProperty()->SetColor( 0, 1, 0 );

    // ===== Y polydata and actor =====
    vtkPtr( yTransformFilter, vtkTransformFilter );
    yTransformFilter->SetInputData( parametricFunctionSource->GetOutput() );

    vtkPtr( transform2, vtkTransform );
    transform2->RotateX( 90 );
    yTransformFilter->SetTransform( transform2 );
    yTransformFilter->Update();

    vtkPtr( yRingClipper, vtkClipPolyData );
    yRingClipper->SetInputData( yTransformFilter->GetOutput() );
    yRingClipper->SetClipFunction( clipPlane );
    //yRingClipper->InsideOutOn();
    yRingClipper->Update();

    vtkPtr( yRingMapper, vtkPolyDataMapper );
    yRingMapper->SetInputConnection( yRingClipper->GetOutputPort() );

    vtkPtr( yRingActor, vtkActor );
    yRingActor->SetMapper( yRingMapper );
    yRingActor->GetProperty()->SetOpacity( 0.5 );
    yRingActor->GetProperty()->SetColor( 0, 0, 1 );
    // ====== end ========

    renderer->AddActor( zRingActor );
    renderer->AddActor( xRingActor );
    renderer->AddActor( yRingActor );
    //renderer->AddActor( axes );
    renderer->AddActor( coneActor );

    cellPicker->AddPickList( zRingActor );
    cellPicker->AddPickList( xRingActor );
    cellPicker->AddPickList( yRingActor );
    cellPicker->PickFromListOn();

    renderer->ResetCamera();

    vtkPtr( style, vtkCustomStyle );
    //style->Setm_Axes( axes );
    style->Setm_ConeActor( coneActor );
    style->Setm_ZRingActor( zRingActor );
    style->Setm_CellPicker( cellPicker );
    style->Setm_Render( renderer );
    style->Setm_Interator( renderWindowInteractor );
    style->Setm_RenderWindow( renderWindow );
    style->Setm_XRingActor( xRingActor );
    style->Setm_YRingActor( yRingActor );
    style->Setm_Plane( clipPlane );

    renderWindowInteractor->SetInteractorStyle( style );
    renderWindow->Render();
    renderWindowInteractor->Start();
    return 0;
}
