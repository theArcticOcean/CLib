#include "UPaintBrush.h"
#include "ULog.h"
#include "point.hpp"

#include <vtkPen.h>
#include <vtkTransform2D.h>
#include <vtkMath.h>
#include <vtkContextScene.h>
#include <vtkContextDevice2D.h>

vtkStandardNewMacro(BrushItem);

BrushItem::BrushItem()
    : m_BrushPts( vtkSmartPointer<vtkPoints2D>::New() )
{
    m_MousePos[0] = m_MousePos[1] = 10;
}

// draw circle, the following PushMatrix and PopMatrix is used to avoid affecting other drawing.
// So you have to update m_MousePos before this interface if you want to make circle to move with your mouse.
bool BrushItem::Paint(vtkContext2D *painter)
{
    painter->GetPen()->SetColor(255, 0, 0);
    painter->GetPen()->SetWidth( 3 );

    painter->PushMatrix();
    vtkNew<vtkTransform2D> tran;
    tran->Translate(m_MousePos[0], m_MousePos[1]);
    painter->AppendTransform(tran);
    painter->DrawPoly( m_BrushPts );
    painter->PopMatrix();

    return true;
}

void BrushItem::SetMousePos(double x, double y)
{
    m_MousePos[0] = x;
    m_MousePos[1] = y;
}

void BrushItem::SetRadius(double radius)
{
    unsigned int n = 360;
    m_BrushPts->Allocate(n+1);
    // draw a circle with radius and center (0, 0).
    for(int i = 0; i <= n; i++)
    {
        double alpha = i * 2 * vtkMath::Pi() / n;
        double x = cos(alpha)*radius, y = sin(alpha)*radius;
        m_BrushPts->InsertNextPoint(x, y);
    }
}

vtkSmartPointer<vtkPoints2D> BrushItem::GetCirclePts()
{
    vtkSmartPointer<vtkPoints2D> resultPts = vtkSmartPointer<vtkPoints2D>::New();
    vtkNew<vtkTransform2D> tran;
    tran->Translate(m_MousePos[0], m_MousePos[1]);
    tran->TransformPoints(m_BrushPts, resultPts);
    return resultPts;
}

UPaintBrush::UPaintBrush()
    : m_BrushActor( vtkSmartPointer<vtkContextActor>::New() )
    , m_CurRenderer( nullptr )
    , m_CriclePts( nullptr )
{

}

void UPaintBrush::UpdateRenderer(vtkSmartPointer<vtkRenderer> renderer)
{
    if( m_CurRenderer != nullptr )
    {
        if( m_CurRenderer != renderer )
        {
            m_CurRenderer->RemoveActor( m_BrushActor );
        }
        else {
            return;
        }
    }

    m_CurRenderer = renderer;
    if( m_CurRenderer )
    {
        m_CurRenderer->AddActor( m_BrushActor );
    }

    //Log( IInfo, "m_CurRenderer: ", m_CurRenderer );
}

void UPaintBrush::UpdateItem(double x, double y)
{
    m_BrushActor->GetScene()->ClearItems();
    vtkSmartPointer<BrushItem> brushItem = vtkSmartPointer<BrushItem>::New();
    brushItem->SetRadius( m_BrushSettings.radius );
    brushItem->SetMousePos( x, y );
    brushItem->Update();
    m_BrushActor->GetScene()->AddItem( brushItem );
    m_CriclePts = brushItem->GetCirclePts();
    //Log( IInfo, "finished! " );
}

void UPaintBrush::CheckPtInsideCircle(double x, double y)
{    
    Point pt( x, y, 0 );
    bool sameDir = true;
    for( int i = 0; i < m_CriclePts->GetNumberOfPoints()-2;++i )
    {
        Point pt0( m_CriclePts->GetPoint( i ) );
        Point pt1( m_CriclePts->GetPoint( i+1 ) );
        Point pt2( m_CriclePts->GetPoint( i+2 ) );
        auto dir0 = (pt0 - pt)^(pt1 - pt);
        auto dir1 = (pt1 - pt)^(pt2 - pt);
        if( dir0.Dot( dir1 ) < 0 )
        {
            sameDir = false;
            break;
        }
    }
    Log( IInfo, "sameDir: ", sameDir );
}
