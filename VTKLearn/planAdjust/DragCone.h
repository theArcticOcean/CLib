#ifndef DRAG_CONE_H
#define DRAG_CONE_H

#include <vtkConeSource.h>
#include <vtkRenderWindow.h>

#include "Plane.h"

class DragCone
{
public:
    DragCone();
    void fetchPlane(Plane *_plane);
    void setPosition();

    vtkSmartPointer<vtkActor> dragConeActor;
    vtkRenderWindow *renderWindow;
private:
    vtkSmartPointer<vtkConeSource> dragConeSource;
    vtkSmartPointer<vtkPolyDataMapper> dragConeSourceMapper;
    vtkSmartPointer<vtkTransform> dragConeTransform;
    Plane *plane;
};

#endif
