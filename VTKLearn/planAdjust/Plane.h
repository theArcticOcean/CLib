#ifndef PLANE_H
#define PLANE_H

#include <vtkPlaneSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkSmartPointer.h>
#include <vtkProperty.h>
#include <vtkTransform.h>
#include <vtkRenderWindow.h>

class Plane
{
public:
    Plane();

    vtkSmartPointer<vtkActor> planeActor;
    vtkPlaneSource *getPlaneSource();
    void showFourPinnacles();
    vtkRenderWindow *renderWindow;

private:
    vtkSmartPointer<vtkPlaneSource> planeSource;
    vtkSmartPointer<vtkPolyDataMapper> planeMapper;
    vtkSmartPointer<vtkPolyData> polyData;
    vtkSmartPointer<vtkTransform> transform;
};

#endif
