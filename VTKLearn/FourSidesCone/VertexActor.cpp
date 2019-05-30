#include <vtkObjectFactory.h>
#include <vtkSphereSource.h>
#include <vtkPolyDataMapper.h>

#include "VertexActor.h"

vtkStandardNewMacro(VertexActor);

VertexActor::VertexActor()
{
    m_ActorId = -1;
}

VertexActor::~VertexActor()
{
}

void VertexActor::UpdatePos(PointStruct pos)
{
    vSPNew( sphere, vtkSphereSource );
    sphere->SetRadius(0.2);
    sphere->SetCenter( pos.point );
    sphere->Update();

    vSPNew( mapper, vtkPolyDataMapper );
    mapper->SetInputData( sphere->GetOutput() );
    mapper->Update();

    this->SetMapper( mapper );
}
