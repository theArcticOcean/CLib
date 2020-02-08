#include "widget.h"
#include "ui_widget.h"

#include <vtkGenericOpenGLRenderWindow.h>
#include <vtkPNGReader.h>
#include <vtkImageActor.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkXMLPolyDataWriter.h>
#include <vtkSTLWriter.h>
#include <vtkTriangleFilter.h>
#include <vtkParametricSpline.h>
#include <vtkCellArray.h>
#include "../share/tool.h"

#include <vtkContourWidget.h>
#include <vtkProperty.h>
#include <vtkOrientedGlyphContourRepresentation.h>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget),
    m_ContourWidget( vtkSPtr<vtkContourWidget>::New() ),
    m_Actor( vtkSPtr<vtkActor>::New() )
{
    ui->setupUi(this);

    vtkObject::GlobalWarningDisplayOn();

    // Override the default representation for the contour widget to customize its look
    vtkSmartPointer<vtkOrientedGlyphContourRepresentation> contourRepresentation =
        vtkSmartPointer<vtkOrientedGlyphContourRepresentation>::New();
    contourRepresentation->GetLinesProperty()->SetColor(1, 0, 0); // Set color to red
    m_ContourWidget->SetRepresentation(contourRepresentation);

    // Generate a set of points arranged in a circle
    int numPts = 10;
    vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
    int offset = numPts / 4;
    for (int i = 0; i < numPts; i++)
    {
        // Create numPts points evenly spread around a circumference of radius 0.1
        const double angle = 2.0*vtkMath::Pi()*(i - offset)/numPts;
        points->InsertPoint(static_cast<vtkIdType>(i), 0.1*cos(angle), 0.1*sin(angle), 0.0 );
    }

    // Create a cell array to connect the points into meaningful geometry
    vtkIdType* vertexIndices = new vtkIdType[numPts];
    for (int i = 0; i < numPts; i++) { vertexIndices[i] = static_cast<vtkIdType>(i); }

    vtkSmartPointer<vtkCellArray> lines = vtkSmartPointer<vtkCellArray>::New();
    lines->InsertNextCell(numPts, vertexIndices);

    // Create polydata to hold the geometry just created, and populate it
    vtkSmartPointer<vtkPolyData> polydata = vtkSmartPointer<vtkPolyData>::New();
    polydata->SetPoints(points);
    polydata->SetLines(lines);

    vtkSmartPointer<vtkRenderer> backRenderer =
            vtkSmartPointer<vtkRenderer>::New();
    backRenderer->SetBackground( 0, 0, 0 );
    backRenderer->AddActor( m_Actor );

    vtkSmartPointer<vtkGenericOpenGLRenderWindow> backWindow =
            vtkSmartPointer<vtkGenericOpenGLRenderWindow>::New();
    backWindow->AddRenderer( backRenderer );

    ui->qvtkWidget->SetRenderWindow( backWindow );
    backRenderer->ResetCamera();
    backWindow->Render();

    // Set up the contour widget within the visualization pipeline just assembled
    m_ContourWidget->SetInteractor( ui->qvtkWidget->GetInteractor() );
    m_ContourWidget->On(); // Turn on the interactor observer
    m_ContourWidget->Initialize(polydata);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_OutputButton_clicked()
{
    vtkContourRepresentation *rep = m_ContourWidget->GetContourRepresentation();
    rep->SetClosedLoop( false );
    vtkSPtrNew( polydata , vtkPolyData );
    rep->GetNodePolyData( polydata );

    vtkSmartPointer<vtkParametricSpline> spline =
        vtkSmartPointer<vtkParametricSpline>::New();
    spline->SetPoints( polydata->GetPoints() );

    vtkSmartPointer<vtkPoints> betaPoints = vtkSmartPointer<vtkPoints>::New();
    int index = 0;
    int countOfDeltaPoints = 5000;
    double step = 1.0 / ( countOfDeltaPoints - 1 );
    for( double i = 0; i <= 1; i = i + step )
    {
        double tmp[3] = { i, 0, 0 };
        spline->Evaluate( tmp, tmp, NULL );
        betaPoints->InsertPoint( index++, tmp );
    }

    vtkSPtrNew( splinePd, vtkPolyData );
    splinePd->SetPoints( betaPoints );
    vtkSPtrNew( splineCells, vtkCellArray );
    for( int i = 0; i < betaPoints->GetNumberOfPoints()-1; ++i )
    {
        vtkIdType ids[2] = { i, i + 1 };
        splineCells->InsertNextCell( 2, ids );
    }
    splinePd->SetLines( splineCells );
    splinePd->BuildLinks();

    vtkSPtrNew( mapper, vtkPolyDataMapper );
    mapper->SetInputData( splinePd );
    m_Actor->SetMapper( mapper );
    m_Actor->GetProperty()->SetColor( 1, 0, 0  );

    //rep->SetVisibility( false );
    m_ContourWidget->EnabledOff();

    vtkSmartPointer<vtkXMLPolyDataWriter> writer =
      vtkSmartPointer<vtkXMLPolyDataWriter>::New();
    writer->SetFileName( "/Users/weiyang/Desktop/curve.vtp" );
    writer->SetInputData( splinePd );
    writer->Write();

    ui->qvtkWidget->GetRenderWindow()->Render();
}
