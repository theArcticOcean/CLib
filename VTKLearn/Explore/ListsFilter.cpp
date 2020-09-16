#include "ListsFilter.h"

#include <vtkIdList.h>
#include <vtkSphereSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkTransform.h>

ListsFilter::ListsFilter(vtkCellArray *lines)
{
    m_Lines = lines;
    m_PolyData = nullptr;
    m_Renderer = nullptr;
}

ListsFilter::~ListsFilter()
{
    m_Lines = nullptr;
    m_EdgePts.clear();
}

void ListsFilter::Init()
{
    m_VisPtIds.clear();
    m_SegmentLists.clear();
    m_Lines->InitTraversal();
    vtkSPtrNew( cell, vtkIdList );
    while( m_Lines->GetNextCell( cell ) )
    {
        auto id0 = cell->GetId( 0 );
        auto id1 = cell->GetId( 1 );

        QSharedPointer<EdgePt> edgePt0 = nullptr;
        if( m_EdgePts.contains( id0 ) )
        {
            edgePt0 = m_EdgePts[ id0 ];
        }
        else
        {
            edgePt0 = QSharedPointer<EdgePt>( new EdgePt() );
            edgePt0->ptId = id0;
        }

        QSharedPointer<EdgePt> edgePt1 = nullptr;
        if( m_EdgePts.contains( id1 ) )
        {
            edgePt1 = m_EdgePts[ id1 ];
        }
        else
        {
            edgePt1 = QSharedPointer<EdgePt>( new EdgePt() );
            edgePt1->ptId = id1;
        }

        edgePt0->next = edgePt1.get();
        edgePt1->last = edgePt0.get();

        m_EdgePts[ id0 ] = edgePt0;
        m_EdgePts[ id1 ] = edgePt1;
        std::cout << id0 << "\t" << id1 << std::endl;

#if IsDebug
        if( 41 == id0 && 25 == id1 )
        {
            ShowPoint( id0 );
            ShowPoint( id1 );
        }
#endif
    }

    std::cout << "m_Lines count: " << m_Lines->GetNumberOfCells() << std::endl;
    std::cout << "m_EdgePts count: " << m_EdgePts.size() << std::endl;
}

vtkIdType ListsFilter::SearchBackwards(const vtkIdType &ptId, vtkSPtr<vtkIdList> list )
{
    vtkIdType currentId = ptId;
    while( m_EdgePts.contains( currentId ) && m_EdgePts[ currentId ]->last )
    {
        EdgePt *pt = m_EdgePts[ currentId ]->last;
        if( m_VisPtIds.contains( pt->ptId ) )
        {
            break;
        }
        list->InsertUniqueId( pt->ptId );
        currentId = pt->ptId;
        m_VisPtIds.push_back( currentId );
    }
    return currentId;
}

vtkIdType ListsFilter::SearchForwards(const vtkIdType &ptId, vtkSmartPointer<vtkIdList> list)
{
    vtkIdType currentId = ptId;
    while( m_EdgePts.contains( currentId ) && m_EdgePts[ currentId ]->next )
    {
        EdgePt *pt = m_EdgePts[ currentId ]->next;
        if( m_VisPtIds.contains( pt->ptId ) )
        {
            break;
        }
        list->InsertUniqueId( pt->ptId );
        currentId = pt->ptId;
        m_VisPtIds.push_back( currentId );
    }
    return currentId;
}

void ListsFilter::Update()
{
    QMapIterator<vtkIdType, QSharedPointer<EdgePt>> it( m_EdgePts );    
    while( it.hasNext() )
    {
        it.next();
        vtkIdType ptId = it.key();
        if( m_VisPtIds.contains( ptId ) )
        {
            continue;
        }

        // ============== start to search ==============
        vtkSPtrNew( list, vtkIdList );
        Search( ptId, list );
        m_SegmentLists.push_back( list );
        std::cout << "list size: " << list->GetNumberOfIds() << std::endl;
    }
    std::cout << "m_SegmentLists size: " << m_SegmentLists.size() << std::endl;
}

int ListsFilter::GetListsCount()
{
    return m_SegmentLists.size();
}

vtkSmartPointer<vtkIdList> ListsFilter::GetList(const int &index)
{
    if( index < m_SegmentLists.size() && index >= 0 )
    {
        return m_SegmentLists[ index ];
    }
    return nullptr;
}

void ListsFilter::SetPolyData(vtkPolyData *_data)
{
    m_PolyData = _data;

}

void ListsFilter::SetRenderer(vtkRenderer *_Renderer)
{
    m_Renderer = _Renderer;
}

void ListsFilter::ShowPoint(vtkIdType ptId)
{
    vtkSmartPointer<vtkSphereSource> sphereSource =
            vtkSmartPointer<vtkSphereSource>::New();
    sphereSource->SetCenter( m_PolyData->GetPoint( ptId ) );
    sphereSource->SetRadius( 0.02 );

    vtkSmartPointer<vtkPolyDataMapper> sphereMapper =
            vtkSmartPointer<vtkPolyDataMapper>::New();
    sphereMapper->SetInputConnection( sphereSource->GetOutputPort() );

    vtkSmartPointer<vtkActor> sphereActor =
            vtkSmartPointer<vtkActor>::New();
    sphereActor->SetMapper( sphereMapper );
    sphereActor->GetProperty()->SetColor( 0, 1, 0 );
    m_Renderer->AddActor( sphereActor );
}

