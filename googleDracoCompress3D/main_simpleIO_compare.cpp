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
#include <vtkCellArray.h>
#include <vtkPoints.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <memory>
#include <array>

// Include Draco headers
#include "draco/compression/encode.h"
#include "draco/compression/decode.h"
#include "draco/mesh/mesh.h"
#include "draco/point_cloud/point_cloud.h"
#include "draco/attributes/point_attribute.h"
#include "draco/core/encoder_buffer.h"
#include "draco/core/decoder_buffer.h"
#include "draco/io/file_utils.h"

// VTK headers (keeping the original IO functionality)
#include <vtkSmartPointer.h>
#include <vtkSTLReader.h>
#include <vtkPolyData.h>
#include <vtkPoints.h>
#include <vtkCellArray.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkImplicitPolyDataDistance.h>
#include <vtkVertexGlyphFilter.h>
#include <vtkFloatArray.h>
#include <vtkPointData.h>
#include <vtkColorTransferFunction.h>

// Vertex structure for demonstration
struct Vertex {
    float position[3];
};

// Helper function to convert VTK PolyData to Draco Mesh
std::unique_ptr<draco::Mesh> VtkPolyDataToDracoMesh(vtkPolyData* polyData) {
    auto mesh = std::make_unique<draco::Mesh>();
    
    // Get points from VTK PolyData
    vtkPoints* points = polyData->GetPoints();
    if (!points) {
        std::cerr << "No points in PolyData" << std::endl;
        return nullptr;
    }
    
    int numPoints = points->GetNumberOfPoints();
    mesh->set_num_points(numPoints);
    
    // Add position attribute - corrected approach
    draco::GeometryAttribute pos_attribute;
    pos_attribute.Init(draco::GeometryAttribute::POSITION, nullptr, 3, draco::DT_FLOAT32, false, draco::DataTypeLength(draco::DT_FLOAT32) * 3, 0);
    int pos_att_id = mesh->AddAttribute(pos_attribute, true, numPoints);
    if (pos_att_id < 0) {
        std::cerr << "Failed to add position attribute to mesh" << std::endl;
        return nullptr;
    }
    draco::PointAttribute* pos_att = mesh->attribute(pos_att_id);
    
    // Copy point data
    for (int i = 0; i < numPoints; i++) {
        double point[3];
        points->GetPoint(i, point);
        float pos[3] = {static_cast<float>(point[0]), 
                       static_cast<float>(point[1]), 
                       static_cast<float>(point[2])};
        pos_att->SetAttributeValue(draco::AttributeValueIndex(i), pos);
    }
    
    // Get cells (faces) from VTK PolyData
    vtkCellArray* polys = polyData->GetPolys();
    if (polys) {
        vtkIdType npts;
        vtkIdType* pts;
        
        polys->InitTraversal();
        while (polys->GetNextCell(npts, pts)) {
            if (npts == 3) {  // Triangle
                draco::Mesh::Face face;
                face[0] = draco::PointIndex(pts[0]);
                face[1] = draco::PointIndex(pts[1]);
                face[2] = draco::PointIndex(pts[2]);
                mesh->AddFace(face);
            }
        }
    }
    
    return mesh;
}

// Helper function to convert Draco Mesh to VTK PolyData
vtkSmartPointer<vtkPolyData> DracoMeshToVtkPolyData(const draco::Mesh* mesh) {
    auto polyData = vtkSmartPointer<vtkPolyData>::New();
    
    // Create points
    auto points = vtkSmartPointer<vtkPoints>::New();
    points->SetNumberOfPoints(mesh->num_points());
    
    // Get position attribute
    const draco::PointAttribute* pos_att = mesh->GetNamedAttribute(draco::GeometryAttribute::POSITION);
    if (!pos_att) {
        std::cerr << "No position attribute in mesh" << std::endl;
        return nullptr;
    }
    
    // Copy points - 修复索引映射问题
    for (draco::PointIndex i(0); i < mesh->num_points(); ++i) {
        std::array<float, 3> pos;
        // 使用正确的属性值索引映射
        draco::AttributeValueIndex att_val_index = pos_att->mapped_index(i);
        pos_att->GetValue(att_val_index, &pos[0]);
        points->SetPoint(i.value(), pos[0], pos[1], pos[2]);
    }
    polyData->SetPoints(points);
    
    // Create cells (faces)
    auto cells = vtkSmartPointer<vtkCellArray>::New();
    for (draco::FaceIndex i(0); i < mesh->num_faces(); ++i) {
        const draco::Mesh::Face& face = mesh->face(i);
        cells->InsertNextCell(3);
        cells->InsertCellPoint(face[0].value());
        cells->InsertCellPoint(face[1].value());
        cells->InsertCellPoint(face[2].value());
    }
    polyData->SetPolys(cells);
    
    return polyData;
}

