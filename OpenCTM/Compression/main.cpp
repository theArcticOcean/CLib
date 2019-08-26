#include <iostream>
#include <stdio.h>
#include <vtkSmartPointer.h>
#include <vtkSTLReader.h>
#include <vtkActor.h>
#include <vtkConeSource.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkCamera.h>
#include <vtkPolyData.h>
#include <string>
#include <vtkTriangleFilter.h>
#include <vtkSTLWriter.h>
#include <time.h>

#include "tool.h"
#include "openctmpp.h"

using namespace std;

void MySaveFile(CTMuint aVertCount, CTMuint aTriCount, CTMfloat * aVertices,
   CTMuint * aIndices, const char * aFileName)
{
    try
    {
        // Create a new OpenCTM exporter object
        CTMexporter ctm;

        // Define our mesh representation to OpenCTM (store references to it in
        // the context)
        ctm.DefineMesh(aVertices, aVertCount, aIndices, aTriCount, NULL);

//        ctm.CompressionMethod( CTM_METHOD_MG2 );
//        ctm.CompressionLevel( 9 );
//        ctm.VertexPrecision( 0.001 );

        // Save the OpenCTM file
        ctm.Save(aFileName);
    }
    catch(exception &e)
    {
        fprintf( stderr, "[%s, %d]: Error => %s\n", __FILE__, __LINE__, e.what( ) );
    }
}

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
    clock_t beginT = clock();
    string file = "/Users/weiyang/Desktop/LingerBar.stl";
    vtkSmartPointer<vtkSTLReader> reader =
            vtkSmartPointer<vtkSTLReader>::New();
    reader->SetFileName( file.c_str() );
    reader->Update();
    clock_t endT = clock();
    double duration = 1.0 * (endT - beginT) / CLOCKS_PER_SEC;
    cout << "stl read duration: " << duration << endl;

    beginT = clock();
    vtkSmartPointer<vtkSTLWriter> writer = vtkSmartPointer<vtkSTLWriter>::New();
    writer->SetFileName( "./LingerBar2.stl" );
    writer->SetInputData( reader->GetOutput() );
    writer->Update();
    writer->Write();
    endT = clock();
    duration = 1.0 * (endT - beginT) / CLOCKS_PER_SEC;
    cout << "stl write duration: " << duration << endl;

    beginT = clock();
    // -------------- OpenCTM save data ---------------
    vtkSmartPointer<vtkTriangleFilter> triangle =
            vtkSmartPointer<vtkTriangleFilter>::New();
    triangle->SetInputData( reader->GetOutput() );
    triangle->Update();

    vtkPolyData *pd = triangle->GetOutput();
    vtkPoints *points = pd->GetPoints();

    //saveIdCoordinate( pd );

    CTMuint aVertCount = static_cast<unsigned int>( points->GetNumberOfPoints() );
    CTMfloat *aVertices = new CTMfloat[3*aVertCount];
    for( CTMuint i = 0; i < aVertCount; ++i )
    {
        PointStruct pt( points->GetPoint( i ) );
        for( CTMuint j = 0; j < 3; ++j )
        {
            aVertices[3*i+j] = static_cast<CTMfloat>( pt[j] );
        }
    }
    CTMuint aTriCount = static_cast<CTMuint>( pd->GetNumberOfCells() );
    CTMuint *aIndices = new CTMuint[3*aTriCount];
    for( CTMuint i = 0; i < aTriCount; ++i )
    {
        vtkIdList *ids = pd->GetCell( i )->GetPointIds();
        for( CTMuint j = 0; j < 3; ++j )
        {
            aIndices[3*i+j] = static_cast<CTMuint>( ids->GetId( j ) );
        }
    }
    string fileName = "./LingerBar2.ctm";
    MySaveFile( aVertCount, aTriCount, aVertices, aIndices, fileName.c_str() );

    delete [] aVertices;
    aVertices = nullptr;
    delete [] aIndices;
    aIndices = nullptr;
    // -------------- Finish: OpenCTM save data ---------------
    endT = clock();
    duration = 1.0 * (endT - beginT) / CLOCKS_PER_SEC;
    cout << "ctm write duration: " << duration << endl; //ctm duration: 1.45066

    vtkSmartPointer<vtkPolyDataMapper> mapper =
            vtkSmartPointer<vtkPolyDataMapper>::New();
    mapper->SetInputConnection( reader->GetOutputPort() );

    vtkSmartPointer<vtkActor> actor =
            vtkSmartPointer<vtkActor>::New();
    actor->SetMapper( mapper );

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
stl read duration: 0.500009
stl write duration: 2.37959
ctm write duration: 1.44125
*/
