#include "vtkCustomStyle.h"
#include "../tool.h"
#include "ULog.h"

#include <string>

void vtkCustomStyle::InitPicker(vtkActor *cubeActor)
{
    m_CubeActor = cubeActor;
    m_CellPicker->InitializePickList();
    m_CellPicker->PickFromListOn();
    m_CellPicker->AddPickList( m_CubeActor );
}

void vtkCustomStyle::OnLeftButtonDown()
{
    int *eventPos = m_Interator->GetEventPosition();
    int pickResult = m_CellPicker->Pick( eventPos[0], eventPos[1], 0, m_Renderer );
    if( pickResult )
    {
        m_Picked = true;
    }
    else
    {
        m_Picked = false;
    }
    vtkInteractorStyleTrackballCamera::OnLeftButtonDown();
}

void vtkCustomStyle::OnLeftButtonUp()
{
    m_Picked = false;
    vtkInteractorStyleTrackballCamera::OnLeftButtonUp();
}

void vtkCustomStyle::OnMouseMove()
{
    if( m_Picked && m_OperateOnCube )
    {
        Log( IInfo, "works" );
    }
    else
    {
        vtkInteractorStyleTrackballCamera::OnMouseMove();
    }
}

void vtkCustomStyle::OnKeyDown()
{
    std::string key = m_Interator->GetKeySym();
    if( key == "Z" || key == "z" )
    {
        m_OperateOnCube = true;
    }
}

void vtkCustomStyle::OnKeyUp()
{
    m_OperateOnCube = false;
}

vtkCustomStyle::vtkCustomStyle()
{
    m_CellPicker = vtkSmartPointer<vtkCellPicker>::New();
    m_CubeActor = nullptr;
    m_RenderWindow = nullptr;
    m_Renderer = nullptr;
    m_Interator = nullptr;
    m_Picked = false;
    m_OperateOnCube = false;
}

vtkCustomStyle::~vtkCustomStyle()
{

}
