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

    newFileName = "arch_o_u_new2.ctm";
    success = ctmIO.SaveCtmFile(newPolyData, newFileName);

    vtkSmartPointer<vtkPolyData> newPolyData2 = vtkSmartPointer<vtkPolyData>::New();
    success = ctmIO.LoadCtmFile(newPolyData2, newFileName);
    if (!success) {
        std::cout << "Failed to read CTM file: " << newFileName << std::endl;
        return -1;
    }

    newFileName = "arch_o_u_new3.ctm";
    success = ctmIO.SaveCtmFile(newPolyData2, newFileName);
    
    vtkSmartPointer<vtkPolyData> newPolyData3 = vtkSmartPointer<vtkPolyData>::New();
    success = ctmIO.LoadCtmFile(newPolyData3, newFileName);
    if (!success) {
        std::cout << "Failed to read CTM file: " << newFileName << std::endl;
        return -1;
    }

    newFileName = "arch_o_u_new4.ctm";
    success = ctmIO.SaveCtmFile(newPolyData3, newFileName);

    vtkSmartPointer<vtkPolyData> newPolyData4 = vtkSmartPointer<vtkPolyData>::New();
    success = ctmIO.LoadCtmFile(newPolyData4, newFileName);
    if (!success) {
        std::cout << "Failed to read CTM file: " << newFileName << std::endl;
        return -1;
    }

    newFileName = "arch_o_u_new5.ctm";
    success = ctmIO.SaveCtmFile(newPolyData4, newFileName);

    // ==== test original function ====
    vtkSmartPointer<vtkPolyData> newPolyData5 = vtkSmartPointer<vtkPolyData>::New();
    success = ctmIO.LoadCtmFile(newPolyData5, newFileName);
    if (!success) {
        std::cout << "Failed to read CTM file: " << newFileName << std::endl;
        return -1;
    }

    newFileName = "arch_o_u_new6.ctm";
    ctmIO.UpdateCompressionMethod(CTM_METHOD_MG1);
    success = ctmIO.SaveCtmFile(newPolyData5, newFileName);
    if (!success) {
        std::cout << "Failed to read CTM file: " << newFileName << std::endl;
        return -1;
    }
    return 0;
}
