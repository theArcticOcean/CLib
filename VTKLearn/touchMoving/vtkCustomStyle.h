#pragma once

#include <iostream>
#include <vtkSmartPointer.h>
#include <vtkCellPicker.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkActor.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>

#define CPP_SET_MACRO(name,type) \
  void Set##name(type _arg) \
  { \
    if (this->name != _arg) \
    { \
    this->name = _arg; \
    } \
  }

class vtkCustomStyle: public vtkInteractorStyleTrackballCamera
{
public:
    static vtkCustomStyle *New(){ return new vtkCustomStyle(); }

    void InitPicker( vtkActor *cubeActor );
    void OnLeftButtonDown() override;
    void OnLeftButtonUp() override;
    void OnMouseMove() override;
    virtual void OnKeyDown();
    virtual void OnKeyUp();

    CPP_SET_MACRO( m_RenderWindow, vtkRenderWindow * )
    CPP_SET_MACRO( m_Renderer, vtkRenderer * )
    CPP_SET_MACRO( m_Interator, vtkRenderWindowInteractor * )
protected:
    vtkCustomStyle();
    ~vtkCustomStyle();

    vtkSmartPointer<vtkCellPicker> m_CellPicker;
    vtkActor *m_CubeActor;
    vtkRenderWindow *m_RenderWindow;
    vtkRenderer *m_Renderer;
    vtkRenderWindowInteractor *m_Interator;
    bool m_Picked;
    bool m_OperateOnCube;
};
