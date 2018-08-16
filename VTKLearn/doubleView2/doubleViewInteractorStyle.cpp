#include "doubleViewInteractorStyle.h"

#include <QDebug>
#include <vtkObjectFactory.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkCamera.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>

vtkStandardNewMacro( DoubleViewInteractorStyle )

void DoubleViewInteractorStyle::fetchStyle(DoubleViewInteractorStyle *_theOtherStyle)
{
    theOtherStyle = _theOtherStyle;
}

void DoubleViewInteractorStyle::OnMouseMove()
{
    int x = this->Interactor->GetEventPosition()[0];
    int y = this->Interactor->GetEventPosition()[1];
    qInfo() << "this State is " << this->State;

    switch (this->State)
    {
      case VTKIS_ROTATE:
        this->FindPokedRenderer(x, y);
        this->Rotate();
        this->InvokeEvent(vtkCommand::InteractionEvent, nullptr);
        break;

      case VTKIS_PAN:
        this->FindPokedRenderer(x, y);
        this->Pan();
        this->InvokeEvent(vtkCommand::InteractionEvent, nullptr);
        break;

      case VTKIS_DOLLY:
        this->FindPokedRenderer(x, y);
        this->Dolly();
        this->InvokeEvent(vtkCommand::InteractionEvent, nullptr);
        break;

      case VTKIS_SPIN:
        this->FindPokedRenderer(x, y);
        this->Spin();
        this->InvokeEvent(vtkCommand::InteractionEvent, nullptr);
        break;
    }
}

void DoubleViewInteractorStyle::Rotate()
{
    qInfo() << "Rotate start!";
    if ( this->CurrentRenderer == nullptr ||
         nullptr == theOtherStyle->CurrentRenderer )
    {
        return;
    }

    vtkRenderWindowInteractor *rwi1 = this->Interactor;
    vtkRenderWindowInteractor *rwi2 = theOtherStyle->Interactor;

    int dx = rwi1->GetEventPosition()[0] - rwi1->GetLastEventPosition()[0];
    int dy = rwi1->GetEventPosition()[1] - rwi1->GetLastEventPosition()[1];
    //qDebug( "( %d, %d )", rwi1->GetEventPosition()[0],  rwi1->GetEventPosition()[1] );

    int *size = this->CurrentRenderer->GetRenderWindow()->GetSize();

    double delta_elevation = -20.0 / size[1];
    double delta_azimuth = -20.0 / size[0];

    double rxf = dx * delta_azimuth * this->MotionFactor;
    double ryf = dy * delta_elevation * this->MotionFactor;

    vtkCamera *camera1 = this->CurrentRenderer->GetActiveCamera();
    if( NULL == camera1 )
    {
        qDebug() << "camera1 is NULL";
        return ;
    }
    camera1->Azimuth(rxf);
    camera1->Elevation(ryf);
    camera1->OrthogonalizeViewUp();

//    qDebug( "theOtherStyle is %p, theOtherStyle->CurrentRenderer is %p",
//            theOtherStyle,
//            theOtherStyle->CurrentRenderer);

    vtkCamera *camera2 = theOtherStyle->CurrentRenderer->GetActiveCamera();
    if( NULL == camera2 )
    {
        qDebug() << "camera2 is NULL";
        return ;
    }
    camera2->Azimuth(rxf);
    camera2->Elevation(ryf);
    camera2->OrthogonalizeViewUp();

    if ( this->AutoAdjustCameraClippingRange )
    {
        this->CurrentRenderer->ResetCameraClippingRange();
    }

    if( theOtherStyle->AutoAdjustCameraClippingRange )
    {
        theOtherStyle->CurrentRenderer->ResetCameraClippingRange();
    }

    if ( rwi1->GetLightFollowCamera() )
    {
        this->CurrentRenderer->UpdateLightsGeometryToFollowCamera();
    }
    rwi1->Render();

    if ( rwi2->GetLightFollowCamera() )
    {
        theOtherStyle->CurrentRenderer->UpdateLightsGeometryToFollowCamera();
    }
    rwi2->Render();
}

