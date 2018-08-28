#ifndef ADJUST_INTERACTOR_STYLE_H
#define ADJUST_INTERACTOR_STYLE_H

#include <vtkSmartPointer.h>
#include <vtkObjectFactory.h>
#include <vtkInteractorStyleTrackballCamera.h>

#include "Plane.h"
#include "DragCone.h"

class AdjustInteractorStyle: public vtkInteractorStyleTrackballCamera
{
public:
    static AdjustInteractorStyle* New();
    vtkTypeMacro( AdjustInteractorStyle, vtkInteractorStyleTrackballCamera )
    void OnMouseMove() override;
    void OnLeftButtonDown() override;
    void OnRightButtonDown() override;
    void fetchPlane( Plane * _plane );
    void fetchDragCone( DragCone *_dragCone );

protected:
    AdjustInteractorStyle();

    Plane *plane;
    DragCone *dragCone;
    double picked[3];
};

#endif
