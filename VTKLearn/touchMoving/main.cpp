#include <iostream>
#include <vtkSmartPointer.h>
#include <vtkSphereSource.h>
#include <vtkActor.h>
#include <vtkConeSource.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkPolyDataMapper.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkCubeSource.h>
#include <vtkAxesActor.h>
#include <vtkProperty.h>
#include <vtkTransformFilter.h>
#include <vtkXMLPolyDataReader.h>
#include <vtkTransform.h>
#include <vtkCamera.h>

#include "vtkCustomStyle.h"
#include "../tool.h"

#define vtkSPtr vtkSmartPointer
#define vtkSPtrNew(Var, Type) vtkSPtr<Type> Var = vtkSPtr<Type>::New();

int main(int argc, char *argv[])
{
    // ------------- init scene -----------------
    std::string path = "../superquadric.vtp";
    vtkSPtrNew( xmlReader, vtkXMLPolyDataReader );
    xmlReader->SetFileName( path.c_str() );
    xmlReader->Update();

    vtkSPtrNew( trans, vtkTransform );
    trans->Scale( 10, 10, 10 );
    trans->Update();
    vtkSPtrNew( transFilter, vtkTransformFilter );
    transFilter->SetInputData( xmlReader->GetOutput() );
    transFilter->SetTransform( trans );
    transFilter->Update();

    vtkSPtrNew( xmlMapper, vtkPolyDataMapper );
    xmlMapper->SetInputData( transFilter->GetPolyDataOutput() );

    vtkSPtrNew( quadricActor, vtkActor );
    quadricActor->SetMapper( xmlMapper );
    quadricActor->GetProperty()->SetColor( 1, 0, 0 );

    vtkSPtrNew( axesActor, vtkAxesActor );
    vtkSPtrNew( cube, vtkCubeSource );
    vtkSPtrNew( mapper, vtkPolyDataMapper );
    mapper->SetInputConnection( cube->GetOutputPort() );

    vtkSPtrNew( cubeActor, vtkActor );
    cubeActor->SetMapper( mapper );
    cubeActor->GetProperty()->SetColor( 1, 0, 0 );

    vtkSPtrNew( cubeTrans, vtkTransform );
    cubeTrans->Translate( 0, 0, 5.5 );
    cubeTrans->Update();
    cubeActor->SetUserTransform( cubeTrans );

    vtkSPtrNew( renderer, vtkRenderer );
    renderer->AddActor( cubeActor );
    renderer->AddActor( axesActor );
    renderer->AddActor( quadricActor );
    renderer->SetBackground( 0, 0, 0 );
    // ------------- finished: init scene -----------------

    vtkSPtrNew( renderWindow, vtkRenderWindow );
    renderWindow->AddRenderer( renderer );

    vtkSPtrNew( renderWindowInteractor, vtkRenderWindowInteractor );
    renderWindowInteractor->SetRenderWindow( renderWindow );

    // ------------- configure vtkCustomStyle ----------------
    vtkSPtrNew( style, vtkCustomStyle );
    style->Setm_RenderWindow( renderWindow );
    style->Setm_Renderer( renderer );
    style->Setm_Interator( renderWindowInteractor );
    style->InitPicker( cubeActor );
    style->Setm_QuadricActor( quadricActor );
    style->ConfigureOBBTree();

    renderWindowInteractor->SetInteractorStyle( style );
    // ------------- finished: configure vtkCustomStyle ----------------

    renderer->ResetCamera();
    renderer->GetActiveCamera()->ParallelProjectionOn();
    renderWindow->Render();
    renderWindowInteractor->Start();
    return 0;
}
