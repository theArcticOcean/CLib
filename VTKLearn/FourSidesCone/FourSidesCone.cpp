#include "FourSidesCone.h"
#include <vtkObjectFactory.h>
#include <vtkInformationVector.h>
#include <vtkInformation.h>
#include <vtkProperty.h>
#include <vtkMapper.h>
#include <vtkPolyDataMapper.h>

vtkStandardNewMacro(FourSidesCone);

FourSidesCone::~FourSidesCone()
{

}

void FourSidesCone::VertexPointsInital()
{
    m_VertexActors->RemoveAllItems();
    m_VertextPositions.clear();
    m_VertexPicker->InitializePickList();

    int i = 0;
    vSPNew( vertex0, VertexActor );
    m_VertextPositions.push_back( PointStruct(0, 0, 0) );
    vertex0->UpdatePos( m_VertextPositions[i] );
    vertex0->GetProperty()->SetColor( 1, 0, 0 );
    vertex0->Setm_ActorId( i );
    m_VertexActors->AddItem( vertex0 );
    m_VertexPicker->AddPickList( vertex0 );
    i++;

    vSPNew( vertex1, VertexActor );
    m_VertextPositions.push_back( PointStruct(2, 0, 0) );
    vertex1->GetProperty()->SetColor( 0, 1, 0 );
    vertex1->UpdatePos( m_VertextPositions[i] );
    vertex1->Setm_ActorId( i );
    m_VertexPicker->AddPickList( vertex1 );
    m_VertexActors->AddItem( vertex1 );
    i++;

    vSPNew( vertex2, VertexActor );
    m_VertextPositions.push_back( PointStruct(2, 0, 2) );
    vertex2->GetProperty()->SetColor( 0, 0, 1 );
    vertex2->UpdatePos( m_VertextPositions[i] );
    vertex2->Setm_ActorId( i );
    m_VertexPicker->AddPickList( vertex2 );
    m_VertexActors->AddItem( vertex2 );
    i++;

    vSPNew( vertex3, VertexActor );
    m_VertextPositions.push_back( PointStruct(0, 0, 2) );
    vertex3->GetProperty()->SetColor( 1, 1, 0 );
    vertex3->UpdatePos( m_VertextPositions[i] );
    vertex3->Setm_ActorId( i );
    m_VertexPicker->AddPickList( vertex3 );
    m_VertexActors->AddItem( vertex3 );
    i++;

    vSPNew( vertex4, VertexActor );
    m_VertextPositions.push_back( PointStruct(1, 3, 1) );
    vertex4->GetProperty()->SetColor( 1, 0, 1 );
    vertex4->UpdatePos( m_VertextPositions[i] );
    vertex4->Setm_ActorId( i );
    m_VertexPicker->AddPickList( vertex4 );
    m_VertexActors->AddItem( vertex4 );
    i++;

    m_VertexPicker->PickFromListOn();
}

void FourSidesCone::UpdateConeShape()
{
    vtkPoints *points = m_OutputPolydata->GetPoints();
    // don't set point by pointer points->GetPoint(i)
    for( int i = 0; i < 5; ++i )
    {
        vtkActor *actor = FetchVertexActor(i);
        PointStruct centerPos( actor->GetCenter() );
        points->SetPoint( i, centerPos.point );
        //printf( "%d point: (%lf, %lf, %lf)\n", i, centerPos[0], centerPos[1], centerPos[2] );
    }

    m_OutputPolydata->Modified();
    vSPNew( polyDataMapper, vtkPolyDataMapper );
    polyDataMapper->SetInputData( m_OutputPolydata );

    m_ConeActor->SetMapper( polyDataMapper );
}

int FourSidesCone::RequestData(vtkInformation *vtkNotUsed(request),
                               vtkInformationVector **vtkNotUsed(inputVector),
                               vtkInformationVector *outputVector)
{
    // get the info object
    vtkInformation *outInfo = outputVector->GetInformationObject(0);

    // get the ouptut
    vtkPolyData *output = vtkPolyData::SafeDownCast(
      outInfo->Get(vtkDataObject::DATA_OBJECT()));

    vtkPoints *newPoints = vtkPoints::New();
    VertexPointsInital();

    for( int i = 0; i < 5; ++i )
    {
        newPoints->InsertNextPoint( m_VertextPositions[i].point );
    }
    output->SetPoints( newPoints );

    vtkCellArray *newFaces = vtkCellArray::New();
    vtkIdType face[3] = { 0 };
    face[0] = 0; face[1] = 2; face[2] = 1;
    newFaces->InsertNextCell( 3, face );
    face[0] = 0; face[1] = 3; face[2] = 2;
    newFaces->InsertNextCell( 3, face );
    face[0] = 3; face[1] = 2; face[2] = 4;
    newFaces->InsertNextCell( 3, face );
    face[0] = 2; face[1] = 1; face[2] = 4;
    newFaces->InsertNextCell( 3, face );
    face[0] = 1; face[1] = 0; face[2] = 4;
    newFaces->InsertNextCell( 3, face );
    face[0] = 0; face[1] = 3; face[2] = 4;
    newFaces->InsertNextCell( 3, face );
    output->SetPolys( newFaces );
    m_OutputPolydata = output;

    newPoints->Delete();
    newFaces->Delete();
    vtkDebugMacro( "finished task." );
    return 1;
}

FourSidesCone::FourSidesCone()
{
    m_VertexActors = vSP<vtkActorCollection>::New();
    m_VertexPicker = vSP<vtkCellPicker>::New();
    this->SetNumberOfInputPorts( 0 );
    m_OutputPolydata = nullptr;
    m_ConeActor = nullptr;
}

vtkActor *FourSidesCone::FetchVertexActor( const vtkIdType index )
{
    m_VertexActors->InitTraversal();
    if( index >= m_VertexActors->GetNumberOfItems() )
    {
        return nullptr;
    }
    int count = 0;
    vtkActor *actor = m_VertexActors->GetNextActor();
    while( count != index )
    {
        actor = m_VertexActors->GetNextActor();
        count++;
    }
    return actor;
}
