#include <iostream>
#include <vtkSmartPointer.h>
#include <vtkSphereSource.h>
#include <vtkActor.h>
#include <vtkConeSource.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkPolyDataMapper.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkCellData.h>
#include <vtkNamedColors.h>
#include <vtkColorTransferFunction.h>

#define vtkSPtr vtkSmartPointer
#define vtkSPtrNew(Var, Type) vtkSPtr<Type> Var = vtkSPtr<Type>::New();

using namespace std;

void InsertPlane(vtkSPtr<vtkCellArray> cells, std::vector<vtkIdType> ptIds)
{
    /*vtkIdType ids1[3] = { ptIds[0], ptIds[1], ptIds[2] };
    vtkIdType ids2[3] = { ptIds[0], ptIds[2], ptIds[3] };
    cells->InsertNextCell( 3, ids1 );
    cells->InsertNextCell( 3, ids2 );*/

    vtkIdType ids[4] = { ptIds[0], ptIds[1], ptIds[2], ptIds[3] };
    cells->InsertNextCell( 4, ids );
}

int main()
{
    const int ptsCount = 8;
    double coords[ptsCount][3] = { {0, 0, 0}, {1, 0, 0}, {1, 1, 0}, {0, 1, 0},
                            {0, 0, 1}, {1, 0, 1}, {1, 1, 1}, {0, 1, 1} };

    vtkSPtrNew( pts, vtkPoints );
    for( int i = 0; i < ptsCount; ++i )
    {
        pts->InsertPoint( i, coords[i] );
    }

    vtkSPtrNew( cells, vtkCellArray );
    std::vector<vtkIdType> ids = { 0, 1, 2, 3 };
    InsertPlane( cells, ids );
    ids = { 4, 5, 6, 7 };
    InsertPlane( cells, ids );
    ids = { 0, 4, 7, 3 };
    InsertPlane( cells, ids );
    ids = { 1, 5, 6, 2 };
    InsertPlane( cells, ids );
    ids = { 7, 6, 2, 3 };
    InsertPlane( cells, ids );
    ids = { 0, 1, 5, 4 };
    InsertPlane( cells, ids );

    vtkSPtrNew( polyData, vtkPolyData );
    polyData->SetPoints( pts );
    polyData->SetPolys( cells );

    vtkSPtrNew( scalars, vtkIntArray );
    scalars->SetNumberOfTuples( ptsCount );
    for( int i = 0; i < 6; ++i )
    {
        scalars->SetTuple1( i, i );
    }
    polyData->GetCellData()->SetScalars( scalars );

    vtkSPtrNew( lut, vtkColorTransferFunction );
    lut->SetClamping( 0 );
    vtkSPtrNew( colors, vtkNamedColors );

    string colorNames[6] = { "Gray", "Blue", "Red", "Maroon", "Yellow", "Lime" };
    for( int i = 0; i < 6; ++i )
    {
        double color[3];
        colors->GetColorRGB( colorNames[i].c_str(), color );
        lut->AddRGBPoint( i, color[0], color[1], color[2] );
    }

    vtkSPtrNew( mapper, vtkPolyDataMapper );
    mapper->SetInputData( polyData );
    mapper->SetScalarModeToUseCellData();
    mapper->SetLookupTable( lut );
    mapper->SetScalarRange( 0, 5 );

    vtkSPtrNew( actor, vtkActor );
    actor->SetMapper( mapper );

    vtkSPtrNew( renderer, vtkRenderer );
    renderer->AddActor(actor);
    renderer->SetBackground( 0, 0, 0 );

    vtkSPtrNew( renderWindow, vtkRenderWindow );
    renderWindow->AddRenderer( renderer );

    vtkSPtrNew( renderWindowInteractor, vtkRenderWindowInteractor );
    renderWindowInteractor->SetRenderWindow( renderWindow );

    renderer->ResetCamera();
    renderWindow->Render();
    renderWindowInteractor->Start();
    return 0;
}