void ListsFilter::ShowOriginalPoints()
{
    for( int i = 0; i < m_PolyData->GetNumberOfPoints(); ++i )
    {
        vtkSmartPointer<vtkTextSource> text2D =
            vtkSmartPointer<vtkTextSource>::New();
        text2D->SetText( QString::number( i ).toStdString().c_str() );

        vtkSmartPointer<vtkTransform> text2DTransform =
            vtkSmartPointer<vtkTransform>::New();
        double *center = m_PolyData->GetPoint( i );
        text2DTransform->Translate( center[0]+0.1, center[1], center[2] );
        text2DTransform->Scale( 0.005, 0.005, 0.005 );

        vtkSmartPointer<vtkTransformPolyDataFilter> text2DDataFilter =
            vtkSmartPointer<vtkTransformPolyDataFilter>::New();
        text2DDataFilter->SetTransform( text2DTransform );
        text2DDataFilter->SetInputConnection( text2D->GetOutputPort() );

        vtkSmartPointer<vtkCoordinate> coords =
            vtkSmartPointer<vtkCoordinate>::New();
        coords->SetCoordinateSystemToWorld();

        vtkSmartPointer<vtkPolyDataMapper2D> text2DMapper =
            vtkSmartPointer<vtkPolyDataMapper2D>::New();
        text2DMapper->SetInputConnection( text2DDataFilter->GetOutputPort() );
        text2DMapper->SetTransformCoordinate( coords );

        vtkSmartPointer<vtkActor2D> text2DActor =
            vtkSmartPointer<vtkActor2D>::New();
        text2DActor->SetMapper( text2DMapper );

        m_Renderer->AddActor( text2DActor );
    }
}

void ListsFilter::ShowPointsByListOrder(vtkSmartPointer<vtkIdList> list)
{
    for( int i = 0; i < list->GetNumberOfIds(); ++i )
    {
        vtkSmartPointer<vtkTextSource> text2D =
            vtkSmartPointer<vtkTextSource>::New();
        text2D->SetText( QString::number( i ).toStdString().c_str() );

        vtkSmartPointer<vtkTransform> text2DTransform =
            vtkSmartPointer<vtkTransform>::New();
        double *center = m_PolyData->GetPoint( list->GetId( i ) );
        text2DTransform->Translate( center[0]+0.1, center[1], center[2] );
        text2DTransform->Scale( 0.005, 0.005, 0.005 );

        vtkSmartPointer<vtkTransformPolyDataFilter> text2DDataFilter =
            vtkSmartPointer<vtkTransformPolyDataFilter>::New();
        text2DDataFilter->SetTransform( text2DTransform );
        text2DDataFilter->SetInputConnection( text2D->GetOutputPort() );

        vtkSmartPointer<vtkCoordinate> coords =
            vtkSmartPointer<vtkCoordinate>::New();
        coords->SetCoordinateSystemToWorld();

        vtkSmartPointer<vtkPolyDataMapper2D> text2DMapper =
            vtkSmartPointer<vtkPolyDataMapper2D>::New();
        text2DMapper->SetInputConnection( text2DDataFilter->GetOutputPort() );
        text2DMapper->SetTransformCoordinate( coords );

        vtkSmartPointer<vtkActor2D> text2DActor =
            vtkSmartPointer<vtkActor2D>::New();
        text2DActor->SetMapper( text2DMapper );

        m_Renderer->AddActor( text2DActor );
    }
}

void ListsFilter::Search(const vtkIdType &ptId, vtkSmartPointer<vtkIdList> list)
{
    const vtkIdType currentId = ptId;
    m_VisPtIds.push_back( ptId );

    vtkSPtrNew( list0, vtkIdList );
    int lastId0 = currentId;
    int currentId0 = SearchForwards( lastId0, list0 );
    SearchType type0 = E_Forward;

    while( currentId0 != lastId0 )
    {
        lastId0 = currentId0;
        if( type0 == E_Forward )
        {
            currentId0 = SearchBackwards( lastId0, list0 );
            type0 = E_Backward;
        }
        else
        {
            currentId0 = SearchForwards( lastId0, list0 );
            type0 = E_Forward;
        }
        std::cout << "currentId0: " << currentId0 << ", lastId0: " << lastId0 << std::endl;
    }

    vtkSPtrNew( list1, vtkIdList );
    int lastId1 = currentId;
    int currentId1 = SearchBackwards( lastId1, list1 );
    SearchType type1 = E_Backward;
    while( currentId1 != lastId1 )
    {
        lastId1 = currentId1;
        if( type1 == E_Backward )
        {
            currentId1 = SearchForwards( lastId1, list1 );
            type1 = E_Forward;
        }
        else
        {
            currentId1 = SearchBackwards( lastId1, list1 );
            type1 = E_Backward;
        }
    }

    for( int i = 0; i < list1->GetNumberOfIds(); ++i )
    {
        vtkIdType id = list1->GetId( i );
        list->InsertUniqueId( id );
    }
    list->InsertUniqueId( currentId );
    for( int i = 0; i < list0->GetNumberOfIds(); ++i )
    {
        vtkIdType id = list0->GetId( i );
        list->InsertUniqueId( id );
    }
}
