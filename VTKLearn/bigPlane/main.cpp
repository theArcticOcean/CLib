#include <vtkImageData.h>
#include <vtkImageCast.h>
#include <vtkSmartPointer.h>
#include <vtkImageMapper3D.h>
#include <vtkImageFlip.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkInteractorStyleImage.h>
#include <vtkRenderer.h>
#include <vtkImageMandelbrotSource.h>
#include <vtkImageActor.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkPNGReader.h>
#include <vtkPNGWriter.h>
#include <vtkPlaneSource.h>
#include <vtkAxesActor.h>
#include <vtkAppendPolyData.h>

#include "tool.h"

/*
*   We can guess a point by plane equation, then use it and center, normal to
*   calculate another two points on plane.
*   The interface is used to calculate four small squares to form a big plane
*   which center and normal are same as parameters.
*/
vtkSmartPointer<vtkPolyData> GetBigPlane(const PointStruct _center,
                                         const PointStruct _normal,
                                         const double lengthOfHalfDiagonal,
                                         const PointStruct guessPoint)
{
    vtkSmartPointer<vtkAppendPolyData> appendFilter =
            vtkSmartPointer<vtkAppendPolyData>::New();

    // a(x - x0) + b(y - y0) + c(z - z0) = 0
    vtkSmartPointer<vtkPlaneSource> planeSource0 =
            vtkSmartPointer<vtkPlaneSource>::New();
    //planeSource->SetCenter( center.point );
    //planeSource->SetNormal( normal.point );

    PointStruct center = _center;
    PointStruct normal = _normal;
    // there is at least one point on axes and on plane at the same time
    planeSource0->SetOrigin( center.point );

    // upper square
    PointStruct p1 = guessPoint;
    p1 = (p1 - center).Unit() * lengthOfHalfDiagonal + center;

    PointStruct p2 = ((p1 - center)^normal).Unit() * lengthOfHalfDiagonal + center;
    planeSource0->SetPoint1( p1.point );
    planeSource0->SetPoint2( p2.point );
    planeSource0->Update();

    appendFilter->AddInputData( planeSource0->GetOutput() );

    // left square
    p1 = guessPoint;
    p1 = (p1 - center).Unit() * lengthOfHalfDiagonal + center;
    p2 = ((p1 - center)^normal).Unit() * (-lengthOfHalfDiagonal) + center;
    vtkSmartPointer<vtkPlaneSource> planeSource1 =
            vtkSmartPointer<vtkPlaneSource>::New();
    planeSource1->SetOrigin( center.point );
    planeSource1->SetPoint1( p1.point );
    planeSource1->SetPoint2( p2.point );
    planeSource1->Update();

    appendFilter->AddInputData( planeSource1->GetOutput() );

    // lower square
    p1 = guessPoint;
    p1 = (p1 - center).Unit() * (-lengthOfHalfDiagonal) + center;
    p2 = ((p1 - center)^normal).Unit() * (lengthOfHalfDiagonal) + center;
    vtkSmartPointer<vtkPlaneSource> planeSource2 =
            vtkSmartPointer<vtkPlaneSource>::New();
    planeSource2->SetOrigin( center.point );
    planeSource2->SetPoint1( p1.point );
    planeSource2->SetPoint2( p2.point );
    planeSource2->Update();

    appendFilter->AddInputData( planeSource2->GetOutput() );

    // right square
    p1 = guessPoint;
    p1 = (p1 - center).Unit() * (-lengthOfHalfDiagonal) + center;
    p2 = ((p1 - center)^normal).Unit() * (-lengthOfHalfDiagonal) + center;
    vtkSmartPointer<vtkPlaneSource> planeSource3 =
            vtkSmartPointer<vtkPlaneSource>::New();
    planeSource3->SetOrigin( center.point );
    planeSource3->SetPoint1( p1.point );
    planeSource3->SetPoint2( p2.point );
    planeSource3->Update();

    appendFilter->AddInputData( planeSource3->GetOutput() );

    appendFilter->Update();
    return appendFilter->GetOutput();
}

int main()
{
    PointStruct center( 0, 1, 0 );
    PointStruct normal( 1, 0, 0 );
    PointStruct guessPoint( 0, 2, 1 );

    vtkSmartPointer<vtkPolyDataMapper> planeMapper =
            vtkSmartPointer<vtkPolyDataMapper>::New();

    planeMapper->SetInputData( GetBigPlane( center, normal, 2, guessPoint ) );

    vtkSmartPointer<vtkActor> planeActor =
            vtkSmartPointer<vtkActor>::New();
    planeActor->SetMapper( planeMapper );
    planeActor->GetProperty()->SetColor( 1, 1, 0 );

    vtkSmartPointer<vtkRenderer> renderer =
        vtkSmartPointer<vtkRenderer>::New();
    vtkSmartPointer<vtkRenderWindow> renderWindow =
        vtkSmartPointer<vtkRenderWindow>::New();
    renderWindow->AddRenderer( renderer );
    vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor =
        vtkSmartPointer<vtkRenderWindowInteractor>::New();
    renderWindowInteractor->SetRenderWindow(renderWindow);
    renderer->SetBackground(.1, .2, .3);

    renderer->AddActor(planeActor);

    vtkSmartPointer<vtkAxesActor> axesActor = vtkSmartPointer<vtkAxesActor>::New();
    renderer->AddActor( axesActor );

    renderer->ResetCamera();
    renderWindow->Render();

    renderWindowInteractor->Start();
    return 0;
}
