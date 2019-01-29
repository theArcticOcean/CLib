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

        ctm.CompressionMethod( CTM_METHOD_MG2 );
        ctm.CompressionLevel( 9 );
        ctm.VertexPrecision( 0.001 );

        // Save the OpenCTM file
        ctm.Save(aFileName);
    }
    catch(exception &e)
    {
        fprintf( stderr, "[%s, %d]: Error => %s\n", __FILE__, __LINE__, e.what( ) );
    }
}

int main()
{
    string file = "/Users/weiyang/Desktop/LingerBar.stl";
    vtkSmartPointer<vtkSTLReader> reader =
            vtkSmartPointer<vtkSTLReader>::New();
    reader->SetFileName( file.c_str() );
    reader->Update();

    // -------------- OpenCTM save data ---------------
    vtkSmartPointer<vtkTriangleFilter> triangle =
            vtkSmartPointer<vtkTriangleFilter>::New();
    triangle->SetInputData( reader->GetOutput() );
    triangle->Update();

    vtkPolyData *pd = triangle->GetOutput();
    vtkPoints *points = pd->GetPoints();

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
