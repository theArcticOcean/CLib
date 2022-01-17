#include "vtkCustomStyle.h"

void vtkCustomStyle::OnLeftButtonDown()
{
    vtkInteractorStyleTrackballCamera::OnLeftButtonDown();
    m_LeftButtonPressed = true;
}

void vtkCustomStyle::OnLeftButtonUp()
{
    vtkInteractorStyleTrackballCamera::OnLeftButtonUp();
    m_LeftButtonPressed = false;
    m_XRingActor->VisibilityOn();
    m_YRingActor->VisibilityOn();
    m_ZRingActor->VisibilityOn();
    m_RenderWindow->Render();
}

void vtkCustomStyle::OnRightButtonDown()
{
    vtkInteractorStyleTrackballCamera::OnRightButtonDown();
    m_RightButtonPressed = true;
}

void vtkCustomStyle::OnRightButtonUp()
{
    vtkInteractorStyleTrackballCamera::OnRightButtonUp();
    m_RightButtonPressed = false;
}

void vtkCustomStyle::OnKeyPress()
{
    std::string keyName( m_Interator->GetKeySym() );
    std::cout << keyName;
    if( "i" == keyName )
    {
        m_InteractorBanned = true;
    }
    else if( "I" == keyName )
    {
        m_InteractorBanned = false;
    }

    vtkInteractorStyleTrackballCamera::OnKeyPress();
}

void vtkCustomStyle::OnMouseMove()
{
    int pos[2], lastPos[2];
    m_Interator->GetLastEventPosition( lastPos );
    m_Interator->GetEventPosition( pos );
    m_CellPicker->Pick( pos[0], pos[1], 0, m_Render );
    if( m_CellPicker->GetCellId() >= 0 && m_LeftButtonPressed )
    {
        if( m_ZRingActor == m_CellPicker->GetProp3D() )
        {
            RotateOnZRing( lastPos, pos );
            m_ZRingActor->VisibilityOn();
            m_XRingActor->VisibilityOff();
            m_YRingActor->VisibilityOff();
        }
        if( m_XRingActor == m_CellPicker->GetProp3D() )
        {
            RotateOnXRing( lastPos, pos );
            m_XRingActor->VisibilityOn();
            m_ZRingActor->VisibilityOff();
            m_YRingActor->VisibilityOff();
        }
        if( m_YRingActor == m_CellPicker->GetProp3D() )
        {
            RotateOnYRing( lastPos, pos );
            m_YRingActor->VisibilityOn();
            m_ZRingActor->VisibilityOff();
            m_XRingActor->VisibilityOff();
        }
        m_RenderWindow->Render();
    }
    else
    {
        if( !m_InteractorBanned )
        {
            vtkInteractorStyleTrackballCamera::OnMouseMove();
        }
    }
}

