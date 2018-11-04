#include "widget.h"
#include "ui_widget.h"

#include <vtkGenericOpenGLRenderWindow.h>
#include <vtkRendererCollection.h>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget),
    coneActor( vtkSmartPointer<vtkActor>::New() ),
    textActor( vtkSmartPointer<vtkTextActor>::New() )
{
    ui->setupUi(this);

    vtkSmartPointer<vtkConeSource> cone =
            vtkSmartPointer<vtkConeSource>::New();

    vtkSmartPointer<vtkPolyDataMapper> coneMapper =
            vtkSmartPointer<vtkPolyDataMapper>::New();
    coneMapper->SetInputConnection( cone->GetOutputPort() );
    coneActor->SetMapper( coneMapper );

    vtkSmartPointer<vtkRenderer> backRenderer =
            vtkSmartPointer<vtkRenderer>::New();
    backRenderer->AddActor( coneActor );
    backRenderer->SetBackground( 1, 1, 1 );

    textActor->SetInput( "hello" );
    textActor->GetProperty()->SetColor( 1, 0, 0 );
    textActor->SetPosition( 0, 10 );
    backRenderer->AddActor( textActor );

    vtkSmartPointer<vtkGenericOpenGLRenderWindow> backWindow =
            vtkSmartPointer<vtkGenericOpenGLRenderWindow>::New();
    backWindow->AddRenderer( backRenderer );

    ui->qvtkWidget->SetRenderWindow( backWindow );
    printf( "ui->qvtkWidget->GetRenderWindow(): %p\n", ui->qvtkWidget->GetRenderWindow() );
    ui->qvtkWidget->GetRenderWindow()->AddRenderer( backRenderer );
    ui->qvtkWidget->GetRenderWindow()->GetInteractor()->SetInteractorStyle( NULL );

    double size[2];
    textActor->GetSize( backRenderer, size );
    printf( "size: %lf, %lf\n", size[0], size[1] );
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_pushButton_clicked()
{
    textActor->SetInput( "hello\nworld\nmac" );

    double size[2];
    vtkRendererCollection *rendererCollection = ui->qvtkWidget->GetRenderWindow()->GetRenderers();
    vtkRenderer *renderer = rendererCollection->GetFirstRenderer();
    textActor->GetSize( renderer, size );
    printf( "size: %lf, %lf\n", size[0], size[1] );
    //renderer->Render();
    ui->qvtkWidget->GetRenderWindow()->Render();
}
