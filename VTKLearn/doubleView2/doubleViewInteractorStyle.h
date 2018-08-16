#ifndef DOUBLE_VIEW_INTERACTOR_STYLE_H
#define DOUBLE_VIEW_INTERACTOR_STYLE_H

#include <vtkSmartPointer.h>
#include <vtkInteractorStyleTrackballCamera.h>

class DoubleViewInteractorStyle: public vtkInteractorStyleTrackballCamera
{
public:
    static DoubleViewInteractorStyle* New();
    vtkTypeMacro( DoubleViewInteractorStyle, vtkInteractorStyleTrackballCamera )
    void fetchStyle( DoubleViewInteractorStyle *_theOtherStyle );
    void OnMouseMove() override;
    void Rotate() override;
    void Spin() override;
    void Dolly() override;
    void FatherDolly( double factor );
    void Pan() override;
private:
    DoubleViewInteractorStyle();
    DoubleViewInteractorStyle *theOtherStyle;
};

#endif
