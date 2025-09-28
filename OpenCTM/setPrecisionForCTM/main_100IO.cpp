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

// Function to perform one IO operation
bool performSingleIO(CUCtmIO& ctmIO, const std::string& inputFileName, const std::string& outputFileName) {
    // Create vtkPolyData object to store the read data
    vtkSmartPointer<vtkPolyData> polyData = vtkSmartPointer<vtkPolyData>::New();
    
    // Read CTM file
    bool success = ctmIO.LoadCtmFile(polyData, inputFileName);
    
    if (success) {
        std::cout << "Successfully read CTM file: " << inputFileName << std::endl;
        std::cout << "Number of points: " << polyData->GetNumberOfPoints() << std::endl;
        std::cout << "Number of faces: " << polyData->GetNumberOfCells() << std::endl;
    } else {
        std::cout << "Failed to read CTM file: " << inputFileName << std::endl;
        return false;
    }

    // Save the polyData to new ctm file
    ctmIO.UpdateCompressionMethod(CTM_METHOD_MG2);
    ctmIO.UpdateVertexPrecision(0.001);
    success = ctmIO.SaveCtmFile(polyData, outputFileName);
    
    if (success) {
        std::cout << "Successfully saved CTM file: " << outputFileName << std::endl;
    } else {
        std::cout << "Failed to save CTM file: " << outputFileName << std::endl;
        return false;
    }
    
    return true;
}

int main()
{
    freopen("output.txt", "w", stdout);

    // Create CUCtmIO object
    CUCtmIO ctmIO;
    
    // Initial file name
    std::string currentFileName = "arch_o_u.ctm";
    
    // Perform 100 IO operations
    for (int i = 1; i <= 100; i++) {
        std::string newFileName = "arch_o_u_new" + std::to_string(i) + ".ctm";
        
        std::cout << "\n=== IO Operation " << i << " ===" << std::endl;
        
        bool success = performSingleIO(ctmIO, currentFileName, newFileName);
        
        if (!success) {
            std::cout << "IO operation " << i << " failed!" << std::endl;
            return -1;
        }
        
        // Update current file name for next iteration
        currentFileName = newFileName;
        
        std::cout << "IO operation " << i << " completed successfully." << std::endl;
    }
    
    std::cout << "\nAll 100 IO operations completed successfully!" << std::endl;
    std::cout << "Final output file: " << currentFileName << std::endl;
    
    return 0;
}
