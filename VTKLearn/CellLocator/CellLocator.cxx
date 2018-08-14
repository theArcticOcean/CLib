#include <vtkSmartPointer.h>
#include <vtkSphereSource.h>
#include <vtkCellLocator.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkProperty.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>

int main(int, char *[])
{
    //========> set sphere and shows.
    vtkSmartPointer<vtkSphereSource> sphereSource = vtkSmartPointer<vtkSphereSource>::New();
    sphereSource->SetRadius( 1.0 );
    sphereSource->SetCenter( 0, 0, 0 );
    sphereSource->Update();

    vtkSmartPointer<vtkPolyDataMapper> sphereMapper =  vtkSmartPointer<vtkPolyDataMapper>::New();
    sphereMapper->SetInputConnection( sphereSource->GetOutputPort() );

    vtkSmartPointer<vtkActor> sphereActor = vtkSmartPointer<vtkActor>::New();
    sphereActor->SetMapper( sphereMapper );
    sphereActor->GetProperty()->SetColor( 0, 1, 0 );
    //<======== set sphere and shows.


    // Create the tree
    vtkSmartPointer<vtkCellLocator> cellLocator =
    vtkSmartPointer<vtkCellLocator>::New();
    cellLocator->SetDataSet(sphereSource->GetOutput());
    cellLocator->BuildLocator();

    double testPoint[3] = {1.5, 0.0, 0.0};

    //Find the closest points to TestPoint
    double closestPoint[3];//the coordinates of the closest point will be returned here
    double closestPointDist2; //the squared distance to the closest point will be returned here
    vtkIdType cellId; //the cell id of the cell containing the closest point will be returned here
    int subId; //this is rarely used (in triangle strips only, I believe)
    cellLocator->FindClosestPoint(testPoint, closestPoint, cellId, subId, closestPointDist2);

    std::cout << "Coordinates of closest point: " << closestPoint[0] << " " << closestPoint[1] << " " << closestPoint[2] << std::endl;
    std::cout << "Squared distance to closest point: " << closestPointDist2 << std::endl;
    std::cout << "CellId: " << cellId << std::endl;

    //========> show test point
    vtkSmartPointer<vtkSphereSource> testSource = vtkSmartPointer<vtkSphereSource>::New();
    testSource->SetRadius( 0.1 );
    testSource->SetCenter( testPoint[0], testPoint[1], testPoint[2] );
    testSource->Update();

    vtkSmartPointer<vtkPolyDataMapper> testMapper =  vtkSmartPointer<vtkPolyDataMapper>::New();
    testMapper->SetInputConnection( testSource->GetOutputPort() );

    vtkSmartPointer<vtkActor> testActor = vtkSmartPointer<vtkActor>::New();
    testActor->SetMapper( testMapper );
    testActor->GetProperty()->SetColor( 1, 0, 0 );
    //<======== show test point

    vtkSmartPointer<vtkRenderWindow> renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
    vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
    renderer->AddActor( sphereActor );
    renderer->AddActor( testActor );
    renderWindow->AddRenderer( renderer );

    vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInterator =
            vtkSmartPointer<vtkRenderWindowInteractor>::New();
    renderWindowInterator->SetRenderWindow( renderWindow );

    int *size = renderWindow->GetSize();
    std::cout << "renderWindow's size is " << size[0] << ", " << size[1] << std::endl;
    renderWindow->Render();
    renderWindowInterator->Start();

    return EXIT_SUCCESS;
}
