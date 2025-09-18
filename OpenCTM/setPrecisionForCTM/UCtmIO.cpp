#include "UCtmIO.h"
#include "ULog.h"
#include "PointStruct.hpp"
#include "UCommonIO.h"
#include "UMacroDefinition.h"
#include "UCommonIO.h"

#include <vtkPoints.h>
#include <vtkPointData.h>
#include <vtkCellArray.h>
#include <vector>

CUCtmIO::CUCtmIO()
{

}

CUCtmIO::~CUCtmIO()
{

}


/*
*   Public interface: it's used to compress polydata to ctm file.
*/
bool CUCtmIO::SaveCtmFile(vtkPolyData *polydata, const std::string fileName)
{
    LOG( NOTICE, "fileName: ", fileName );

    if( nullptr == polydata ||
        0 == polydata->GetNumberOfPoints() ||
        0 == polydata->GetNumberOfCells() )
    {
        LOG( ERR, "Invalid polydata, save nothing" );
        return false;
    }
    vtkPoints *points = polydata->GetPoints();
    POINTER_CHECK_WITHBOOL( points );
    if( strstr(fileName.c_str(), "tooth") != nullptr || strstr(fileName.c_str(), "gum") != nullptr )
    {
        ConvertToValidMesh( polydata );
    }
    CTMuint aVertCount = static_cast<unsigned int>( points->GetNumberOfPoints() );
    CTMfloat *aVertices = new CTMfloat[3*aVertCount];
    LOG( NOTICE, "GetNumberOfPoints: ", polydata->GetNumberOfPoints() );
    for( CTMuint i = 0; i < aVertCount; ++i )
    {
        PointStruct pt( points->GetPoint( i ) );
        for( CTMuint j = 0; j < 3; ++j )
        {
            aVertices[3*i+j] = static_cast<CTMfloat>( pt[j] );
        }
    }
    CTMuint aTriCount = static_cast<CTMuint>( polydata->GetNumberOfCells() );
    LOG( NOTICE, "fileName: ", fileName, ", count of cells: ", aTriCount );
    CTMuint *aIndices = new CTMuint[3*aTriCount];
    for( CTMuint i = 0; i < aTriCount; ++i )
    {
        vtkIdList *ids = polydata->GetCell( i )->GetPointIds();
        for( CTMuint j = 0; j < 3; ++j )
        {
            aIndices[3*i+j] = static_cast<CTMuint>( ids->GetId( j ) );
        }
    }

    bool result = WriteFile( aVertCount, aTriCount, aVertices, aIndices, fileName.c_str());
    if( !result )
    {
        PrintInfoOfPolydata( polydata );
    }

    delete [] aVertices;
    aVertices = nullptr;
    delete [] aIndices;
    aIndices = nullptr;

    return result;
}

