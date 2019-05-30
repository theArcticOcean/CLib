#pragma once

#include <vtkPolyDataAlgorithm.h>
#include <vtkCellPicker.h>
#include <vector>
#include <vtkActorCollection.h>

#include "VertexActor.h"
#include "macroDefinition.h"

using namespace std;

class FourSidesCone: public vtkPolyDataAlgorithm
{
public:
    vtkTypeMacro( FourSidesCone, vtkPolyDataAlgorithm );
    static FourSidesCone *New();
    CPP_GET_MACRO( m_VertexActors, vSP<vtkActorCollection> )
    CPP_GET_MACRO( m_VertextPositions, vector<PointStruct> );
    vtkActor *FetchVertexActor( const int index );
protected:
    FourSidesCone();
    ~FourSidesCone() override;

    int RequestData (vtkInformation *vtkNotUsed(request),
                     vtkInformationVector **vtkNotUsed(inputVector),
                     vtkInformationVector *outputVector) override;
    void VertexPointsInital();

    vSP<vtkCellPicker> m_VertexPicker;
    vSP<vtkActorCollection> m_VertexActors;
    vector<PointStruct> m_VertextPositions;
private:
    FourSidesCone(const FourSidesCone&) = delete;
    void operator=(const FourSidesCone&) = delete;
};
