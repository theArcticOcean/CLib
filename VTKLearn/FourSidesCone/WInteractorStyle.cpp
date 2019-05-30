#include "WInteractorStyle.h"
#include "VertexActor.h"

#include <vtkObjectFactory.h>
#include <vtkRenderWindowInteractor.h>

vtkStandardNewMacro(WInteractorStyle);

WInteractorStyle::WInteractorStyle()
    : m_Render( nullptr )
    , m_Cone( nullptr )
    , m_RenderWindow( nullptr )
    , m_PickId( -1 )
{

}

WInteractorStyle::~WInteractorStyle()
{

}

void WInteractorStyle::OnMouseMove()
{
    if( m_PickId > -1 )
    {
        int *pickPos = this->Interactor->GetEventPosition();
        int *lastEventPos = this->Interactor->GetLastEventPosition();
        vtkActor *vertexActor = m_Cone->FetchVertexActor( m_PickId );
        PointStruct pickDisplayPos( pickPos[0], pickPos[1], 0 ), pickWorldPos;
        PointStruct lastDisplayPos( lastEventPos[0], lastEventPos[1], 0 ), lastWorldPos;
        PointStruct moveVector = pickDisplayPos - lastDisplayPos;
        PointStruct centerPos( vertexActor->GetCenter() );
        /*cout << "pickDisplayPos: " << pickDisplayPos;
        cout << "lastDisplayPos: " << lastDisplayPos;
        cout << "moveVector: " << moveVector;
        cout << "centerPos: " << centerPos;
        cout << "----------------------------\n"; */

        m_Render->SetDisplayPoint( pickDisplayPos.point );
        m_Render->DisplayToWorld();
        m_Render->GetWorldPoint( pickWorldPos.point );

        m_Render->SetDisplayPoint( lastDisplayPos.point );
        m_Render->DisplayToWorld();
        m_Render->GetWorldPoint( lastWorldPos.point );

        moveVector = pickWorldPos - lastWorldPos;
        vertexActor->AddPosition( moveVector.point );
        m_Render->RemoveActor( m_Cone->Getm_ConeActor() );
        m_Cone->UpdateConeShape();
        m_Render->AddActor( m_Cone->Getm_ConeActor() );
        m_Render->Render();
        m_RenderWindow->Render();
    }
    else
    {
        vtkInteractorStyleTrackballCamera::OnMouseMove();
    }
}

void WInteractorStyle::OnLeftButtonDown()
{
    int *pickPos = this->Interactor->GetEventPosition();
    PointStruct displayPos( pickPos[0], pickPos[1], pickPos[2] );
    PointStruct worldPos;
    //printf( "pickPos: (%d, %d, %d)\n", pickPos[0], pickPos[1], pickPos[2] );
    m_Render->SetDisplayPoint( displayPos.point );
    m_Render->DisplayToWorld();
    m_Render->GetWorldPoint( worldPos.point );
    //printf( "pickPos: (%d, %d, %d)\n", worldPos[0], worldPos[1], worldPos[2] );

    m_Cone->Getm_VertexPicker()->Pick( pickPos[0], pickPos[1], 0, m_Render );
    vtkIdType pickPointId = m_Cone->Getm_VertexPicker()->GetPointId();
    if( pickPointId > -1 )
    {
        VertexActor *pickedActor =
                dynamic_cast<VertexActor*>( m_Cone->Getm_VertexPicker()->GetProp3D() );
        m_PickId = pickedActor->Getm_ActorId();
        //printf( "pickId: %d\n", m_PickId );
    }
    vtkInteractorStyleTrackballCamera::OnLeftButtonDown();
}

void WInteractorStyle::OnLeftButtonUp()
{
    m_PickId = -1;
    vtkInteractorStyleTrackballCamera::OnLeftButtonUp();
}
