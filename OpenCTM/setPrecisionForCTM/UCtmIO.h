#pragma once

#include "openctmpp.h"

#include <vtkPolyData.h>
#include <string>

class CUCtmIO
{
public:
    CUCtmIO();
    ~CUCtmIO();

    bool SaveCtmFile(vtkPolyData *polydata, const std::string fileName);
    bool LoadCtmFile(vtkPolyData *polydata, const std::string fileName);

    void UpdateCompressionMethod(CTMenum aMethod);
    void UpdateVertexPrecision(CTMfloat aPrecision);
protected:
    bool WriteFile(CTMuint aVertCount, CTMuint aTriCount, CTMfloat *aVertices, CTMuint *aIndices, const char *aFileName);
    void PrintInfoOfPolydata(vtkPolyData *polydata);
    void ConvertToValidMesh(vtkPolyData *polydata, const bool &handleScalar = true);

    CTMexporter m_CtmExporter;
    CTMimporter m_CtmImporter;
};
