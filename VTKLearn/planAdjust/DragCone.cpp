#include "DragCone.h"

#include <QDebug>

DragCone::DragCone()
{
    dragConeSource = vtkSmartPointer<vtkConeSource>::New();
    dragConeSourceMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    dragConeSourceMapper->SetInputConnection( dragConeSource->GetOutputPort() );
    dragConeActor = vtkSmartPointer<vtkActor>::New();
    dragConeActor->SetMapper( dragConeSourceMapper );
    dragConeActor->GetProperty()->SetColor( 0, 1, 0 );
    dragConeTransform = vtkSmartPointer<vtkTransform>::New();
    plane = NULL;
}

void DragCone::fetchPlane(Plane *_plane)
{
    plane = _plane;
}

void DragCone::setPosition()
{
    dragConeActor->SetPosition( 0.5, 0, 0.5 );
    //dragConeTransform->Scale( 0.2, 0.2, 0.2 );
    dragConeTransform->RotateZ( 90 );
    dragConeActor->SetUserTransform( dragConeTransform );
    renderWindow->Render();
}
