#pragma once

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
#include "tool.h"

#define vtkPtr( var, className ) vtkSmartPointer<className> var = \
    vtkSmartPointer<className>::New()

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
    enum CurrentViewDir{ ZPositive, YPositive, ZNegative, YNegative, XPositive, XNegative };
    static vtkCustomStyle *New(){ return new vtkCustomStyle(); }
    void OnLeftButtonDown() override;
    void OnLeftButtonUp() override;
    void OnRightButtonDown() override;
    void OnRightButtonUp() override;
    virtual void OnKeyPress();
    virtual void OnMouseMove();
    void RotateOnZRing(int *lastPos, int *pos);
    void RotateOnXRing(int *lastPos, int *pos);
    void RotateOnYRing(int *lastPos, int *pos);
    PointStruct GetViewDirect();
    CurrentViewDir CalViewDirection();

    CPP_SET_MACRO( m_ConeActor, vtkActor* )
    CPP_SET_MACRO( m_Axes, vtkAxesActor* )
    CPP_SET_MACRO( m_CellPicker, vtkCellPicker* )
    CPP_SET_MACRO( m_Render, vtkRenderer* )
    CPP_SET_MACRO( m_ZRingActor, vtkActor* )
    CPP_SET_MACRO( m_XRingActor, vtkActor* )
    CPP_SET_MACRO( m_YRingActor, vtkActor* )
    CPP_SET_MACRO( m_Interator, vtkRenderWindowInteractor* )
    CPP_SET_MACRO( m_RenderWindow, vtkRenderWindow *)

protected:
    vtkCustomStyle();
    ~vtkCustomStyle();

    vtkActor *m_ConeActor;
    vtkAxesActor *m_Axes;
    vtkCellPicker *m_CellPicker;
    vtkRenderer *m_Render;
    vtkActor *m_ZRingActor;
    vtkActor *m_XRingActor;
    vtkActor *m_YRingActor;
    vtkRenderWindowInteractor *m_Interator;
    vtkRenderWindow *m_RenderWindow;

    bool m_LeftButtonPressed;
    bool m_RightButtonPressed;
    bool m_InteractorBanned;
};
