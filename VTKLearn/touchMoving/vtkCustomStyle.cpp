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

void vtkCustomStyle::InitPicker(vtkActor *bracketActor)
{
    m_BracketActor = bracketActor;
    m_CellPicker->InitializePickList();
    m_CellPicker->PickFromListOn();
    m_CellPicker->AddPickList( m_BracketActor );
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

        int *eventPos = m_Interator->GetEventPosition();
        int *lastEventPos = m_Interator->GetLastEventPosition();
        m_Renderer->SetDisplayPoint( eventPos[0], eventPos[1], 0 );
        m_Renderer->DisplayToWorld();
        PointStruct eventWPos( m_Renderer->GetWorldPoint() );

        m_Renderer->SetDisplayPoint( lastEventPos[0], lastEventPos[1], 0 );
        m_Renderer->DisplayToWorld();
        PointStruct lastEventWPos( m_Renderer->GetWorldPoint() );

        PointStruct moveVec = eventWPos - lastEventWPos;

        vtkSmartPointer<vtkTransform> trans0 = vtkSmartPointer<vtkTransform>::New();
        trans0->Translate( moveVec.point );
        trans0->Concatenate( m_BracketActor->GetUserTransform() );
        trans0->Update();

        vtkPolyData *toothMesh = (vtkPolyData *)m_ToothActor->GetMapper()->GetInput();
        vtkSmartPointer<vtkPoints> sourcePts = vtkSmartPointer<vtkPoints>::New();
        vtkSmartPointer<vtkPoints> targetPts = vtkSmartPointer<vtkPoints>::New();
        for( int i = 0; i < m_BottomPoints->GetNumberOfPoints(); ++i )
        {
            PointStruct pt;
            m_BottomPoints->GetPoint( i, pt.point );
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
        m_BracketActor->SetUserTransform( finalTrans );
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

vtkCustomStyle::vtkCustomStyle()
{
    m_CellPicker = vtkSmartPointer<vtkCellPicker>::New();
    m_BracketActor = nullptr;
    m_RenderWindow = nullptr;
    m_Renderer = nullptr;
    m_Interator = nullptr;
    m_Picked = false;
    m_OperateOnCube = false;

    m_BottomPoints = vtkSmartPointer<vtkPoints>::New();
    m_BottomPoints->InsertNextPoint( 2.4419, 0.139898, 0.480285  );
    m_BottomPoints->InsertNextPoint( 2.81234, 0.149289, 0.358082 );
    m_BottomPoints->InsertNextPoint( 3.07482, 0.616191, 0.318028 );
    m_BottomPoints->InsertNextPoint( 3.11932, 2.68914, 0.350697  );
    m_BottomPoints->InsertNextPoint( 2.87255, 3.04637, 0.414844  );
    m_BottomPoints->InsertNextPoint( 2.52146, 3.15815, 0.581167 );
    m_BottomPoints->InsertNextPoint( 0.820492, 3.29711, 0.891389 );
    m_BottomPoints->InsertNextPoint( 0.374531, 3.29901, 0.965822 );
    m_BottomPoints->InsertNextPoint( 0.084016, 2.87562, 0.920924 );
    m_BottomPoints->InsertNextPoint( 0.120712, 0.771902, 0.889076 );
    m_BottomPoints->InsertNextPoint( 0.327275, 0.389509, 0.913525 );
    m_BottomPoints->InsertNextPoint( 0.879891, 0.23646, 0.863442 );

    /*double halfLen = 0.5;
    m_BottomPoints->InsertNextPoint( -halfLen, -halfLen, -halfLen );
    m_BottomPoints->InsertNextPoint( halfLen, -halfLen, -halfLen );
    m_BottomPoints->InsertNextPoint( halfLen, halfLen, -halfLen );
    m_BottomPoints->InsertNextPoint( -halfLen, halfLen, -halfLen );*/

    m_OBBTree = vtkSmartPointer<vtkOBBTree>::New();    
    m_ToothActor = nullptr;
}

vtkCustomStyle::~vtkCustomStyle()
{
    m_BracketActor = nullptr;
    m_RenderWindow = nullptr;
    m_Renderer = nullptr;
    m_Interator = nullptr;
    m_ToothActor = nullptr;
}

PointStruct vtkCustomStyle::GetProjectedPtOnSurface(PointStruct pt,
                                                    vtkPolyData *currentCubeMesh)
{
    vtkSmartPointer<vtkPoints> intersectionPts = vtkSmartPointer<vtkPoints>::New();
    vtkSmartPointer<vtkIdList> cellIds = vtkSmartPointer<vtkIdList>::New();
    PointStruct normal( 0, 0, 1 );
    auto *userTrans = m_BracketActor->GetUserTransform();
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

void vtkCustomStyle::ConfigureOBBTree()
{
    m_OBBTree->SetTolerance( 1e-6 );
    m_OBBTree->AutomaticOn();
    m_OBBTree->SetDataSet( m_ToothActor->GetMapper()->GetInput() );
    m_OBBTree->BuildLocator();
}
