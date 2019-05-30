#pragma once

#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>

#include "macroDefinition.h"
#include "FourSidesCone.h"

class WInteractorStyle: public vtkInteractorStyleTrackballCamera
{
public:
    vtkTypeMacro( WInteractorStyle, vtkInteractorStyleTrackballCamera )
    static WInteractorStyle *New();

    void OnMouseMove() override;
    void OnLeftButtonDown() override;
    void OnLeftButtonUp() override;

    CPP_SET_MACRO( m_Render, vtkRenderer* )
    CPP_SET_MACRO( m_Cone, FourSidesCone* )
    CPP_SET_MACRO( m_RenderWindow, vtkRenderWindow* )
protected:
    WInteractorStyle();
    ~WInteractorStyle() override;

    vtkRenderer *m_Render;
    FourSidesCone *m_Cone;
    vtkRenderWindow *m_RenderWindow;
    vtkIdType m_PickId;
private:
    WInteractorStyle(const WInteractorStyle &) = delete;
    void operator = (const WInteractorStyle &) = delete;
};
