#include "widget.h"
#include "ui_widget.h"

#include <qDebug>
#include <vtkClipPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkPlane.h>
#include <vtkActor.h>
#include <QCoreApplication>
#include <vtkSTLReader.h>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget),
    m_STLReader( vtkSmartPointer<vtkSTLReader>::New() ),
    m_Renderer( vtkSmartPointer<vtkRenderer>::New() ),
    m_RenderWindow( vtkSmartPointer<vtkRenderWindow>::New() ),
    m_ClipPlane( vtkSmartPointer<vtkPlane>::New() ),
    m_ArchActor( vtkSmartPointer<vtkActor>::New() ),
    m_SelectActor( vtkSmartPointer<vtkActor>::New() ),
    m_Clipper( vtkSmartPointer<vtkClipPolyData>::New() ),
    m_SelectMapper( vtkSmartPointer<vtkPolyDataMapper>::New() ),
    m_PlaneWidget( vtkSmartPointer<vtkImplicitPlaneWidget>::New() ),
    m_ClipPlaneNormal( PointStruct( 0, 0, 1 ) ),
    m_LastValue( 0 )
{
    ui->setupUi(this);
    loadSTLFile();

    double *bounds = m_ArchActor->GetBounds();
    printf( "bounds: (%lf, %lf, %lf, %lf, %lf, %lf)\n",
            bounds[0], bounds[1], bounds[2], bounds[3], bounds[4], bounds[5] );
    m_ZLength = bounds[5] - bounds[4];

    clipSetting();
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_verticalSlider_valueChanged(int value)
{
    qDebug() << "valueChanged: " << value;

    int dif = value - m_LastValue;
    PointStruct origin( m_ClipPlane->GetOrigin() );
    origin = origin - m_ClipPlaneNormal * m_ZLength * dif / 100.0;
    m_ClipPlane->SetOrigin( origin.point );
    m_LastValue = value;
    ui->qvtkWidget->GetRenderWindow()->Render();
}

void Widget::loadSTLFile()
{
    QString dir = QCoreApplication::applicationDirPath();
    dir = dir + "/LingerBar.stl";
    qDebug() << "dir: " << dir;

    m_STLReader->SetFileName( dir.toStdString().c_str() );
    m_STLReader->Update();

    vSPNew(archMapper, vtkPolyDataMapper);
    archMapper->SetInputConnection( m_STLReader->GetOutputPort() );

    m_ArchActor->SetMapper( archMapper );
}

void Widget::clipSetting()
{
    m_ClipPlane->SetNormal( m_ClipPlaneNormal.point );
    m_ClipPlane->SetOrigin( 0.0, 0.0, m_ZLength/2 );

    m_Clipper->SetInputConnection( m_STLReader->GetOutputPort() );
    m_Clipper->SetClipFunction( m_ClipPlane );
    m_Clipper->InsideOutOn();

    m_SelectMapper->SetInputConnection( m_Clipper->GetOutputPort() );

    m_SelectActor->SetMapper( m_SelectMapper );

    m_Renderer->AddActor( m_SelectActor );
    m_RenderWindow->AddRenderer( m_Renderer );

    ui->qvtkWidget->SetRenderWindow( m_RenderWindow );
}
