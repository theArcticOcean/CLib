#include <iostream>
#include <vtkSmartPointer.h>
#include <vtkSphereSource.h>
#include <vtkActor.h>
#include <vtkConeSource.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkPolyDataMapper.h>
#include <vtkLineSource.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkAxesActor.h>
#include <vtkProperty.h>
#include <vtkXMLPolyDataReader.h>
#include <vtkCharArray.h>
#include <vtkPointData.h>
#include <vtkConeSource.h>
#include <vtkTransform.h>
#include <vtkColorTransferFunction.h>
#include <vtkCellData.h>
#include <vtkPoints.h>
#include <vtkPlane.h>
#include <vtkCell.h>
#include <vtkCellArray.h>
#include <vtkTriangle.h>
#include <vtkLineSource.h>
#include <vtkTriangleFilter.h>
#include <vtkCutter.h>
#include <QString>
#include <vtkXMLPolyDataReader.h>
#include <vtkProperty.h>

#include "ListsFilter.h"
#include "../tool.h"

using namespace std;

vtkSPtr<vtkRenderer> renderer;

int main()
{
    vtkSPtrNew( reader, vtkXMLPolyDataReader );
    reader->SetFileName( "../items.vtp" );
    reader->Update();

    vtkSPtrNew( mapper, vtkPolyDataMapper );
    mapper->SetInputData( reader->GetOutput() );

    vtkSPtrNew( actor, vtkActor );
    actor->SetMapper( mapper );

    vtkSPtrNew( clipPlane, vtkPlane );
    clipPlane->SetOrigin( 0, 0, 0 );
    clipPlane->SetNormal( 0, 0, 1 );

    vtkSPtrNew( cutter, vtkCutter );
    cutter->SetInputData( reader->GetOutput() );
    cutter->SetCutFunction( clipPlane );
    cutter->Update();

    vtkPolyData *cutPd = cutter->GetOutput();
    vtkSPtrNew( cutMapper, vtkPolyDataMapper );
    cutMapper->SetInputData( cutPd );

    vtkSPtrNew( cutActor, vtkActor );
    cutActor->SetMapper( cutMapper );
    cutActor->GetProperty()->SetColor( 1, 0, 0 );
    cutActor->GetProperty()->SetLineWidth( 5 );

    renderer = vtkSPtr<vtkRenderer>::New();
    renderer->AddActor( cutActor );
    renderer->AddActor( actor );
    renderer->SetBackground( 0, 0, 0 );

    vtkCellArray *lines = cutPd->GetLines();
    ListsFilter listF( lines );
    listF.SetPolyData( cutPd ); // for debug
    listF.SetRenderer( renderer ); // for debug
    listF.Init();
    //listF.ShowOriginalPoints();
    listF.Update();

    for( int i = 0; i < listF.GetListsCount(); ++i )
    {
        vtkSPtr<vtkIdList> list = listF.GetList( i );
        listF.ShowPointsByListOrder( list );
    }

    vtkSPtrNew( renderWindow, vtkRenderWindow );
    renderWindow->AddRenderer( renderer );

    vtkSPtrNew( renderWindowInteractor, vtkRenderWindowInteractor );
    renderWindowInteractor->SetRenderWindow( renderWindow );

    renderer->ResetCamera();
    renderWindow->Render();
    renderWindowInteractor->Start();
    return 0;
}