void vtkCustomStyle::RotateOnZRing(int *lastPos, int *pos)
{
    vtkPtr( transform, vtkTransform );
    transform->SetMatrix( m_ConeActor->GetMatrix() );
    PointStruct ZAxis( transform->TransformDoubleVector( 0, 0, 1 ) );
    PointStruct origin( m_ConeActor->GetCenter() ); //( transform->TransformDoublePoint( 0, 0, 0 ) );
    m_Render->SetWorldPoint( origin.point );
    m_Render->WorldToDisplay();
    PointStruct originDisplay( m_Render->GetDisplayPoint() );
    PointStruct lastPosStruct( lastPos[0], lastPos[1], 0 );
    PointStruct posStruct( pos[0], pos[1], 0 );
    PointStruct vec1 = lastPosStruct - originDisplay;
    PointStruct vec2 = posStruct - originDisplay;
    PointStruct crossPt = vec1 ^ vec2;
    double radianDelta = vtkMath::AngleBetweenVectors( vec1.point, vec2.point );
    double degreeDelta = 180.0 / vtkMath::Pi() * radianDelta;
    cout << "RotateOnZRing degreeDelta: " << degreeDelta << endl;

    PointStruct viewDirection = GetViewDirect();
    double radianDeltaBetweenViewAndZAixs =
            vtkMath::AngleBetweenVectors( ZAxis.point, viewDirection.point );
    double degreeDeltaBetweenViewAndZAixs =
            180.0 / vtkMath::Pi() * radianDeltaBetweenViewAndZAixs;
    cout << "degreeDeltaBetweenViewAndZAixs: " << fabs( degreeDeltaBetweenViewAndZAixs ) << endl;
    // Rotating axis
    if( crossPt[2] < 0 )
    {
        degreeDelta = -degreeDelta;
    }
    // View direct vector is in different direct comparing with rotating axis
    if( degreeDeltaBetweenViewAndZAixs > 90 )
    {
        degreeDelta = -degreeDelta;
    }

    //m_ConeActor->RotateWXYZ( degreeDelta, crossPt.point[0], crossPt.point[1], crossPt.point[2] );
    transform->SetMatrix( m_ZRingActor->GetMatrix() );
    PointStruct rotateAxis( transform->TransformDoubleVector( 0, 0, 1 ) );
    m_ConeActor->RotateZ( degreeDelta );
    cout << "Changing, degreeDelta: " << degreeDelta << ", " << rotateAxis;
}

void vtkCustomStyle::RotateOnXRing(int *lastPos, int *pos)
{
    vtkPtr( transform, vtkTransform );
    transform->SetMatrix( m_ConeActor->GetMatrix() );
    PointStruct XAxis( transform->TransformDoubleVector( 1, 0, 0 ) );
    PointStruct origin( m_ConeActor->GetCenter() ); //( transform->TransformDoublePoint( 0, 0, 0 ) );
    m_Render->SetWorldPoint( origin.point );
    m_Render->WorldToDisplay();
    PointStruct originDisplay( m_Render->GetDisplayPoint() );
    PointStruct lastPosStruct( lastPos[0], lastPos[1], 0 );
    PointStruct posStruct( pos[0], pos[1], 0 );
    PointStruct vec1 = lastPosStruct - originDisplay;
    PointStruct vec2 = posStruct - originDisplay;
    PointStruct crossPt = vec1 ^ vec2;
    double radianDelta = vtkMath::AngleBetweenVectors( vec1.point, vec2.point );
    double degreeDelta = 180.0 / vtkMath::Pi() * radianDelta;
    cout << "RotateOnXRing degreeDelta: " << degreeDelta << endl;

    PointStruct viewDirection = GetViewDirect();
    double radianDeltaBetweenViewAndXAixs =
            vtkMath::AngleBetweenVectors( XAxis.point, viewDirection.point );
    double degreeDeltaBetweenViewAndXAixs =
            180.0 / vtkMath::Pi() * radianDeltaBetweenViewAndXAixs;
    cout << "degreeDeltaBetweenViewAndXAixs: " << fabs( degreeDeltaBetweenViewAndXAixs ) << endl;
    // Rotating axis
    if( crossPt[2] < 0 )
    {
        degreeDelta = -degreeDelta;
    }
    // View direct vector is in different direct comparing with rotating axis
    if( degreeDeltaBetweenViewAndXAixs > 90 )
    {
        degreeDelta = -degreeDelta;
    }

    m_ConeActor->RotateX( degreeDelta );
}

