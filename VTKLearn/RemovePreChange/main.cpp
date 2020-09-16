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
#include <vtkTransform.h>

#include "../tool.h"

#define vtkSPtr vtkSmartPointer
#define vtkSPtrNew(Var, Type) vtkSPtr<Type> Var = vtkSPtr<Type>::New();

using namespace std;

void ConvertToStage1( vtkActor *actor );
void ConvertToStage2( vtkActor *actor );

int main()
{
    double zoomFactor = 0.3;
    vtkSPtrNew( cone, vtkConeSource );
    vtkSPtrNew( mapper, vtkPolyDataMapper );
    mapper->SetInputConnection( cone->GetOutputPort() );

    vtkSPtrNew( actor, vtkActor );
    actor->SetMapper( mapper );

    vtkSPtrNew( renderer, vtkRenderer );
    renderer->AddActor(actor);
    renderer->SetBackground( 0, 0, 0 );
    renderer->GetActiveCamera()->ParallelProjectionOn();
    renderer->GetActiveCamera()->Zoom( zoomFactor );
    renderer->SetLayer( 1 );

    vtkSPtrNew( backRenderer, vtkRenderer );
    backRenderer->SetBackground( 0, 0, 0 );
    backRenderer->SetInteractive( 0 );
    backRenderer->GetActiveCamera()->ParallelProjectionOn();
    backRenderer->GetActiveCamera()->Zoom( zoomFactor );
    backRenderer->SetLayer( 0 );

    vtkSPtrNew( renderWindow, vtkRenderWindow );
    renderWindow->SetNumberOfLayers( 2 );
    renderWindow->AddRenderer( backRenderer );
    renderWindow->AddRenderer( renderer );
    renderWindow->SetSize( 100, 100 );

    // ------------- add lmm lines -----------------
    vtkSPtrNew( v1mmMapper, vtkPolyDataMapper );
    vtkSPtrNew( v1mmPolyData, vtkPolyData );
    vtkSPtrNew( v1mmLines, vtkCellArray );
    vtkSPtrNew( v1mmPoints, vtkPoints );

    v1mmPolyData->SetPoints( v1mmPoints );
    v1mmPolyData->SetLines( v1mmLines );
    v1mmMapper->SetInputData( v1mmPolyData );

    vtkSPtrNew( v1mmLinesActor, vtkActor );
    v1mmLinesActor->SetMapper( v1mmMapper );
    v1mmLinesActor->GetProperty()->SetColor( 1, 0, 0 );
    v1mmLinesActor->GetProperty()->SetLineWidth( 1 );
    v1mmLinesActor->DragableOff();
    v1mmLinesActor->PickableOff();

    backRenderer->AddActor( v1mmLinesActor );

    //--------------------start generate lines------------------
    double mmSize = 1;
    v1mmPoints->Initialize();
    v1mmLines->Initialize();
    double width = renderWindow->GetScreenSize()[0]; //GetActualSize()[0];
    double height = renderWindow->GetScreenSize()[1];
    printf("w: %lf, h: %lf\n", width, height);

    double x, y;
    //--------------------vertical lines------------------
    for ( x = -width / 2; x <= width / 2; x += mmSize )
    {
        double linePoint1[3] = { x, -height/2, 0.0 };
        double linePoint2[3] = { x, height/2, 0.0 };
        vtkIdType pointId1 = v1mmPoints->InsertNextPoint(linePoint1);
        vtkIdType pointId2 = v1mmPoints->InsertNextPoint(linePoint2);
        vtkIdType lineIds[2] = { pointId1, pointId2 };
        v1mmLines->InsertNextCell(2, lineIds);
    }
    //--------------------horizontal lines----------------
    for ( y = -height/2; y <= height/2; y += mmSize )
    {
        double linePoint1[3] = { -width/2, y, 0.0 };
        double linePoint2[3] = { width/2, y, 0.0 };
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

    ConvertToStage1( actor );
    vtkSPtrNew( trans1, vtkTransform );
    trans1->DeepCopy( actor->GetUserTransform() );

    ConvertToStage2( actor );
    vtkSPtrNew( trans2, vtkTransform );
    trans2->DeepCopy( actor->GetUserTransform() );

    trans1->Inverse();
    trans1->Update();

    trans2->Concatenate( trans1 );
    trans2->Update();
    actor->SetUserTransform( trans2 );

    renderWindow->Render();
    renderWindowInteractor->Start();
    return 0;
}

void ConvertToStage1( vtkActor *actor )
{
    vtkSPtrNew( trans, vtkTransform );
    trans->RotateZ( 30 );
    trans->Scale( 2, 2, 2 );
    trans->Update();

    actor->SetUserTransform( trans );
};

void ConvertToStage2( vtkActor *actor )
{
    vtkSPtrNew( trans, vtkTransform );
    trans->Translate( 2, 2, 0 );
    trans->Update();
    if( actor->GetUserTransform() )
    {
        trans->Concatenate( actor->GetUserTransform() );
        trans->Update();
    }

    actor->SetUserTransform( trans );
};
