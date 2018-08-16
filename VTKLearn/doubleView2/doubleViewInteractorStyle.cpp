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

void DoubleViewInteractorStyle::Rotate()
{
    if ( this->CurrentRenderer == nullptr ||
         nullptr == theOtherStyle->CurrentRenderer )
    {
        return;
    }

    vtkRenderWindowInteractor *rwi1 = this->Interactor;
    vtkRenderWindowInteractor *rwi2 = theOtherStyle->Interactor;

    int dx = rwi1->GetEventPosition()[0] - rwi1->GetLastEventPosition()[0];
    int dy = rwi1->GetEventPosition()[1] - rwi1->GetLastEventPosition()[1];
    qDebug( "( %d, %d )", rwi1->GetEventPosition()[0],  rwi1->GetEventPosition()[1] );

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
    if ( nullptr == this->CurrentRenderer ||
         nullptr == theOtherStyle->CurrentRenderer )
    {
        return;
    }

    vtkRenderWindowInteractor *rwi = this->Interactor;

    double *center = this->CurrentRenderer->GetCenter();

    double newAngle =
      vtkMath::DegreesFromRadians( atan2( rwi->GetEventPosition()[1] - center[1],
                                          rwi->GetEventPosition()[0] - center[0] ) );

    double oldAngle =
      vtkMath::DegreesFromRadians( atan2( rwi->GetLastEventPosition()[1] - center[1],
                                          rwi->GetLastEventPosition()[0] - center[0] ) );

    vtkCamera *camera1 = this->CurrentRenderer->GetActiveCamera();
    camera1->Roll( newAngle - oldAngle );
    camera1->OrthogonalizeViewUp();

    vtkCamera *camera2 = theOtherStyle->CurrentRenderer->GetActiveCamera();
    camera2->Roll( newAngle - oldAngle );
    camera2->OrthogonalizeViewUp();

    rwi->Render();
}

DoubleViewInteractorStyle::DoubleViewInteractorStyle()
{
    theOtherStyle = NULL;
}
