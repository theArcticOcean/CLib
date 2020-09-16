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
#include <vtkMatrix4x4.h>

#include "vtkCustomStyle.h"
#include "../tool.h"

#define vtkSPtr vtkSmartPointer
#define vtkSPtrNew(Var, Type) vtkSPtr<Type> Var = vtkSPtr<Type>::New();

int main(int argc, char *argv[])
{
    // ------------- init scene -----------------
    std::string path = "../tooth.vtp";
    vtkSPtrNew( toothReader, vtkXMLPolyDataReader );
    toothReader->SetFileName( path.c_str() );
    toothReader->Update();

    vtkSPtrNew( toothMapper, vtkPolyDataMapper );
    toothMapper->SetInputData( toothReader->GetOutput() );

    vtkSPtrNew( toothActor, vtkActor );
    toothActor->SetMapper( toothMapper );
    toothActor->GetProperty()->SetColor( 1, 1, 1 );

    vtkSPtrNew( axesActor, vtkAxesActor );
    path = "../bracket.vtp";
    vtkSPtrNew( bracketReader, vtkXMLPolyDataReader );
    bracketReader->SetFileName( path.c_str() );
    bracketReader->Update();
    vtkSPtrNew( mapper, vtkPolyDataMapper );
    mapper->SetInputConnection( bracketReader->GetOutputPort() );

    vtkSPtrNew( bracketActor, vtkActor );
    bracketActor->SetMapper( mapper );
    bracketActor->GetProperty()->SetColor( 1, 0, 0 );

    vtkSPtrNew( bracketTrans, vtkTransform );
    double elements[4][4] = {{0.142535, -0.912866, 0.382571, 5.80672},
                             {0.953836, 0.229913, 0.19323, 7.26857 },
                             {-0.264351, 0.337367, 0.903494, 25.1871},
                             { 0, 0, 0, 1}};
    vtkSPtrNew( matrix, vtkMatrix4x4 );
    for( int i = 0; i < 4; ++i )
    {
        for( int j = 0; j < 4; ++j )
        {
            matrix->SetElement( i, j, elements[i][j] );
        }
    }
    bracketTrans->SetMatrix( matrix );
    bracketActor->SetUserTransform( bracketTrans );

    vtkSPtrNew( renderer, vtkRenderer );
    renderer->AddActor( bracketActor );
    //renderer->AddActor( axesActor );
    renderer->AddActor( toothActor );
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
    style->InitPicker( bracketActor );
    style->Setm_ToothActor( toothActor );
    style->ConfigureOBBTree();
    renderWindowInteractor->SetInteractorStyle( style );
    // ------------- finished: configure vtkCustomStyle ----------------

    renderer->ResetCamera();
    renderWindow->Render();
    renderWindowInteractor->Start();
    return 0;
}
