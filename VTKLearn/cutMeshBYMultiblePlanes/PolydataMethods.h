#pragma once

#include <vtkPolyData.h>
#include <vtkSmartPointer.h>

class CPolydataMethods
{
public:
    static void IncreaseDensity( vtkSmartPointer<vtkPolyData> pld, const double maxEdgeLen = 0.3 );
};
