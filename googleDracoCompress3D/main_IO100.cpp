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
#include <vtkSTLWriter.h>
#include <vtkTriangleFilter.h>
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
#include "draco/compression/encode.h"
#include "draco/compression/decode.h"
#include "draco/mesh/mesh.h"
#include "draco/point_cloud/point_cloud.h"
#include "draco/attributes/geometry_attribute.h"
#include "draco/core/vector_d.h"

// Convert VTK PolyData to Draco Mesh
std::unique_ptr<draco::Mesh> VtkPolyDataToDracoMesh(vtkPolyData* polyData) {
    if (!polyData) {
        std::cerr << "Input polyData is null" << std::endl;
        return nullptr;
    }
    
    auto mesh = std::make_unique<draco::Mesh>();
    
    // Add position attribute
    draco::GeometryAttribute pos_att;
    pos_att.Init(draco::GeometryAttribute::POSITION, nullptr, 3, draco::DT_FLOAT32, false, 
                 sizeof(float) * 3, 0);
    int pos_att_id = mesh->AddAttribute(pos_att, true, polyData->GetNumberOfPoints());
    if (pos_att_id < 0) {
        std::cerr << "Failed to add position attribute" << std::endl;
        return nullptr;
    }
    
    // Set vertex positions
    for (vtkIdType i = 0; i < polyData->GetNumberOfPoints(); i++) {
        double point[3];
        polyData->GetPoint(i, point);
        
        float pos[3] = {static_cast<float>(point[0]), 
                       static_cast<float>(point[1]), 
                       static_cast<float>(point[2])};
        
        mesh->attribute(pos_att_id)->SetAttributeValue(
            draco::AttributeValueIndex(i), pos);
    }
    
    // Add faces
    vtkCellArray* polys = polyData->GetPolys();
    vtkIdType npts;
    vtkIdType* pts;
    
    polys->InitTraversal();
    while (polys->GetNextCell(npts, pts)) {
        if (npts == 3) {
            draco::Mesh::Face face;
            face[0] = pts[0];
            face[1] = pts[1];
            face[2] = pts[2];
            mesh->AddFace(face);
        }
    }
    
    return mesh;
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

// Compress mesh using Draco with lossy compression
bool CompressMeshLossy(const draco::Mesh* mesh, const std::string& filename) {
    draco::Encoder encoder;
    
    // Set up encoder for lossy compression with 6 decimal places precision
    encoder.SetAttributeQuantization(draco::GeometryAttribute::POSITION, 20);
    encoder.SetSpeedOptions(0, 0); // Best compression
    
    draco::EncoderBuffer buffer;
    const draco::Status status = encoder.EncodeMeshToBuffer(*mesh, &buffer);
    
    if (!status.ok()) {
        std::printf("Draco encoding failed: %s\n", status.error_msg());
        return false;
    }
    
    // Save to file
    std::ofstream file(filename, std::ios::binary);
    if (!file) {
        std::cerr << "Failed to open file for writing: " << filename << std::endl;
        return false;
    }
    
    file.write(buffer.data(), buffer.size());
    file.close();
    
    return true;
}

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

// Function to perform one IO operation using Draco
bool performSingleIO(const std::string& inputFileName, const std::string& outputFileName) {
    // Load Draco mesh from file
    auto dracoMesh = LoadDracoMesh(inputFileName);
    if (!dracoMesh) {
        std::cout << "Failed to load Draco mesh: " << inputFileName << std::endl;
        return false;
    }
    
    std::cout << "Successfully loaded Draco mesh: " << inputFileName << std::endl;
    std::cout << "Number of points: " << dracoMesh->num_points() << std::endl;
    std::cout << "Number of faces: " << dracoMesh->num_faces() << std::endl;
    
    // Save with lossy compression
    bool success = CompressMeshLossy(dracoMesh.get(), outputFileName);
    
    if (success) {
        std::cout << "Successfully saved Draco mesh: " << outputFileName << std::endl;
    } else {
        std::cout << "Failed to save Draco mesh: " << outputFileName << std::endl;
        return false;
    }
    
    return true;
}

int main() {
    freopen("output.txt", "w", stdout);
    
    // First, we need to create the initial .drc file from STL
    // Load STL file
    vtkSmartPointer<vtkSTLReader> reader = vtkSmartPointer<vtkSTLReader>::New();
    reader->SetFileName("arch_o_u.stl");
    reader->Update();
    
    vtkSmartPointer<vtkPolyData> polyData = reader->GetOutput();
    
    // Convert to Draco mesh and save as initial file
    auto dracoMesh = VtkPolyDataToDracoMesh(polyData);
    if (!dracoMesh) {
        std::cerr << "Failed to convert STL to Draco mesh" << std::endl;
        return -1;
    }
    
    std::string initialFileName = "arch_o_u.drc";
    bool success = CompressMeshLossy(dracoMesh.get(), initialFileName);
    if (!success) {
        std::cout << "Failed to create initial Draco file" << std::endl;
        return -1;
    }
    
    std::cout << "Created initial Draco file: " << initialFileName << std::endl;
    
    // Current file name for iterations
    std::string currentFileName = initialFileName;
    
    // Perform 100 IO operations
    for (int i = 1; i <= 100; i++) {
        std::string newFileName = "arch_o_u_new" + std::to_string(i) + ".drc";
        
        std::cout << "\n=== IO Operation " << i << " ===" << std::endl;
        
        bool success = performSingleIO(currentFileName, newFileName);
        
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
