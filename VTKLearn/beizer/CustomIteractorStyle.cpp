#include "CustomIteractorStyle.h"
#include "point.hpp"
#include "UPaintBrush.h"

#include <vtkCamera.h>

void CustomIteractorStyle::OnMouseMove()
{
    if( m_Brush )
    {
        int *eventPos = GetInteractor()->GetEventPosition();
        m_Brush->UpdateItem( eventPos[0], eventPos[1] );
        m_Renderer->GetRenderWindow()->Render();

        double pos[3] = { 0.5, 0, 0 };
        m_Renderer->SetWorldPoint( pos[0], pos[1], pos[2], 0 );
        m_Renderer->WorldToDisplay();
        double *displayPos = m_Renderer->GetDisplayPoint();
        m_Brush->CheckPtInsideCircle( displayPos[0], displayPos[1] );
    }
}

void CustomIteractorStyle::SetRenderer(vtkSmartPointer<vtkRenderer> renderer)
{
    m_Renderer = renderer;
    if( m_Brush ) m_Brush->UpdateRenderer( renderer );
}

CustomIteractorStyle::CustomIteractorStyle()
{
    m_Brush = new UPaintBrush();
    m_Renderer = nullptr;
}

CustomIteractorStyle::~CustomIteractorStyle()
{
    delete m_Brush;
    m_Brush = nullptr;
}
