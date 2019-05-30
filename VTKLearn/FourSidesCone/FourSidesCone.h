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
    CPP_GET_MACRO( m_VertextPositions, vector<PointStruct> )
    CPP_GET_MACRO( m_VertexPicker, vSP<vtkCellPicker> )
    CPP_SET_MACRO( m_ConeActor, vSP<vtkActor> )
    CPP_GET_MACRO( m_ConeActor, vSP<vtkActor> )

    vtkActor *FetchVertexActor( const vtkIdType index );
    void UpdateConeShape();
protected:
    FourSidesCone();
    ~FourSidesCone() override;

    int RequestData (vtkInformation *vtkNotUsed(request),
                     vtkInformationVector **vtkNotUsed(inputVector),
                     vtkInformationVector *outputVector) override;
    void VertexPointsInital();

    vSP<vtkCellPicker> m_VertexPicker;
    vSP<vtkActorCollection> m_VertexActors;
    vSP<vtkActor> m_ConeActor;
    vector<PointStruct> m_VertextPositions;
    vtkPolyData *m_OutputPolydata;
private:
    FourSidesCone(const FourSidesCone&) = delete;
    void operator=(const FourSidesCone&) = delete;
};
