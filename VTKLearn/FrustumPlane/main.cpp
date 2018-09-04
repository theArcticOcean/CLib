#include <vtkActor.h>
#include <vtkActor2D.h>
#include <vtkCamera.h>
#include <vtkHull.h>
#include <vtkNamedColors.h>
#include <vtkPlanes.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkSmartPointer.h>
#include <vtkSphereSource.h>
#include <vtkTextMapper.h>
#include <vtkTextProperty.h>
#include <vtkPlane.h>

#include <string>
#include <vector>
#include <stdio.h>

#include "MyStyle.h"

int main(int, char* [])
{
    setbuf(stdout, NULL);

    vtkSmartPointer<vtkNamedColors> colors =
            vtkSmartPointer<vtkNamedColors>::New();

    vtkSmartPointer<vtkPlanes> planes =
            vtkSmartPointer<vtkPlanes>::New();

    // Using frustum planess.
    vtkSmartPointer<vtkCamera> camera =
            vtkSmartPointer<vtkCamera>::New();
    double planesArray[24];
    // 得到约束视图截锥体的平面方程。
    camera->GetFrustumPlanes( 1, planesArray );
    planesArray[20] = planesArray[21] = planesArray[22] = planesArray[23] = 0;
    planes->SetFrustumPlanes( planesArray );
    std::cout << "planes->GetNumberOfPlanes(): " << planes->GetNumberOfPlanes() << std::endl;
    printf( "(%lf, %lf, %lf, %lf)\n", planesArray[0], planesArray[1], planesArray[2], planesArray[3] );
    printf( "(%lf, %lf, %lf, %lf)\n", planesArray[4], planesArray[5], planesArray[6], planesArray[7] );
    printf( "(%lf, %lf, %lf, %lf)\n", planesArray[8], planesArray[9], planesArray[10], planesArray[11] );
    printf( "(%lf, %lf, %lf, %lf)\n", planesArray[12], planesArray[13], planesArray[14], planesArray[15] );
    printf( "(%lf, %lf, %lf, %lf)\n", planesArray[16], planesArray[17], planesArray[18], planesArray[19] );
    printf( "(%lf, %lf, %lf, %lf)\n", planesArray[20], planesArray[21], planesArray[22], planesArray[23] );

    double *origin  = NULL;
    origin = planes->GetPlane( 0 )->GetOrigin();
    printf( "origin: (%lf, %lf, %lf)\n", origin[0], origin[1], origin[2] );
    origin = planes->GetPlane( 1 )->GetOrigin();
    printf( "origin: (%lf, %lf, %lf)\n", origin[0], origin[1], origin[2] );
    origin = planes->GetPlane( 2 )->GetOrigin();
    printf( "origin: (%lf, %lf, %lf)\n", origin[0], origin[1], origin[2] );
    origin = planes->GetPlane( 3 )->GetOrigin();
    printf( "origin: (%lf, %lf, %lf)\n", origin[0], origin[1], origin[2] );
    origin = planes->GetPlane( 4 )->GetOrigin();
    printf( "origin: (%lf, %lf, %lf)\n", origin[0], origin[1], origin[2] );
    origin = planes->GetPlane( 5 )->GetOrigin();
    printf( "origin: (%lf, %lf, %lf)\n", origin[0], origin[1], origin[2] );

    vtkSmartPointer<vtkTextProperty> textProperty =
            vtkSmartPointer<vtkTextProperty>::New();
    textProperty->SetFontSize(16);
    textProperty->SetJustificationToCentered();

    // Create the render window and interactor.
    vtkSmartPointer<vtkRenderWindow> renWin =
            vtkSmartPointer<vtkRenderWindow>::New();
    renWin->SetWindowName("Planes");
    vtkSmartPointer<vtkRenderWindowInteractor> iRen =
            vtkSmartPointer<vtkRenderWindowInteractor>::New();
    iRen->SetRenderWindow(renWin);

    vtkSmartPointer<vtkHull> hull =
            vtkSmartPointer<vtkHull>::New();
    vtkSmartPointer<vtkPolyData> pd =
            vtkSmartPointer<vtkPolyData>::New();
    vtkSmartPointer<vtkPolyDataMapper> mapper =
            vtkSmartPointer<vtkPolyDataMapper>::New();
    vtkSmartPointer<vtkActor> actor =
            vtkSmartPointer<vtkActor>::New();
    vtkSmartPointer<vtkTextMapper> textMapper =
            vtkSmartPointer<vtkTextMapper>::New();
    vtkSmartPointer<vtkActor2D> textActor =
            vtkSmartPointer<vtkActor2D>::New();
    vtkSmartPointer<vtkRenderer> renderer =
            vtkSmartPointer<vtkRenderer>::New();

    hull->SetPlanes( planes );
    hull->GenerateHull( pd, -150, 150, -150, 150, -150, 150 );
    mapper->SetInputData( pd );
    actor->SetMapper( mapper );
    textMapper->SetInput( actor->GetClassName() );
    textMapper->SetTextProperty( textProperty );
    textActor->SetMapper( textMapper );
    textActor->SetPosition( 120, 16 );
    renderer->AddActor( actor );
    renderer->AddViewProp( textActor );
    renWin->AddRenderer( renderer );

    vtkSmartPointer<MyStyle> style =
            vtkSmartPointer<MyStyle>::New();
    style->SetDefaultRenderer( renderer );
    iRen->SetInteractorStyle(style);
    iRen->Initialize();
    renWin->Render();
    iRen->Start();

    return EXIT_SUCCESS;
}
