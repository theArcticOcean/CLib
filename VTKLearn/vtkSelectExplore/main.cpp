#include <iostream>
#include <vtkSmartPointer.h>
#include <vtkSphereSource.h>
#include <vtkActor.h>
#include <vtkConeSource.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkPolyDataMapper.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkSTLReader.h>
#include <vtkXMLPolyDataReader.h>
#include <vtkProperty.h>
#include <vtkSTLWriter.h>
#include <vtkSTLReader.h>
#include <vtkTransform.h>

#include "UVtkSelectPolyData.h"
#include "ULog.h"
#include "ConnectedEdgeFilter.hpp"

using namespace std;

#define vtkSPtr vtkSmartPointer
#define vtkSPtrNew(Var, Type) vtkSPtr<Type> Var = vtkSPtr<Type>::New();
#define Debug 1

int main()
{
    vtkSPtrNew( toothReader, vtkXMLPolyDataReader );
    toothReader->SetFileName( "../tooth.vtp" );
    toothReader->Update();

    vtkSPtrNew( circleReader, vtkXMLPolyDataReader );
    circleReader->SetFileName( "../line.vtp" );
    circleReader->Update();

    vtkSPtrNew( circleMapper, vtkPolyDataMapper );
    circleMapper->SetInputData( circleReader->GetOutput() );

    vtkSPtrNew( circleActor, vtkActor );
    circleActor->SetMapper( circleMapper );
    circleActor->GetProperty()->SetColor( 1, 0, 0 );

    vtkSmartPointer<CUVtkSelectPolyData> loop = vtkSmartPointer<CUVtkSelectPolyData>::New();
#if Debug
    loop->GlobalWarningDisplayOn();
    loop->DebugOn();
#endif
    loop->SetInputData( toothReader->GetOutput() );

    // ---------------- sort points on circle --------------------
    vtkPolyData *circlePd = circleReader->GetOutput();
    ConnectedEdgeFilter* connectFilter = new ConnectedEdgeFilter;
    connectFilter->Initialise( circlePd->GetLines() );
    connectFilter->HandleEdges();

    vSP<vtkPoints> connectPoints = vSP<vtkPoints>::New();
    vSP<vtkIdList> longestList = connectFilter->GetLongestList();
    for (vtkIdType i = 0; i < longestList->GetNumberOfIds(); i++)
    {
        connectPoints->InsertNextPoint( circleReader->GetOutput()->GetPoint(longestList->GetId(i)) );
    }
    delete connectFilter;
    // ---------------- finish: sort points --------------------

    loop->SetLoop( connectPoints ); // circleReader->GetOutput()->GetPoints() );
    loop->GenerateSelectionScalarsOn();
    loop->GlobalWarningDisplayOn();
    loop->DebugOn();
    loop->SetSelectionModeToSmallestRegion();
    //loop->SetSelectionModeToClosestPointRegion();
    loop->Update();

    auto *loopPd = loop->GetOutput();
    cout << loopPd->GetNumberOfCells() << ", " << loopPd->GetNumberOfPoints() << endl;

    //negative scalars inside loop->Update();
    vtkSPtrNew( toothMapper, vtkPolyDataMapper );
    toothMapper->SetInputData( loop->GetOutput() ); // toothReader->GetOutput() );

    vtkSPtrNew( toothActor, vtkActor );
    toothActor->SetMapper( toothMapper );

    vtkSPtrNew( renderer, vtkRenderer );
    renderer->AddActor( toothActor );
    renderer->AddActor( circleActor );
    renderer->SetBackground( 0, 0, 0 );

    // ============= show edge ================
    vtkPolyData *edgeData = loop->GetSelectionEdges();
    vtkSPtrNew( edgeMapper, vtkPolyDataMapper );
    edgeMapper->SetInputData( edgeData );
    vtkSPtrNew( edgeActor, vtkActor );
    edgeActor->SetMapper( edgeMapper );
    edgeActor->GetProperty()->SetColor( 1, 1, 0 );
    renderer->AddActor( edgeActor );
    // ============= finish ================

    vtkSPtrNew( renderWindow, vtkRenderWindow );
    renderWindow->AddRenderer( renderer );

    vtkSPtrNew( renderWindowInteractor, vtkRenderWindowInteractor );
    renderWindowInteractor->SetRenderWindow( renderWindow );
    renderer->ResetCamera();
    renderWindow->Render();
    renderWindowInteractor->Start();
    return 0;
}
