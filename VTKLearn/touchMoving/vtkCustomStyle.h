#pragma once

#include <iostream>
#include <vtkSmartPointer.h>
#include <vtkCellPicker.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkActor.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkPoints.h>
#include <vtkOBBTree.h>

#include "../tool.h"

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

    void InitPicker( vtkActor *bracketActor );
    void OnLeftButtonDown() override;
    void OnLeftButtonUp() override;
    void OnMouseMove() override;
    virtual void OnKeyDown() override;
    virtual void OnKeyUp() override;

    CPP_SET_MACRO( m_RenderWindow, vtkRenderWindow * )
    CPP_SET_MACRO( m_Renderer, vtkRenderer * )
    CPP_SET_MACRO( m_Interator, vtkRenderWindowInteractor * )
    CPP_SET_MACRO( m_ToothActor, vtkActor *)
    void ConfigureOBBTree();

protected:
    vtkCustomStyle();
    ~vtkCustomStyle() override;
    PointStruct GetProjectedPtOnSurface( PointStruct pt, vtkPolyData* currentCubeMesh );

    vtkSmartPointer<vtkCellPicker> m_CellPicker;
    vtkActor *m_BracketActor;
    vtkRenderWindow *m_RenderWindow;
    vtkRenderer *m_Renderer;
    vtkRenderWindowInteractor *m_Interator;
    vtkSmartPointer<vtkPoints> m_BottomPoints;
    bool m_Picked;
    bool m_OperateOnCube;
    vtkSmartPointer<vtkOBBTree> m_OBBTree;
    vtkActor *m_ToothActor;
};
