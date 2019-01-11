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
    m_Renderer( vtkSmartPointer<vtkRenderer>::New() ),
    m_RenderWindow( vtkSmartPointer<vtkRenderWindow>::New() ),
    m_ClipPlane( vtkSmartPointer<vtkPlane>::New() ),
    m_ClipPlaneNormal( PointStruct( 0, 0, 1 ) )
{
    ui->setupUi(this);
    loadSTLFile();
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_verticalSlider_valueChanged(int value)
{
    qDebug() << "valueChanged: " << value;
}

void Widget::loadSTLFile()
{
    QString dir = QCoreApplication::applicationDirPath();
    dir = dir + "/LingerBar.stl";
    qDebug() << "dir: " << dir;

    vSPNew(STLReader, vtkSTLReader);
    STLReader->SetFileName( dir.toStdString().c_str() );
    STLReader->Update();

    m_ClipPlane->SetNormal( m_ClipPlaneNormal.point );
    m_ClipPlane->SetOrigin( 0.0, 0.0, 0.0 );

    vSPNew(clip, vtkClipPolyData);
    clip->SetInputConnection( STLReader->GetOutputPort() );
    clip->SetClipFunction( m_ClipPlane );
    clip->InsideOutOn();

    vSPNew(selectMapper, vtkPolyDataMapper);
    selectMapper->SetInputConnection( clip->GetOutputPort() );

    vSPNew(selectActor, vtkActor);
    selectActor->SetMapper( selectMapper );

    m_Renderer->AddActor( selectActor );
    m_RenderWindow->AddRenderer( m_Renderer );

    ui->qvtkWidget->SetRenderWindow( m_RenderWindow );
}