void DoubleViewInteractorStyle::Spin()
{
    qInfo() << "Spin start.";
    if ( nullptr == this->CurrentRenderer ||
         nullptr == theOtherStyle->CurrentRenderer )
    {
        return;
    }

    vtkRenderWindowInteractor *rwi1 = this->Interactor;
    vtkRenderWindowInteractor *rwi2 = theOtherStyle->Interactor;

    double *center = this->CurrentRenderer->GetCenter();

    double newAngle =
      vtkMath::DegreesFromRadians( atan2( rwi1->GetEventPosition()[1] - center[1],
                                          rwi1->GetEventPosition()[0] - center[0] ) );

    double oldAngle =
      vtkMath::DegreesFromRadians( atan2( rwi1->GetLastEventPosition()[1] - center[1],
                                          rwi1->GetLastEventPosition()[0] - center[0] ) );

    vtkCamera *camera1 = this->CurrentRenderer->GetActiveCamera();
    camera1->Roll( newAngle - oldAngle );
    camera1->OrthogonalizeViewUp();

    vtkCamera *camera2 = theOtherStyle->CurrentRenderer->GetActiveCamera();
    camera2->Roll( newAngle - oldAngle );
    camera2->OrthogonalizeViewUp();

    rwi1->Render();
    rwi2->Render();
}

void DoubleViewInteractorStyle::Dolly()
{
    if ( this->CurrentRenderer == nullptr ||
         theOtherStyle->CurrentRenderer == nullptr )
    {
        return;
    }

    vtkRenderWindowInteractor *rwi = this->Interactor;
    double *center = this->CurrentRenderer->GetCenter();
    int dy = rwi->GetEventPosition()[1] - rwi->GetLastEventPosition()[1];
    double dyf = this->MotionFactor * dy / center[1];
    FatherDolly( pow( 1.1, dyf ) );
    theOtherStyle->FatherDolly( pow( 1.1, dyf ) );
}

void DoubleViewInteractorStyle::FatherDolly(double factor)
{
    vtkInteractorStyleTrackballCamera::Dolly( factor );
}

void DoubleViewInteractorStyle::Pan()
{
    qInfo() << "Pan start!";
    if ( this->CurrentRenderer == nullptr ||
         theOtherStyle->CurrentRenderer == nullptr )
    {
        return;
    }

    vtkRenderWindowInteractor *rwi1 = this->Interactor;
    vtkRenderWindowInteractor *rwi2 = theOtherStyle->Interactor;

    double viewFocus[4], focalDepth, viewPoint[3];
    double newPickPoint[4], oldPickPoint[4], motionVector[3];

    // Calculate the focal depth since we'll be using it a lot

    vtkCamera *camera1 = this->CurrentRenderer->GetActiveCamera();
    vtkCamera *camera2 = theOtherStyle->CurrentRenderer->GetActiveCamera();

    camera1->GetFocalPoint(viewFocus);
    this->ComputeWorldToDisplay(viewFocus[0], viewFocus[1], viewFocus[2],
                                viewFocus);
    focalDepth = viewFocus[2];

    this->ComputeDisplayToWorld(rwi1->GetEventPosition()[0],
                                rwi1->GetEventPosition()[1],
                                focalDepth,
                                newPickPoint);

    // Has to recalc old mouse point since the viewport has moved,
    // so can't move it outside the loop

    this->ComputeDisplayToWorld(rwi1->GetLastEventPosition()[0],
                                rwi1->GetLastEventPosition()[1],
                                focalDepth,
                                oldPickPoint);

    // Camera motion is reversed
    motionVector[0] = oldPickPoint[0] - newPickPoint[0];
    motionVector[1] = oldPickPoint[1] - newPickPoint[1];
    motionVector[2] = oldPickPoint[2] - newPickPoint[2];

    camera1->GetFocalPoint( viewFocus );
    camera1->GetPosition( viewPoint );
    camera1->SetFocalPoint( motionVector[0] + viewFocus[0],
                          motionVector[1] + viewFocus[1],
                          motionVector[2] + viewFocus[2] );

    camera1->SetPosition(motionVector[0] + viewPoint[0],
                        motionVector[1] + viewPoint[1],
                        motionVector[2] + viewPoint[2]);

    camera2->GetFocalPoint( viewFocus );
    camera2->GetPosition( viewPoint );
    camera2->SetFocalPoint( motionVector[0] + viewFocus[0],
                          motionVector[1] + viewFocus[1],
                          motionVector[2] + viewFocus[2] );

    camera2->SetPosition(motionVector[0] + viewPoint[0],
                        motionVector[1] + viewPoint[1],
                        motionVector[2] + viewPoint[2]);

    if (rwi1->GetLightFollowCamera())
    {
        this->CurrentRenderer->UpdateLightsGeometryToFollowCamera();
    }

    rwi1->Render();

    if (rwi2->GetLightFollowCamera())
    {
        theOtherStyle->CurrentRenderer->UpdateLightsGeometryToFollowCamera();
    }

    rwi2->Render();
}

DoubleViewInteractorStyle::DoubleViewInteractorStyle()
{
    theOtherStyle = NULL;
}
