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
#include <vtkSTLReader.h>
#include <vtkTriangleFilter.h>
#include <vtkSTLWriter.h>
#include <vtkImplicitPolyDataDistance.h>
#include <vtkVertexGlyphFilter.h>
#include <vtkFloatArray.h>
#include <vtkPointData.h>
#include <vtkColorTransferFunction.h>
#include <vtkLookupTable.h>
#include <vtkCellArray.h>
#include <vtkPoints.h>
#include <string>
#include <iostream>
#include <fstream>
#include <memory>
#include <vector>

// Draco includes
#include "draco/compression/decode.h"
#include "draco/mesh/mesh.h"
#include "draco/point_cloud/point_cloud.h"
#include "draco/attributes/geometry_attribute.h"
#include "draco/core/vector_d.h"

// Load Draco mesh from file
std::unique_ptr<draco::Mesh> LoadDracoMesh(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        std::cerr << "Failed to open file for reading: " << filename << std::endl;
        return nullptr;
    }
    
    file.seekg(0, std::ios::end);
    size_t file_size = file.tellg();
    file.seekg(0, std::ios::beg);
    
    std::vector<char> data(file_size);
    file.read(data.data(), file_size);
    file.close();
    
    draco::DecoderBuffer buffer;
    buffer.Init(data.data(), data.size());
    
    draco::Decoder decoder;
    auto statusor = decoder.DecodeMeshFromBuffer(&buffer);
    
    if (!statusor.ok()) {
        std::cerr << "Failed to decode mesh: " << statusor.status().error_msg() << std::endl;
        return nullptr;
    }
    
    return std::move(statusor).value();
}

// Convert Draco Mesh to VTK PolyData
vtkSmartPointer<vtkPolyData> DracoMeshToVtkPolyData(const draco::Mesh* mesh) {
    if (!mesh) {
        std::cerr << "Input mesh is null" << std::endl;
        return nullptr;
    }
    
    vtkSmartPointer<vtkPolyData> polyData = vtkSmartPointer<vtkPolyData>::New();
    vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
    vtkSmartPointer<vtkCellArray> polys = vtkSmartPointer<vtkCellArray>::New();
    
    // Get position attribute
    const draco::PointAttribute* pos_att = mesh->GetNamedAttribute(draco::GeometryAttribute::POSITION);
    if (!pos_att) {
        std::cerr << "No position attribute found" << std::endl;
        return nullptr;
    }
    
    // Add vertices
    for (draco::PointIndex i(0); i < mesh->num_points(); ++i) {
        draco::AttributeValueIndex pos_index = pos_att->mapped_index(i);
        float pos[3];
        pos_att->GetValue(pos_index, pos);
        points->InsertNextPoint(pos[0], pos[1], pos[2]);
    }
    
    // Add faces
    for (draco::FaceIndex i(0); i < mesh->num_faces(); ++i) {
        const draco::Mesh::Face& face = mesh->face(i);
        polys->InsertNextCell(3);
        polys->InsertCellPoint(face[0].value());
        polys->InsertCellPoint(face[1].value());
        polys->InsertCellPoint(face[2].value());
    }
    
    polyData->SetPoints(points);
    polyData->SetPolys(polys);
    
    return polyData;
}

int main(int argc, char* argv[])
{
    freopen("output.txt", "w", stdout);

    // Load original STL file
    vtkSmartPointer<vtkSTLReader> stlReader = vtkSmartPointer<vtkSTLReader>::New();
    stlReader->SetFileName("arch_o_u.stl");
    stlReader->Update();
    
    vtkSmartPointer<vtkPolyData> polyData = stlReader->GetOutput();
    
    if (polyData && polyData->GetNumberOfPoints() > 0) {
        std::cout << "Successfully read STL file: arch_o_u.stl" << std::endl;
        std::cout << "Number of points: " << polyData->GetNumberOfPoints() << std::endl;
        std::cout << "Number of faces: " << polyData->GetNumberOfCells() << std::endl;
    } else {
        std::cout << "Failed to read STL file: arch_o_u.stl" << std::endl;
        return -1;
    }

    // Load compressed Draco file
    auto dracoMesh = LoadDracoMesh("arch_o_u_new1.drc");
    if (!dracoMesh) {
        std::cout << "Failed to load Draco mesh: arch_o_u_new100.drc" << std::endl;
        return -1;
    }
    
    // Convert Draco mesh to VTK PolyData
    vtkSmartPointer<vtkPolyData> newPolyData = DracoMeshToVtkPolyData(dracoMesh.get());
    if (!newPolyData) {
        std::cout << "Failed to convert Draco mesh to VTK PolyData" << std::endl;
        return -1;
    }
    
    std::cout << "Successfully loaded Draco file: arch_o_u_new100.drc" << std::endl;
    std::cout << "Number of points: " << newPolyData->GetNumberOfPoints() << std::endl;
    std::cout << "Number of faces: " << newPolyData->GetNumberOfCells() << std::endl;

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

    vtkSmartPointer<vtkPolyDataMapper> signedDistanceMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    signedDistanceMapper->SetInputData( newPolyData );
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
    
    return 0;
}
