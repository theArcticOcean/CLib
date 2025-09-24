#include <iostream>
#include <stdio.h>
#include <vtkSmartPointer.h>
#include <vtkSphereSource.h>
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
#include <vtkImplicitPolyDataDistance.h>
#include <vtkVertexGlyphFilter.h>
#include <vtkFloatArray.h>
#include <vtkPointData.h>
#include <vtkColorTransferFunction.h>
#include <vtkLookupTable.h>
#include "UCtmIO.h"  // Add CUCtmIO header file

int main()
{
    freopen("output.txt", "w", stdout);

    // Create CUCtmIO object
    CUCtmIO ctmIO;
    
    // Create vtkPolyData object to store the read data
    vtkSmartPointer<vtkPolyData> polyData = vtkSmartPointer<vtkPolyData>::New();
    
    // Read CTM file
    std::string fileName = "arch_o_u.ctm";
    bool success = ctmIO.LoadCtmFile(polyData, fileName);
    
    if (success) {
        std::cout << "Successfully read CTM file: " << fileName << std::endl;
        std::cout << "Number of points: " << polyData->GetNumberOfPoints() << std::endl;
        std::cout << "Number of faces: " << polyData->GetNumberOfCells() << std::endl;
    } else {
        std::cout << "Failed to read CTM file: " << fileName << std::endl;
        return -1;
    }

    // save the polyData to new ctm file
    std::string newFileName = "arch_o_u_new.ctm";

    ctmIO.UpdateCompressionMethod(CTM_METHOD_MG2);
    ctmIO.UpdateVertexPrecision(0.001);
    success = ctmIO.SaveCtmFile(polyData, newFileName);
    if (success) {
        std::cout << "Successfully saved CTM file: " << newFileName << std::endl;
    }

    vtkSmartPointer<vtkPolyData> newPolyData = vtkSmartPointer<vtkPolyData>::New();
    success = ctmIO.LoadCtmFile(newPolyData, newFileName);
    if( success )
    {
        // Add distances to each point
        vtkSmartPointer<vtkFloatArray> signedDistances = vtkSmartPointer<vtkFloatArray>::New();
        signedDistances->SetNumberOfComponents(1);
        signedDistances->SetName("SignedDistances");

        vtkSmartPointer<vtkImplicitPolyDataDistance> implicitPolyDataDistance = vtkSmartPointer<vtkImplicitPolyDataDistance>::New();
        implicitPolyDataDistance->SetInput( polyData );

        // Evaluate the signed distance function at all of the grid points
        double maxValue = VTK_DOUBLE_MIN, minValue = VTK_DOUBLE_MAX;
        double maxAbsDisValue = 0;
        for( int i = 0; i < newPolyData->GetNumberOfPoints(); i++  )
        {
            double p[3];
            newPolyData->GetPoint( i, p );
            double signedDistance = implicitPolyDataDistance->EvaluateFunction( p );
            signedDistances->InsertNextValue(signedDistance);
            maxValue = std::max( maxValue, signedDistance );
            minValue = std::min( minValue, signedDistance );

            std::cout << i << " : " << signedDistance << std::endl;
            if( std::abs( signedDistance ) > maxAbsDisValue )
                maxAbsDisValue = std::abs( signedDistance );
        }
        std::cout << "maxAbsDisValue: " << maxAbsDisValue << std::endl;
        std::cout << "minValue: " << minValue << std::endl;
        std::cout << "maxValue: " << maxValue << std::endl;

        newPolyData->GetPointData()->SetScalars( signedDistances );

        vtkSmartPointer<vtkVertexGlyphFilter> vertexGlyphFilter = vtkSmartPointer<vtkVertexGlyphFilter>::New();
        vertexGlyphFilter->SetInputData( newPolyData );

        vtkSmartPointer<vtkPolyDataMapper> signedDistanceMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
        signedDistanceMapper->SetInputData( newPolyData );//vertexGlyphFilter->GetOutputPort() );
        signedDistanceMapper->ScalarVisibilityOn();

        // Replace the simple 2-color mapping with multi-level color mapping
        vtkSmartPointer<vtkColorTransferFunction> colorFunction =
                vtkSmartPointer<vtkColorTransferFunction>::New();
        colorFunction->SetUseBelowRangeColor(true);
        colorFunction->SetNanColor(0.5, 0.5, 0.5); // Gray for NaN values
        
        // Create distinct color layers
        double range = maxValue - minValue;
        double step = range / 6.0; // Divide into 6 color bands
        
        // Blue to Cyan to Green to Yellow to Orange to Red
        colorFunction->AddRGBPoint(minValue, 0.0, 0.0, 1.0);                    // Blue
        colorFunction->AddRGBPoint(minValue + step, 0.0, 1.0, 1.0);             // Cyan
        colorFunction->AddRGBPoint(minValue + 2*step, 0.0, 1.0, 0.0);           // Green
        colorFunction->AddRGBPoint(minValue + 3*step, 1.0, 1.0, 0.0);           // Yellow
        colorFunction->AddRGBPoint(minValue + 4*step, 1.0, 0.5, 0.0);           // Orange
        colorFunction->AddRGBPoint(maxValue, 1.0, 0.0, 0.0);                    // Red
        std::cout << "minValue + 3*step: " << (minValue + 3*step) << std::endl;
        colorFunction->Build();


        signedDistanceMapper->SetLookupTable( colorFunction );
        signedDistanceMapper->UseLookupTableScalarRangeOn();

        vtkSmartPointer<vtkActor> signedDistanceActor = vtkSmartPointer<vtkActor>::New();
        signedDistanceActor->SetMapper( signedDistanceMapper );

        vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
        renderer->AddViewProp(signedDistanceActor);

        vtkSmartPointer<vtkRenderWindow> renderWindow =
                vtkSmartPointer<vtkRenderWindow>::New();
        renderWindow->AddRenderer( renderer );

        vtkSmartPointer<vtkRenderWindowInteractor> renWinInteractor =
                vtkSmartPointer<vtkRenderWindowInteractor>::New();
        renWinInteractor->SetRenderWindow( renderWindow );

        renderWindow->Render();
        renWinInteractor->Start();
    }
    
    newFileName = "arch_o_u_new2.ctm";
    success = ctmIO.SaveCtmFile(newPolyData, newFileName);
    if( success  )
    {
        std::cout << "Successfully saved CTM file: " << newFileName << std::endl;
    }
    
    return 0;
}