void vtkCustomStyle::RotateOnYRing(int *lastPos, int *pos)
{
    vtkPtr( transform, vtkTransform );
    transform->SetMatrix( m_ConeActor->GetMatrix() );
    PointStruct YAxis( transform->TransformDoubleVector( 0, 1, 0 ) );
    PointStruct origin( m_ConeActor->GetCenter() ); //( transform->TransformDoublePoint( 0, 0, 0 ) );
    m_Render->SetWorldPoint( origin.point );
    m_Render->WorldToDisplay();
    PointStruct originDisplay( m_Render->GetDisplayPoint() );
    PointStruct lastPosStruct( lastPos[0], lastPos[1], 0 );
    PointStruct posStruct( pos[0], pos[1], 0 );
    PointStruct vec1 = lastPosStruct - originDisplay;
    PointStruct vec2 = posStruct - originDisplay;
    PointStruct crossPt = vec1 ^ vec2;
    double radianDelta = vtkMath::AngleBetweenVectors( vec1.point, vec2.point );
    double degreeDelta = 180.0 / vtkMath::Pi() * radianDelta;
    cout << "RotateOnYRing degreeDelta: " << degreeDelta << endl;

    PointStruct viewDirection = GetViewDirect();
    double radianDeltaBetweenViewAndYAixs =
            vtkMath::AngleBetweenVectors( YAxis.point, viewDirection.point );
    double degreeDeltaBetweenViewAndYAixs =
            180.0 / vtkMath::Pi() * radianDeltaBetweenViewAndYAixs;
    cout << "degreeDeltaBetweenViewAndYAixs: " << fabs( degreeDeltaBetweenViewAndYAixs ) << endl;
    // Rotating axis
    if( crossPt[2] < 0 )
    {
        degreeDelta = -degreeDelta;
    }
    // View direct vector is in different direct comparing with rotating axis
    if( degreeDeltaBetweenViewAndYAixs > 90 )
    {
        degreeDelta = -degreeDelta;
    }

    m_ConeActor->RotateY( degreeDelta );
}

PointStruct vtkCustomStyle::GetViewDirect()
{
    vtkCamera *curCamera = m_Render->GetActiveCamera();
    double curFocalPt[3], curCamPos[3], curViewUp[3];
    curCamera->GetFocalPoint(curFocalPt);
    curCamera->GetPosition(curCamPos);
    curCamera->GetViewUp(curViewUp);

    double curViewDirection[3];
    vtkMath::Subtract(curCamPos, curFocalPt, curViewDirection);
    PointStruct viewDirection(curViewDirection[0], curViewDirection[1], curViewDirection[2]);

    return viewDirection;
}

vtkCustomStyle::CurrentViewDir vtkCustomStyle::CalViewDirection()
{
    PointStruct viewDirection = GetViewDirect();
    viewDirection.Unit();

    PointStruct x, z, y;
    PointStruct viewDir[6];
    CurrentViewDir resultDir = YPositive;
    x = PointStruct( 1, 0, 0 );
    y = PointStruct( 0, 1, 0 );
    z = PointStruct( 0, 0, 1 );

    viewDir[0] = z;
    viewDir[1] = y;
    viewDir[2] = -z;
    viewDir[3] = -y;
    viewDir[4] = x;
    viewDir[5] = -x;

    double maxDotValue = -1 * 1000;
    for( int i = 0; i < 6; ++i )
    {
        double viewUpDotValue = vtkMath::Dot( viewDir[i].point, viewDirection.point );
        viewUpDotValue = viewUpDotValue * 1000;
        if( maxDotValue < viewUpDotValue )  // absolutely, resultDir will be assgined.
        {
            resultDir = static_cast<CurrentViewDir>( i );
            maxDotValue = viewUpDotValue;
        }
    }
    return resultDir;
}

vtkCustomStyle::vtkCustomStyle()
    : m_ConeActor( nullptr )
    , m_Axes( nullptr )
    , m_CellPicker( nullptr )
    , m_Render( nullptr )
    , m_ZRingActor( nullptr )
    , m_XRingActor( nullptr )
    , m_YRingActor( nullptr )
    , m_Interator( nullptr )
    , m_RenderWindow( nullptr )
{
    m_LeftButtonPressed = false;
    m_RightButtonPressed = false;
    m_InteractorBanned = true;
}

vtkCustomStyle::~vtkCustomStyle()
{

}
