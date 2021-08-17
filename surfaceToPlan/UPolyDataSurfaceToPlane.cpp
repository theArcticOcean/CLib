#include "UPolyDataSurfaceToPlane.h"
#include "tool.h"

#include <vtkInformationVector.h>
#include <vtkInformation.h>
#include <vtkMath.h>

vtkStandardNewMacro(CUPolyDataSurfaceToPlane);

CUPolyDataSurfaceToPlane::CUPolyDataSurfaceToPlane(vtkPlane *cf)
{
    m_ClipFunction = cf;
}

CUPolyDataSurfaceToPlane::~CUPolyDataSurfaceToPlane()
{
    m_ClipFunction = nullptr;
}

void CUPolyDataSurfaceToPlane::SetClipPlane(vtkPlane *plane)
{
    m_ClipFunction = plane;
}

vtkSmartPointer<vtkPlane> CUPolyDataSurfaceToPlane::GetCellPlane(vtkSmartPointer<vtkCell> cell, bool &outOfPlane)
{
    auto points = cell->GetPoints();
    if( points->GetNumberOfPoints() != 3 )
    {
        cout << "not triangle!\n";
        return nullptr;
    }

    PointStruct center, pts[3];
    outOfPlane = false;
    for( int i = 0; i < points->GetNumberOfPoints(); ++i )
    {
        PointStruct pt( points->GetPoint( i ) );
        center = center + pt;
        pts[i] = pt;

        if( m_ClipFunction->EvaluateFunction(pt.point) < 0 )
        {
            outOfPlane = true;
        }
    }
    center /= 3;
    PointStruct normal = ( pts[0] - pts[1] ) ^ ( pts[0] - pts[2] );
    normal.Unit();
    vtkSmartPointer<vtkPlane> plane = vtkSmartPointer<vtkPlane>::New();
    plane->SetOrigin( center.point );
    plane->SetNormal( normal.point );

    return plane;
}

int CUPolyDataSurfaceToPlane::RequestData
    (
    vtkInformation* vtkNotUsed(request),
    vtkInformationVector** inputVector,
    vtkInformationVector* outputVector
    )
{
    // get the info objects
    vtkInformation* inInfo = inputVector[0]->GetInformationObject(0);
    vtkInformation* outInfo = outputVector->GetInformationObject(0);

    // get the input and output
    vtkPolyData* input = vtkPolyData::SafeDownCast(inInfo->Get(vtkDataObject::DATA_OBJECT()));
    vtkPolyData* output = vtkPolyData::SafeDownCast(outInfo->Get(vtkDataObject::DATA_OBJECT()));
    double funcValueLimit = 1, normalAngleDiffLimit = 10;
    vtkSmartPointer<vtkIdList> surfaceCells = vtkSmartPointer<vtkIdList>::New();
    PointStruct clipNormal( m_ClipFunction->GetNormal() );
    cout << "clipNormal: " << clipNormal[0] << ", " << clipNormal[1] <<  ", " << clipNormal[2] << endl;
    for( int i = 0; i < input->GetNumberOfCells(); ++i )
    {
        vtkSmartPointer<vtkCell> cell = input->GetCell( i );
        bool outOfPlane;
        auto cellPlane = GetCellPlane( cell, outOfPlane );
        PointStruct cellNormal( cellPlane->GetNormal() );
        auto normalAngle = vtkMath::AngleBetweenVectors( clipNormal.point, cellNormal.point );
        auto funcValue = m_ClipFunction->EvaluateFunction( cellPlane->GetOrigin() );
        if( outOfPlane ||
            ( fabs( funcValue ) < funcValueLimit && fabs( normalAngle ) < normalAngleDiffLimit )
          )
        {
            surfaceCells->InsertNextId( i );
        }
    }

    vtkSmartPointer<vtkPoints> newPoints = vtkSmartPointer<vtkPoints>::New();
    newPoints->DeepCopy( input->GetPoints() );
    for( int i = 0; i < surfaceCells->GetNumberOfIds(); ++i )
    {
        auto cellId = surfaceCells->GetId( i );
        vtkIdType npts;
        vtkIdType *pts;
        input->GetCell( cellId, pts );
        for( int j = 0; j < 3; ++j )
        {
            PointStruct pt( input->GetPoint( pts[j] ) );
            m_ClipFunction->ProjectPoint( pt.point, pt.point );
            newPoints->SetPoint( pts[j], pt.point );
        }
    }

    vtkSmartPointer<vtkCellArray> newCells = vtkSmartPointer<vtkCellArray>::New();
    newCells->DeepCopy( input->GetPolys() );

    output->SetPoints( newPoints );
    output->SetPolys( newCells );

    return 1;
}
