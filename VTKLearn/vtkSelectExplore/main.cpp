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

#include "vtkSelectPolyData.h"
#include "ULog.h"

using namespace std;

#define vtkSPtr vtkSmartPointer
#define vtkSPtrNew(Var, Type) vtkSPtr<Type> Var = vtkSPtr<Type>::New();

int main()
{
    vtkSPtrNew( toothReader, vtkSTLReader );
    toothReader->SetFileName( "../tooth.STL" );
    toothReader->Update();

    vtkSPtrNew( circleReader, vtkXMLPolyDataReader );
    circleReader->SetFileName( "../circle0.vtp" );
    circleReader->Update();

    vtkSPtrNew( circleMapper, vtkPolyDataMapper );
    circleMapper->SetInputData( circleReader->GetOutput() );

    vtkSPtrNew( circleActor, vtkActor );
    circleActor->SetMapper( circleMapper );
    circleActor->GetProperty()->SetColor( 1, 0, 0 );

    vtkSmartPointer<vtkSelectPolyData> loop = vtkSmartPointer<vtkSelectPolyData>::New();
    loop->GlobalWarningDisplayOn();
    loop->DebugOn();

    vtkSPtrNew( tmpPd, vtkPolyData );
    tmpPd->DeepCopy( toothReader->GetOutput() );

    vtkSPtrNew( writer, vtkSTLWriter );
    string tmpFile = "newData.STL";
    writer->SetInputData( tmpPd );
    writer->SetFileName( tmpFile.c_str() );
    writer->Write();

    vtkSPtrNew( reader, vtkSTLReader );
    reader->SetFileName( tmpFile.c_str() );
    reader->Update();

    tmpPd->DeepCopy( reader->GetOutput() );
    tmpPd->Modified();

    loop->SetInputData( tmpPd );
    loop->SetLoop( circleReader->GetOutput()->GetPoints() );
    loop->GenerateSelectionScalarsOn();
    loop->SetSelectionModeToSmallestRegion();
    loop->Update();

    auto *loopPd = loop->GetOutput();
    cout << loopPd->GetNumberOfCells() << ", " << loopPd->GetNumberOfPoints() << endl;


    //negative scalars inside loop->Update();
    vtkSPtrNew( toothMapper, vtkPolyDataMapper );
    toothMapper->SetInputData( toothReader->GetOutput() ); //loop->GetOutput() );

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

    // ============= show broken point =============
    auto brokenId = loop->Getm_BrokenPtId();
    LOG( INFO, "brokenId: ", brokenId );
    double *brokenPt = tmpPd->GetPoint( brokenId );

    vtkSPtrNew( sphereSource, vtkSphereSource );
    sphereSource->SetCenter( brokenPt );
    sphereSource->Update();
    vtkSPtrNew( sphereMapper, vtkPolyDataMapper );
    sphereMapper->SetInputData( sphereSource->GetOutput() );
    sphereMapper->Update();
    vtkSPtrNew( sphereActor, vtkActor );
    sphereActor->SetMapper( sphereMapper );
    sphereActor->GetProperty()->SetColor( 0, 1, 0 );
    vtkSPtrNew( trans, vtkTransform );
    trans->Translate( brokenPt );
    trans->Scale( 0.1, 0.1, 0.1 );
    trans->Translate( -brokenPt[0], -brokenPt[1], -brokenPt[2] );
    trans->Update();
    sphereActor->SetUserTransform( trans );

    renderer->AddActor( sphereActor );
    // =======================================

    vtkSPtrNew( renderWindow, vtkRenderWindow );
    renderWindow->AddRenderer( renderer );

    vtkSPtrNew( renderWindowInteractor, vtkRenderWindowInteractor );
    renderWindowInteractor->SetRenderWindow( renderWindow );
    renderer->ResetCamera();
    renderWindow->Render();
    renderWindowInteractor->Start();
    return 0;
}
