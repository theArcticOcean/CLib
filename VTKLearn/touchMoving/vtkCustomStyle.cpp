#include "vtkCustomStyle.h"
#include "../tool.h"
#include "ULog.h"

#include <string>
#include <vtkTransform.h>
#include <vtkTransformFilter.h>
#include <vtkMapper.h>
#include <vtkDataSet.h>
#include <vtkPointSet.h>
#include <vtkLandmarkTransform.h>
#include <vtkIdList.h>
#include <vtkPolyData.h>

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
        //Log( IInfo, "works" );

        int *eventPos = m_Interator->GetEventPosition();
        int *lastEventPos = m_Interator->GetLastEventPosition();
        m_Renderer->SetDisplayPoint( eventPos[0], eventPos[1], 0 );
        m_Renderer->DisplayToWorld();
        double eventWorldPt[4];
        m_Renderer->GetWorldPoint( eventWorldPt );
        PointStruct eventWPos( eventWorldPt[0], eventWorldPt[1], eventWorldPt[2] );

        m_Renderer->SetDisplayPoint( lastEventPos[0], lastEventPos[1], 0 );
        m_Renderer->DisplayToWorld();
        double lastEventWorldPt[4];
        m_Renderer->GetWorldPoint( lastEventWorldPt );
        PointStruct lastEventWPos( lastEventWorldPt[0], lastEventWorldPt[1], lastEventWorldPt[2] );

        PointStruct moveVec = eventWPos - lastEventWPos;

        vtkSmartPointer<vtkTransform> trans0 = vtkSmartPointer<vtkTransform>::New();
        trans0->Translate( moveVec.point );
        trans0->Update();
        trans0->Concatenate( m_CubeActor->GetUserTransform() );
        trans0->Update();

        m_CubeActor->SetUserTransform( trans0 );

        vtkPolyData *toothMesh = (vtkPolyData *)m_QuadricActor->GetMapper()->GetInput();
        vtkSmartPointer<vtkPoints> sourcePts = vtkSmartPointer<vtkPoints>::New();
        vtkSmartPointer<vtkPoints> targetPts = vtkSmartPointer<vtkPoints>::New();
        for( int i = 0; i < m_BottomPoints->GetNumberOfPoints(); ++i )
        {
            PointStruct pt;
            m_BottomPoints->GetPoint( i, pt.point );
            //m_CubeActor->GetUserTransform()->TransformPoint( pt.point, pt.point );
            trans0->TransformPoint( pt.point, pt.point );
            PointStruct ptOnSurface = GetProjectedPtOnSurface( pt, toothMesh );
            sourcePts->InsertNextPoint( pt.point );
            targetPts->InsertNextPoint( ptOnSurface.point );
        }

        vtkSmartPointer<vtkLandmarkTransform> landmarkTrans = vtkSmartPointer<vtkLandmarkTransform>::New();
        landmarkTrans->SetSourceLandmarks( sourcePts );
        landmarkTrans->SetTargetLandmarks( targetPts );
        landmarkTrans->SetModeToRigidBody();
        landmarkTrans->Update();

        vtkSmartPointer<vtkTransform> finalTrans = vtkSmartPointer<vtkTransform>::New();
        finalTrans->Concatenate( landmarkTrans );
        finalTrans->Concatenate( trans0 );
        finalTrans->Update();
        m_CubeActor->SetUserTransform( finalTrans );
        m_RenderWindow->Render();

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

void vtkCustomStyle::ConfigureOBBTree()
{
    m_OBBTree->SetTolerance( 1e-6 );
    m_OBBTree->AutomaticOn();
    m_OBBTree->SetDataSet( m_QuadricActor->GetMapper()->GetInput() );
    m_OBBTree->BuildLocator();
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

    double halfLen = 0.5;
    m_BottomPoints = vtkSmartPointer<vtkPoints>::New();
    m_BottomPoints->InsertNextPoint( -halfLen, -halfLen, -halfLen );
    m_BottomPoints->InsertNextPoint( halfLen, -halfLen, -halfLen );
    m_BottomPoints->InsertNextPoint( halfLen, halfLen, -halfLen );
    m_BottomPoints->InsertNextPoint( -halfLen, halfLen, -halfLen );

    m_OBBTree = vtkSmartPointer<vtkOBBTree>::New();
}

vtkCustomStyle::~vtkCustomStyle()
{

}

PointStruct vtkCustomStyle::GetProjectedPtOnSurface(PointStruct pt, vtkPolyData *currentCubeMesh)
{
    vtkSmartPointer<vtkPoints> intersectionPts = vtkSmartPointer<vtkPoints>::New();
    vtkSmartPointer<vtkIdList> cellIds = vtkSmartPointer<vtkIdList>::New();
    PointStruct normal( 0, 0, 1 );
    auto *userTrans = m_CubeActor->GetUserTransform();
    userTrans->TransformVector( normal.point, normal.point );
    normal.Unit();
    m_OBBTree->IntersectWithLine( (pt - normal*10).point, (pt + normal*10).point, intersectionPts, cellIds );
    double minDis = VTK_INT_MAX;
    PointStruct result = pt;
    for( int i = 0; i < intersectionPts->GetNumberOfPoints(); ++i )
    {
        PointStruct tmpPt;
        intersectionPts->GetPoint( i, tmpPt.point );
        double dis = (pt - tmpPt).Length();
        if( minDis > dis )
        {
            result = tmpPt;
            minDis = dis;
        }
    }

    if( minDis > VTK_INT_MAX - 1 )
    {
        int ptId = currentCubeMesh->FindPoint( pt.point );
        result = PointStruct( currentCubeMesh->GetPoint( ptId ) );
        Log( IInfo, "use closed point" );
    }
    return result;
}
