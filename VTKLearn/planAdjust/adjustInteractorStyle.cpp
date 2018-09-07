#include "adjustInteractorStyle.h"

#include <vtkRenderWindowInteractor.h>
#include <vtkAbstractPicker.h>
#include <vtkRenderWindow.h>
#include <vtkRendererCollection.h>
#include <stdio.h>
#include <unistd.h>
#include <QDebug>

vtkStandardNewMacro( AdjustInteractorStyle )

void AdjustInteractorStyle::OnMouseMove()
{
    vtkInteractorStyleTrackballCamera::OnMouseMove();
}

void AdjustInteractorStyle::OnLeftButtonDown()
{
    char buffer[1024];

    write( STDOUT_FILENO, "OnLeftButtonDown\n", 17 );

    this->Interactor->GetPicker()->Pick(
            this->Interactor->GetEventPosition()[0],
            this->Interactor->GetEventPosition()[1],
            this->Interactor->GetEventPosition()[2],
            this->Interactor->GetRenderWindow()->GetRenderers()->GetFirstRenderer()
            );
    Interactor->GetPicker()->GetPickPosition( picked );

    qDebug( "OnLeftButtonDown picked: (%lf, %lf, %lf)", picked[0], picked[1], picked[2] );
    vtkInteractorStyleTrackballCamera::OnLeftButtonDown();

    double *orientation = plane->planeActor->GetOrientationWXYZ();
    qDebug( "GetOrientationWXYZ: (%lf, %lf, %lf, %lf)", orientation[0], orientation[1],
            orientation[2], orientation[3] );
}

void AdjustInteractorStyle::OnRightButtonDown()
{

}

void AdjustInteractorStyle::fetchPlane(Plane *_plane)
{
    plane = _plane;
}

void AdjustInteractorStyle::fetchDragCone(DragCone *_dragCone)
{
    dragCone = _dragCone;
}

AdjustInteractorStyle::AdjustInteractorStyle()
{

}