// Corrected function to perform lossless Draco compression
bool CompressMeshLossless(const draco::Mesh* mesh, const std::string& filename) {
    draco::Encoder encoder;
    
    // **Key step: Disable quantization for completely lossless compression**
    encoder.SetAttributeQuantization(draco::GeometryAttribute::POSITION, 0);
    // For other attributes (normals/UVs), also set to 0 for lossless
    // encoder.SetAttributeQuantization(draco::GeometryAttribute::NORMAL, 0);
    
    // Set compression level: 0 (fastest) to 10 (best compression)
    encoder.SetSpeedOptions(0, 0); // Both encoding and decoding speed set to slowest (0) for best compression
    
    // Perform encoding
    draco::EncoderBuffer buffer;
    const draco::Status status = encoder.EncodeMeshToBuffer(*mesh, &buffer);
    
    if (!status.ok()) {
        std::printf("Draco encoding failed: %s\n", status.error_msg());
        return false;
    }
    
    // Calculate compression ratio
    size_t original_size = mesh->num_points() * 3 * sizeof(float) + mesh->num_faces() * 3 * sizeof(uint32_t);
    std::printf("Draco lossless compression successful! Original size: %zu bytes, Compressed size: %zu bytes\n", 
               original_size, buffer.size());
    std::printf("Compression ratio: %.2f%%\n", (1.0 - (double)buffer.size() / original_size) * 100.0);
    
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

// Function to load Draco mesh from file
std::unique_ptr<draco::Mesh> LoadDracoMesh(const std::string& filename) {
    // Read file
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
    
    // Decode mesh
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

// Corrected function to perform lossy Draco compression
bool CompressMeshLossy(const draco::Mesh* mesh, const std::string& filename) {
    draco::Encoder encoder;
    
    // Set up encoder for lossy compression with 6 decimal places precision
    // For 6 decimal places precision, we need about 20 quantization bits
    // 2^20 = 1,048,576 which gives us sufficient precision for 6 decimal places
    encoder.SetAttributeQuantization(draco::GeometryAttribute::POSITION, 20);
    // For other attributes (normals/UVs), you can also set quantization
    // encoder.SetAttributeQuantization(draco::GeometryAttribute::NORMAL, 10);
    
    // Set compression level: 0 (fastest) to 10 (best compression)
    encoder.SetSpeedOptions(0, 0); // Both encoding and decoding speed set to slowest (0) for best compression
    
    // Perform encoding
    draco::EncoderBuffer buffer;
    const draco::Status status = encoder.EncodeMeshToBuffer(*mesh, &buffer);
    
    if (!status.ok()) {
        std::printf("Draco encoding failed: %s\n", status.error_msg());
        return false;
    }
    
    // Calculate compression ratio
    size_t original_size = mesh->num_points() * 3 * sizeof(float) + mesh->num_faces() * 3 * sizeof(uint32_t);
    std::printf("Draco lossy compression successful! Original size: %zu bytes, Compressed size: %zu bytes\n", 
               original_size, buffer.size());
    std::printf("Compression ratio: %.2f%%\n", (1.0 - (double)buffer.size() / original_size) * 100.0);
    
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


int main() {
    freopen("output.txt", "w", stdout);

    // Load STL file (keeping original IO process)
    vtkSmartPointer<vtkSTLReader> reader = vtkSmartPointer<vtkSTLReader>::New();
    reader->SetFileName("arch_o_u.stl");
    reader->Update();
    
    vtkSmartPointer<vtkPolyData> polyData = reader->GetOutput();
    
    std::cout << "Original mesh:" << std::endl;
    std::cout << "Number of points: " << polyData->GetNumberOfPoints() << std::endl;
    std::cout << "Number of faces: " << polyData->GetNumberOfCells() << std::endl;
    
    // Convert VTK PolyData to Draco Mesh
    auto dracoMesh = VtkPolyDataToDracoMesh(polyData);
    if (!dracoMesh) {
        std::cerr << "Failed to convert VTK PolyData to Draco Mesh" << std::endl;
        return -1;
    }
    
    std::cout << "Successfully converted to Draco mesh" << std::endl;
    std::cout << "Draco mesh points: " << dracoMesh->num_points() << std::endl;
    std::cout << "Draco mesh faces: " << dracoMesh->num_faces() << std::endl;
    
    // Perform lossless compression
    std::string compressedFileName = "arch_o_u_lossless.drc";
    bool success = CompressMeshLossless(dracoMesh.get(), compressedFileName);

    // // Perform lossy compression with 6 decimal places precision
    // std::string compressedFileName = "arch_o_u_lossy.drc";
    // bool success = CompressMeshLossy(dracoMesh.get(), compressedFileName);
    if (!success) {
        std::cout << "Failed to compress mesh" << std::endl;
        return -1;
    }
    
    // Load the compressed mesh
    auto decompressedMesh = LoadDracoMesh(compressedFileName);
    if (!decompressedMesh) {
        std::cout << "Failed to load compressed mesh: " << compressedFileName << std::endl;
        return -1;
    }
    
    std::cout << "Successfully loaded compressed mesh: " << compressedFileName << std::endl;
    std::cout << "Decompressed mesh points: " << decompressedMesh->num_points() << std::endl;
    std::cout << "Decompressed mesh faces: " << decompressedMesh->num_faces() << std::endl;
    
    // Convert back to VTK PolyData for further processing
    vtkSmartPointer<vtkPolyData> newPolyData = DracoMeshToVtkPolyData(decompressedMesh.get());
    if (!newPolyData) {
        std::cout << "Failed to convert Draco mesh back to VTK PolyData" << std::endl;
        return -1;
    }
    
    // Continue with the original distance calculation and visualization
    vtkSmartPointer<vtkFloatArray> signedDistances = vtkSmartPointer<vtkFloatArray>::New();
    signedDistances->SetNumberOfComponents(1);
    signedDistances->SetName("SignedDistances");

    vtkSmartPointer<vtkImplicitPolyDataDistance> implicitPolyDataDistance = vtkSmartPointer<vtkImplicitPolyDataDistance>::New();
    implicitPolyDataDistance->SetInput(polyData);

    // Evaluate the signed distance function at all grid points
    double maxValue = VTK_DOUBLE_MIN, minValue = VTK_DOUBLE_MAX;
    double maxAbsDisValue = 0;
    for(int i = 0; i < newPolyData->GetNumberOfPoints(); i++) {
        double p[3];
        newPolyData->GetPoint(i, p);
        double signedDistance = implicitPolyDataDistance->EvaluateFunction(p);
        signedDistances->InsertNextValue(signedDistance);
        maxValue = std::max(maxValue, signedDistance);
        minValue = std::min(minValue, signedDistance);

        std::cout << i << " : " << signedDistance << std::endl;
        if(std::abs(signedDistance) > maxAbsDisValue)
            maxAbsDisValue = std::abs(signedDistance);
    }
    std::cout << "maxAbsDisValue: " << maxAbsDisValue << std::endl;
    std::cout << "minValue: " << minValue << std::endl;
    std::cout << "maxValue: " << maxValue << std::endl;

    newPolyData->GetPointData()->SetScalars(signedDistances);

    vtkSmartPointer<vtkVertexGlyphFilter> vertexGlyphFilter = vtkSmartPointer<vtkVertexGlyphFilter>::New();
    vertexGlyphFilter->SetInputData(newPolyData);

    vtkSmartPointer<vtkPolyDataMapper> signedDistanceMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    signedDistanceMapper->SetInputData(newPolyData);
    signedDistanceMapper->ScalarVisibilityOn();

    // Multi-level color mapping
    vtkSmartPointer<vtkColorTransferFunction> colorFunction = vtkSmartPointer<vtkColorTransferFunction>::New();
    colorFunction->SetUseBelowRangeColor(true);
    colorFunction->SetNanColor(0.5, 0.5, 0.5);
    
    double range = maxValue - minValue;
    double step = range / 6.0;
    
    colorFunction->AddRGBPoint(minValue, 0.0, 0.0, 1.0);                    // Blue
    colorFunction->AddRGBPoint(minValue + step, 0.0, 1.0, 1.0);             // Cyan
    colorFunction->AddRGBPoint(minValue + 2*step, 0.0, 1.0, 0.0);           // Green
    colorFunction->AddRGBPoint(minValue + 3*step, 1.0, 1.0, 0.0);           // Yellow
    colorFunction->AddRGBPoint(minValue + 4*step, 1.0, 0.5, 0.0);           // Orange
    colorFunction->AddRGBPoint(maxValue, 1.0, 0.0, 0.0);                    // Red
    std::cout << "minValue + 3*step: " << (minValue + 3*step) << std::endl;
    colorFunction->Build();

    signedDistanceMapper->SetLookupTable(colorFunction);
    signedDistanceMapper->UseLookupTableScalarRangeOn();

    vtkSmartPointer<vtkActor> signedDistanceActor = vtkSmartPointer<vtkActor>::New();
    signedDistanceActor->SetMapper(signedDistanceMapper);

    vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
    renderer->AddViewProp(signedDistanceActor);

    vtkSmartPointer<vtkRenderWindow> renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
    renderWindow->AddRenderer(renderer);

    vtkSmartPointer<vtkRenderWindowInteractor> renWinInteractor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
    renWinInteractor->SetRenderWindow(renderWindow);

    renderWindow->Render();
    renWinInteractor->Start();
    
    // Save the final result
    std::string finalFileName = "arch_o_u_final.drc";
    auto finalDracoMesh = VtkPolyDataToDracoMesh(newPolyData);
    if (finalDracoMesh) {
        success = CompressMeshLossless(finalDracoMesh.get(), finalFileName);
        if (success) {
            std::cout << "Successfully saved final Draco file: " << finalFileName << std::endl;
        }
    }
    
    return 0;
}

