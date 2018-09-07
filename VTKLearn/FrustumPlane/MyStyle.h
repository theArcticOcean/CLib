#include <vtkInteractorStyleTrackballCamera.h>

class MyStyle : public vtkInteractorStyleTrackballCamera
{
public:
    static MyStyle* New();

    virtual void OnLeftButtonDown();
};
