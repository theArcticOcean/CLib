#include <iostream>
#include <vtkSmartPointer.h>
#include <vtkSphereSource.h>
#include <vtkActor.h>
#include <vtkConeSource.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkLight.h>
#include <vtkCamera.h>
#include <vtkActor2D.h>
#include <vtkTextActor.h>
#include <vtkProperty2D.h>
#include <vtkTextProperty.h>

using namespace std;

vtkSmartPointer<vtkTextActor> textActor =
        vtkSmartPointer<vtkTextActor>::New();

vtkSmartPointer<vtkRenderer> renderer =
        vtkSmartPointer<vtkRenderer>::New();

vtkSmartPointer<vtkRenderWindow> renderWindow =
        vtkSmartPointer<vtkRenderWindow>::New();

class CallBack: public vtkCommand
{
public:
    static CallBack *New()
    {
        return new CallBack;
    }

    virtual void Execute(vtkObject *caller, unsigned long eventId,
                         void *callData)
    {
        textActor->SetInput( "Hello\nHello\nHello" );
        double actorSize[2];
        textActor->GetSize( renderer, actorSize );
        printf( "actorSize size: %lf, %lf\n", actorSize[0], actorSize[1] );
        int *winSize = renderWindow->GetSize();
        printf( "winSize: %d, %d\n", winSize[0], winSize[1] );
        textActor->SetPosition( winSize[0] / 2.0 - actorSize[0] / 2.0, winSize[1] / 2.0 - actorSize[1] / 2.0 );
    }
};

int main()
{
    setbuf( stdout, NULL );
    vtkSmartPointer<vtkConeSource> cone =
            vtkSmartPointer<vtkConeSource>::New();

    vtkSmartPointer<vtkPolyDataMapper> mapper =
            vtkSmartPointer<vtkPolyDataMapper>::New();
    mapper->SetInputConnection( cone->GetOutputPort() );

    vtkSmartPointer<vtkActor> actor =
            vtkSmartPointer<vtkActor>::New();
    actor->SetMapper( mapper );

    vtkSmartPointer<vtkRenderer> renderer =
            vtkSmartPointer<vtkRenderer>::New();
    renderer->AddActor(actor);
    renderer->SetBackground( 0, 0, 0 );

    renderWindow->AddRenderer( renderer );

    vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor =
            vtkSmartPointer<vtkRenderWindowInteractor>::New();
    renderWindowInteractor->SetRenderWindow( renderWindow );

    renderWindow->Render();

    renderer->AddActor( textActor );

    textActor->SetInput( "Hello\n" );
    textActor->GetProperty()->SetColor( 1, 0, 0 );
    textActor->GetTextProperty()->SetFontSize( 20 );
    double actorSize[2];
    textActor->GetSize( renderer, actorSize );
    printf( "actorSize size: %lf, %lf\n", actorSize[0], actorSize[1] );
    int *winSize = renderWindow->GetSize();
    printf( "winSize: %d, %d\n", winSize[0], winSize[1] );
    textActor->SetPosition( winSize[0] / 2.0 - actorSize[0] / 2.0, winSize[1] / 2.0 - actorSize[1] / 2.0 );

    CallBack cb;
    actor->AddObserver( vtkCommand::RightButtonPressEvent, &cb );

    renderer->ResetCamera();
    renderWindow->Render();
    renderWindowInteractor->Start();
    return 0;
}
