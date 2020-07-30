#include <iostream>
#include <vtkSmartPointer.h>
#include <vtkSphereSource.h>
#include <vtkActor.h>
#include <vtkConeSource.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkPolyDataMapper.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkProperty.h>
#include <vtkPolyDataMapper2D.h>
#include <vtkActor2D.h>
#include <vtkCoordinate.h>
#include <vtkCamera.h>
#include <vtkProperty2D.h>

#include "../tool.h"

#define vtkSPtr vtkSmartPointer
#define vtkSPtrNew(Var, Type) vtkSPtr<Type> Var = vtkSPtr<Type>::New();

using namespace std;

int main()
{
    vtkSPtrNew( cone, vtkConeSource );
    vtkSPtrNew( mapper, vtkPolyDataMapper );
    mapper->SetInputConnection( cone->GetOutputPort() );

    vtkSPtrNew( actor, vtkActor );
    actor->SetMapper( mapper );

    vtkSPtrNew( renderer, vtkRenderer );
    renderer->AddActor(actor);
    renderer->SetBackground( 0, 0, 0 );

    vtkSPtrNew( renderWindow, vtkRenderWindow );
    renderWindow->AddRenderer( renderer );
    renderWindow->SetSize( 100, 100 );

    // ------------- add lmm lines -----------------
    vtkSPtrNew( v1mmMapper, vtkPolyDataMapper2D );
    vtkSPtrNew( v1mmPolyData, vtkPolyData );
    vtkSPtrNew( v1mmLines, vtkCellArray );
    vtkSPtrNew( v1mmPoints, vtkPoints );
    vtkSPtrNew( coords, vtkCoordinate );

    v1mmPolyData->SetPoints( v1mmPoints );
    v1mmPolyData->SetLines( v1mmLines );
    v1mmMapper->SetInputData( v1mmPolyData );

    coords->SetCoordinateSystemToView();
    v1mmMapper->SetTransformCoordinate( coords );

    vtkSPtrNew( v1mmLinesActor, vtkActor2D );
    v1mmLinesActor->SetMapper( v1mmMapper );
    v1mmLinesActor->GetProperty()->SetColor( 1, 0, 0 );
    v1mmLinesActor->GetProperty()->SetLineWidth( 1 );
    v1mmLinesActor->DragableOff();
    v1mmLinesActor->PickableOff();

    renderer->AddActor2D( v1mmLinesActor );

    //--------------------start generate lines------------------
    double mmSize = 0.2;
    v1mmPoints->Initialize();
    v1mmLines->Initialize();
    double width = renderWindow->GetScreenSize()[0]; //GetActualSize()[0];
    double height = renderWindow->GetScreenSize()[1];
    printf("w: %lf, h: %lf\n", width, height);

    double x, y;
    //--------------------vertical lines------------------
    for ( x = -1; x <= 1; x += mmSize )
    {
        double linePoint1[3] = { x, -1, 0.0 };
        double linePoint2[3] = { x, 1, 0.0 };
        vtkIdType pointId1 = v1mmPoints->InsertNextPoint(linePoint1);
        vtkIdType pointId2 = v1mmPoints->InsertNextPoint(linePoint2);
        vtkIdType lineIds[2] = { pointId1, pointId2 };
        v1mmLines->InsertNextCell(2, lineIds);
    }
    //--------------------horizontal lines----------------
    for ( y = -1; y <= 1; y += mmSize )
    {
        double linePoint1[3] = { -1, y, 0.0 };
        double linePoint2[3] = { 1, y, 0.0 };
        vtkIdType pointId1 = v1mmPoints->InsertNextPoint(linePoint1);
        vtkIdType pointId2 = v1mmPoints->InsertNextPoint(linePoint2);
        vtkIdType lineIds[2] = { pointId1, pointId2 };
        v1mmLines->InsertNextCell(2, lineIds);
    }

    v1mmPolyData->Modified();
    //-------------------- end ------------------

    // ------------- finished: 1mm lines -----------

    vtkSPtrNew( renderWindowInteractor, vtkRenderWindowInteractor );
    renderWindowInteractor->SetRenderWindow( renderWindow );

    // -------------- configure camera --------------
    renderer->ResetCamera();
    vtkCamera *camera = renderer->GetActiveCamera();
    PointStruct cameraPos;
    camera->GetPosition( cameraPos.point );
    PointStruct focusPos;
    camera->GetFocalPoint( focusPos.point );
    PointStruct viewDir = cameraPos - focusPos;
    viewDir.Unit();
    cameraPos = focusPos + viewDir * 10;
    camera->SetPosition( cameraPos.point );

    renderer->ResetCameraClippingRange();
    // -------------- configure camera --------------

    renderWindow->Render();
    renderWindowInteractor->Start();
    return 0;
}
