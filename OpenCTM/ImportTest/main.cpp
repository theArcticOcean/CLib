#include <iostream>
#include <vtkSmartPointer.h>
#include <vtkActor.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkPolyDataMapper.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkPolyData.h>
#include <string>
#include <time.h>

#include "openctmpp.h"
#include "tool.h"

using namespace std;

void saveIdCoordinate( vtkPolyData *polyData  )
{
    setbuf( stdout, nullptr );
    printf( "saveIdCoordinate\n" );
    if( freopen("info.txt", "w", stdout) )
    {
        vtkPoints *pts = polyData->GetPoints();
        for( int i = 0; i < pts->GetNumberOfPoints(); ++i )
        {
            PointStruct pt( pts->GetPoint( i ) );
            printf( "id: %d, (%lf, %lf, %lf)\n", i, pt[0], pt[1], pt[2] );
        }
        printf( "GetNumberOfCells: %d\n", polyData->GetNumberOfCells() );
        fclose( stdout );
    }
}

int main()
{
    vtkSmartPointer<vtkPolyData> polyData =
            vtkSmartPointer<vtkPolyData>::New();
    vtkSmartPointer<vtkPoints> points =
            vtkSmartPointer<vtkPoints>::New();
    vtkSmartPointer<vtkCellArray> cells =
            vtkSmartPointer<vtkCellArray>::New();
    //string file = "/Users/weiyang/pro/CLib/OpenCTM/ExportTest/qt-ExportTest-Default/cone.ctm";
    string file = "/Users/weiyang/pro/CLib/OpenCTM/Compression/qt-Compression-Default/LingerBar2.ctm";

    clock_t beginT = clock();
    try {
        CTMimporter ctm;
        ctm.Load( file.c_str() );

        CTMuint vertCount = ctm.GetInteger(CTM_VERTEX_COUNT);
        const CTMfloat *vertices = ctm.GetFloatArray(CTM_VERTICES);
        CTMuint triCount = ctm.GetInteger(CTM_TRIANGLE_COUNT);
        const CTMuint *indices = ctm.GetIntegerArray(CTM_INDICES);

        for( CTMuint i = 0; i < vertCount; ++i )
        {
            PointStruct pt;
            for( int j = 0; j < 3; ++j )
            {
                pt[j] = vertices[3*i+j];
            }
            points->InsertNextPoint( pt.point );
        }
        for( CTMuint i = 0; i < triCount; ++i )
        {
            vtkIdType cell[3];
            for( int j = 0; j < 3; ++j )
            {
                cell[j] = indices[3*i+j];
            }
            cells->InsertNextCell( 3, cell );
        }
        polyData->SetPoints( points );
        polyData->SetPolys( cells );
    }
    catch(exception &e)
    {
        fprintf( stderr, "[%s, %d]: Error => %s\n", __FILE__, __LINE__, e.what( ) );
    }
    clock_t endT = clock();
    double duration = 1.0 * (endT - beginT) / CLOCKS_PER_SEC;
    cout << "ctm read duration: " << duration << endl;

    saveIdCoordinate( polyData.Get() );

    vtkSmartPointer<vtkPolyDataMapper> polyDataMapper =
            vtkSmartPointer<vtkPolyDataMapper>::New();
    polyDataMapper->SetInputData( polyData );
    vtkSmartPointer<vtkActor> actor =
            vtkSmartPointer<vtkActor>::New();
    actor->SetMapper( polyDataMapper );

    vtkSmartPointer<vtkRenderer> renderer =
            vtkSmartPointer<vtkRenderer>::New();
    renderer->AddActor(actor);
    renderer->SetBackground( 0, 0, 0 );

    vtkSmartPointer<vtkRenderWindow> renderWindow =
            vtkSmartPointer<vtkRenderWindow>::New();
    renderWindow->AddRenderer( renderer );

    vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor =
            vtkSmartPointer<vtkRenderWindowInteractor>::New();
    renderWindowInteractor->SetRenderWindow( renderWindow );

    renderer->ResetCamera();
    renderWindow->Render();
    renderWindowInteractor->Start();
    return 0;
}
/*
ctm read duration: 0.202797
*/
