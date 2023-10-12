#include <vtkActor.h>
#include <vtkCleanPolyData.h>
#include <vtkDistancePolyDataFilter.h>
#include <vtkNamedColors.h>
#include <vtkNew.h>
#include <vtkPointData.h>
#include <vtkPolyDataMapper.h>
#include <vtkPolyDataReader.h>
#include <vtkProperty.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkScalarBarActor.h>
#include <vtkSmartPointer.h>
#include <vtkSphereSource.h>
#include <vtkSTLReader.h>

#include <vector>
#include <list>
#include "point.hpp"

#define vtkSPtr vtkSmartPointer
#define vtkSPtrNew(Var, Type) vtkSPtr<Type> Var = vtkSPtr<Type>::New();

std::vector<Point> originalPts = { Point(1, 0, 0), Point(3, 1, 0), Point(7, 10, 0), Point(9, 0, 0) };

vtkSmartPointer<vtkPolyData> ShowListByLine(std::vector<Point> list)
{
    vtkSPtrNew( result, vtkPolyData );
    vtkSPtrNew( resultPts, vtkPoints );
    vtkSPtrNew( resultLines, vtkCellArray );
    for( int i = 0; i < list.size(); ++i )
    {
        resultPts->InsertNextPoint( list[i].point );
    }
    for( int i = 0; i < list.size()-1; ++i )
    {
        vtkIdType pts[2] = { i, i + 1 };
        resultLines->InsertNextCell( 2, pts );
    }
    result->SetPoints( resultPts );
    result->SetLines( resultLines );
    result->Modified();
    return result;
}

std::vector<Point> HandleNumSet(std::vector<Point> numSet)
{
    Point p3 = (numSet[0] + numSet[1])*0.5;
    Point p4 = (numSet[1] + numSet[2])*0.5;
    Point p1_ = (p3 + p4)*0.5;
    return std::vector<Point>{ p3, p1_, p4 };
}

std::list<Point> SubdivisionProcedure( std::vector<Point> inputPoints, std::vector<Point> &markedPts )
{
    std::list<Point> curList;
    for( auto value : inputPoints ){ curList.push_back( value ); }

    for( int i = 1; i < inputPoints.size() - 1; ++i )
    {
        // the points had been changed will be ignore.
        if( std::find( markedPts.begin(), markedPts.end(), inputPoints[i] ) != markedPts.end() ) continue;

        std::list<Point>::iterator lastOne = std::find(curList.begin(), curList.end(), inputPoints[i] );
        std::advance( lastOne, -1);
        std::list<Point>::iterator nextOne = std::find(curList.begin(), curList.end(), inputPoints[i] );
        std::advance( nextOne, 1);

        std::vector<Point> numSet = { *lastOne, inputPoints[i], *nextOne };
        std::vector<Point> newPts = HandleNumSet( numSet );

        std::list<Point>::iterator pos = std::find(curList.begin(), curList.end(), inputPoints[i] );
        curList.insert( pos, newPts[0] );
        curList.insert( pos, newPts[1] );
        curList.insert( pos, newPts[2] );
        curList.erase( pos );

        markedPts.push_back( newPts[1] );
    }
    return curList;
}

int main(int argc, char* argv[])
{
    vtkSmartPointer<vtkPolyData> input1 = ShowListByLine( originalPts );

    std::vector<Point> markedPts;
    std::vector<Point> samplePts = originalPts;
    std::list<Point> pts;
    int iterationTimes = 11;
    while( iterationTimes-- )
    {
        pts = SubdivisionProcedure( samplePts, markedPts );
        samplePts.clear();
        for( auto element: pts  )
        {
            samplePts.push_back( element );
        }
    }

    std::vector<Point> inputPts;
    for (const auto& element : pts) {
        inputPts.push_back( element );
    }

    vtkSmartPointer<vtkPolyData> input2 = ShowListByLine( inputPts );

    vtkNew<vtkRenderWindow> renWin;
    renWin->SetSize(1200, 500);

    vtkNew<vtkRenderWindowInteractor> renWinInteractor;
    renWinInteractor->SetRenderWindow(renWin);

    double leftViewport[4] = {0.0, 0.0, 0.5, 1.0};
    double rightViewport[4] = {0.5, 0.0, 1.0, 1.0};

    vtkSPtrNew( mapper1, vtkPolyDataMapper );
    mapper1->SetInputData( input1 );
    vtkSPtrNew( actor1, vtkActor );
    actor1->SetMapper( mapper1 );

    vtkSPtrNew( mapper2, vtkPolyDataMapper );
    mapper2->SetInputData( input2 );
    vtkSPtrNew( actor2, vtkActor );
    actor2->SetMapper( mapper2 );

    // Setup renderers
    vtkSPtrNew( leftRenderer, vtkRenderer );
    leftRenderer->SetViewport( leftViewport );
    leftRenderer->AddActor( actor1 );
    leftRenderer->SetBackground(.6, .5, .4);
    leftRenderer->ResetCamera();

    vtkSPtrNew( rightRenderer, vtkRenderer );
    rightRenderer->SetViewport(rightViewport);
    rightRenderer->AddActor( actor2 );
    rightRenderer->SetBackground(.4, .5, .6);
    rightRenderer->SetActiveCamera( leftRenderer->GetActiveCamera() );

    renWin->AddRenderer( leftRenderer );
    renWin->AddRenderer( rightRenderer );

    renWin->Render();
    renWinInteractor->Start();


    return EXIT_SUCCESS;
}
