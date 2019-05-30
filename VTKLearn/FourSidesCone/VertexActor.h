#pragma once

#include <vtkOpenGLActor.h>
#include "macroDefinition.h"
#include "tool.h"

class VertexActor: public vtkOpenGLActor
{
public:
    vtkTypeMacro(VertexActor, vtkActor);
    static VertexActor *New();

    CPP_SET_MACRO(m_ActorId, int)
    CPP_GET_MACRO(m_ActorId, int)
    void UpdatePos( PointStruct pos );

protected:
    VertexActor();
    ~VertexActor() override;

    int m_ActorId;
};