bool CUCtmIO::LoadCtmFile(vtkPolyData *polydata, const std::string fileName)
{
    if( nullptr == polydata || "" == fileName || !UCommonIO::FileExists( fileName ) )
    {
        LOG( ERR, "invalid input data: ", fileName );
        return false;
    }
    vSPNew( points, vtkPoints );
    vSPNew( cells, vtkCellArray );
    try
    {
        m_CtmImporter.Load(fileName.c_str());
        CTMuint vertCount = m_CtmImporter.GetInteger( CTM_VERTEX_COUNT );
        const CTMfloat *vertices = m_CtmImporter.GetFloatArray( CTM_VERTICES );
        CTMuint triCount = m_CtmImporter.GetInteger( CTM_TRIANGLE_COUNT );
        const CTMuint *indices = m_CtmImporter.GetIntegerArray( CTM_INDICES );
        LOG( NOTICE, "triCount: ", triCount );
        for( CTMuint i = 0; i < vertCount; ++i )
        {
            PointStruct pt;
            for( int j = 0; j < 3; ++j )
            {
                pt[j] = vertices[3*i+j];
            }
            points->InsertNextPoint( pt.GetPoint());
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
        polydata->Initialize();
        polydata->SetPoints( points );
        polydata->SetPolys( cells );
        polydata->BuildCells();
        polydata->BuildLinks();

        LOG( NOTICE, "successfully, cells->GetNumberOfCells: ", cells->GetNumberOfCells(),
             ", polydata->GetNumberOfCells: ", polydata->GetNumberOfCells() );
    }
    catch( std::exception &e )
    {
        LOG( ERR, "Error => ", e.what() );
        return false;
    }
    return true;
}

bool CUCtmIO::WriteFile(CTMuint aVertCount, CTMuint aTriCount, CTMfloat *aVertices, CTMuint *aIndices, const char *aFileName)
{
    try
    {
        // Define our mesh representation to OpenCTM (store references to it in
        // the context)
        m_CtmExporter.DefineMesh( aVertices, aVertCount, aIndices, aTriCount, NULL );

        // Save the OpenCTM file
        m_CtmExporter.Save( aFileName );
    }
    catch( std::exception &e )
    {
        LOG( ERR, "Error => ", e.what() );
        return false;
    }
    return true;
}

void CUCtmIO::UpdateCompressionMethod(CTMenum aMethod)
{
    try {
        m_CtmExporter.CompressionMethod( aMethod );
    } catch( std::exception &e ) {
        LOG( ERR, "Error => ", e.what() );
    }
}

void CUCtmIO::UpdateVertexPrecision(CTMfloat aPrecision)
{
    try {
        m_CtmExporter.VertexPrecision( aPrecision );
    } catch( std::exception &e ) {
        LOG( ERR, "Error => ", e.what() );
    }
}

void CUCtmIO::PrintInfoOfPolydata(vtkPolyData *polydata)
{
    POINTER_CHECK( polydata );
    vtkPoints *points = polydata->GetPoints();
    POINTER_CHECK( points );
    vtkIdType numberOfPts = points->GetNumberOfPoints();
    vtkIdType numberOfCells = polydata->GetNumberOfCells();

    // points
    for( int i = 0; i < numberOfPts; ++i )
    {
        PointStruct pt( points->GetPoint( i ) );
        for( int j = 0; j < 3; ++j )
        {
#ifdef __linux__
            if (!std::isfinite(pt[j]))
#else
            if (!isfinite(pt[j]))
#endif // __linux__
            {
                LOG( ERR, "#==> Invalid pt: ", i, " , ", pt[0], ", ", pt[1], ", ", pt[2] );
            }
        }
    }

    // polys
    if( numberOfCells < 1LL )
    {
        LOG( ERR, "#==> Invalid numberOfCells: ", numberOfCells );
    }
    for( int i = 0; i < numberOfCells; ++i )
    {
        vtkIdList *ptIds = vtkIdList::New();
        polydata->GetCellPoints( i, ptIds );
        if( ptIds->GetNumberOfIds() != 3 )
        {
            LOG( ERR, "Cell id: ", i, ", the number of ids: ", ptIds->GetNumberOfIds() );
        }
        for( int j = 0; j < ptIds->GetNumberOfIds(); ++j )
        {
            if( ptIds->GetId(j) >= numberOfPts || ptIds->GetId(j) < 0 )
            {
                LOG( ERR, "#==> Invalid ptId: ", ptIds->GetId(j) );
            }
        }
        ptIds->Delete();
    }
}

void CUCtmIO::ConvertToValidMesh(vtkPolyData *polydata, const bool &handleScalar)
{
    POINTER_CHECK( polydata );
    vtkPoints *points = polydata->GetPoints();
    POINTER_CHECK( points );
    vtkIdType numberOfPts = points->GetNumberOfPoints();
    vtkIdType numberOfCells = polydata->GetNumberOfCells();
    int pointCountDiff = 0;
    LOG( INFO, "numberOfCells: ", polydata->GetNumberOfCells() );
    std::vector<bool> needPts;
    for( int i = 0; i < numberOfPts; ++i )
    {
        needPts.push_back( false );
    }
    bool hasBadCell = false;
    // polys
    for( int i = 0; i < numberOfCells; ++i )
    {
       vtkIdList *ptIds = vtkIdList::New();
       polydata->GetCellPoints( i, ptIds );
       if( ptIds->GetNumberOfIds() != 3 )
       {
           LOG( INFO, "delete Invalid Cell, id: ", i, ", GetNumberOfIds: ", ptIds->GetNumberOfIds() );
           polydata->DeleteCell( i );   // the number of ids: mark empty_cell
           hasBadCell = true;
       }
       else
       {
           vtkIdType nPoints;
#if (VTK_MAJOR_VERSION <8 || ((VTK_MAJOR_VERSION == 8) && (VTK_MINOR_VERSION < 90)))
           vtkIdType* pts;
#else
           const vtkIdType* pts;
#endif
           polydata->GetCellPoints( i, nPoints, pts );
           for( int j = 0; j < nPoints; ++j )
           {
               vtkIdType pointId = pts[j];
               needPts[pointId] = true;
           }
       }
       ptIds->Delete();
    }

    if( hasBadCell )
    {
       vtkIntArray *labelIdArray = nullptr;
       if( polydata->GetPointData() )
       {
           labelIdArray = dynamic_cast<vtkIntArray *>( polydata->GetPointData()->GetScalars() );
       }
       //POINTER_CHECK( labelIdArray );
       vtkPoints *newPoints = vtkPoints::New();
       vtkIntArray *newLabelIdArray = vtkIntArray::New();
       for( int i = 0; i < points->GetNumberOfPoints(); ++i )
       {
           if( needPts[i] )
           {
               PointStruct pt( points->GetPoint(i) );
               newPoints->InsertNextPoint( pt.GetPoint() );
               if( handleScalar && labelIdArray && labelIdArray->GetNumberOfValues() > 0 )
               {
                   //newLabelIdArray->InsertValue( i, labelIdArray->GetValue( i ) );
                   newLabelIdArray->InsertNextValue( labelIdArray->GetValue( i ) );
               }
           }
       }
       LOG( INFO, "after deleting, old number of points: ", points->GetNumberOfPoints(),
            ", new number of points: ", newPoints->GetNumberOfPoints() );
       pointCountDiff = points->GetNumberOfPoints() - newPoints->GetNumberOfPoints();
       points->DeepCopy( newPoints );

       newPoints->Delete();
       if( handleScalar && labelIdArray && labelIdArray->GetNumberOfValues() > 0 )
       {
           labelIdArray->DeepCopy( newLabelIdArray );
       }
       newLabelIdArray->Delete();

       polydata->RemoveDeletedCells();  // delete empty cells

       // update pointId in cell
       if( pointCountDiff > 0 )
       {
           vSPNew( cells, vtkCellArray );
           std::vector<int> badPoints;
           for( int i = 0; i < needPts.size(); ++i )
           {
               if( false == needPts[i] )
               {
                   badPoints.push_back( i );
               }
           }

           for( int i = 0; i < polydata->GetNumberOfCells(); ++i ) // all cells
           {
               vtkIdList *ptIds = vtkIdList::New();
               polydata->GetCellPoints( i, ptIds );
               for( int j = 0; j < ptIds->GetNumberOfIds(); ++j )  // triangle
               {
                   int pointId = ptIds->GetId(j);
                   int countOfBiggerOnes = 0;
                   for( int k = 0; k < badPoints.size(); ++k )
                   {
                       if( badPoints[k] < pointId  )
                       {
                           countOfBiggerOnes++;
                       }
                   }
                   ptIds->SetId( j, pointId - countOfBiggerOnes );
               }
               cells->InsertNextCell( ptIds );
               ptIds->Delete();
           }
           polydata->SetPolys( cells );
           LOG( INFO, "update pointId in cells, badPoints's count: ", badPoints.size() );
       }
       polydata->BuildCells();
       polydata->BuildLinks();
       polydata->Modified();

       LOG( INFO, "after RemoveDeletedCells, numberOfCells: ", polydata->GetNumberOfCells() );
    }
}
