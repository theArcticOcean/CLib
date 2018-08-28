#include "Plane.h"
#include <QDebug>

Plane::Plane()
{
    planeSource = vtkSmartPointer<vtkPlaneSource>::New();
    planeMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    planeMapper->SetInputConnection(planeSource->GetOutputPort());
    planeActor = vtkSmartPointer<vtkActor>::New();
    planeActor->SetMapper( planeMapper );
    planeActor->GetProperty()->SetColor( 1, 0, 0 );
    transform = vtkSmartPointer<vtkTransform>::New();
    transform->PostMultiply();
    transform->RotateX( 90 );
    planeActor->SetUserTransform( transform );
}

vtkPlaneSource *Plane::getPlaneSource()
{
    return planeSource.Get();
}

void Plane::showFourPinnacles()
{
    planeSource->Update();
    polyData = planeSource->GetOutput();

    qDebug( "polyData->GetNumberOfPoints() is %lld\n" , polyData->GetNumberOfPoints() );
    for( vtkIdType it = 0; it < polyData->GetNumberOfPoints(); ++it )
    {
        double *pos = polyData->GetPoint( it );
        qDebug( "(%lf, %lf, %lf)", pos[0], pos[1], pos[2] );
    }
}
