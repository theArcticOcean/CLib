#pragma once

#include <vtkCellArray.h>
#include <vtkIdList.h>

#define vtkSPtr vtkSmartPointer
#define vtkSPtrNew(Var, Type) vtkSPtr<Type> Var = vtkSPtr<Type>::New();

class ConnectedEdgeFilter
{
public:
    ConnectedEdgeFilter(){}
    ~ConnectedEdgeFilter()
    {
        m_ConnectedLists.clear();
    }
    void Initialise( vtkSPtr<vtkCellArray> edgeSegments )
    {
        m_EdgeSegments = edgeSegments;
        m_EdgeSegments->InitTraversal();
        m_ConnectedLists.clear();
        vtkSPtrNew( cell, vtkIdList );
        while( m_EdgeSegments->GetNextCell( cell ) )
        {
            m_EdgeStartPointIds.push_back( cell->GetId( 0 ) );
            m_EdgeEndPointIds.push_back( cell->GetId( 1 ) );
            m_EgdeHandleFlag.push_back( false );
        }
    }
    void BackSearch(int startIndex, std::vector<int>& connectedList)
    {
        if ( nullptr == m_EdgeSegments || m_EdgeSegments->GetNumberOfCells() < 1 )
        {
            printf( "%s, %d: bad data", __FILE__, __LINE__ );
            return;
        }
        int segmentStartPointId = m_EdgeStartPointIds[startIndex];
        int segmentEndPointId = m_EdgeEndPointIds[startIndex];
        connectedList.push_back( segmentStartPointId );
        connectedList.push_back( segmentEndPointId );
        m_EgdeHandleFlag[ startIndex ] = true;

        int nextId = segmentEndPointId;
        bool foundNext = true;
        while (foundNext)
        {
            foundNext = false;
            for (int i = 0; i< m_EdgeSegments->GetNumberOfCells(); i++)
            {
                if (m_EgdeHandleFlag[i])
                {
                    continue;
                }
                if (nextId == m_EdgeEndPointIds[i])
                {
                    nextId = m_EdgeStartPointIds[i];
                    connectedList.push_back( nextId );
                    m_EgdeHandleFlag[i] = true;
                    foundNext = true;
                    break;

                }
                else if (nextId == m_EdgeStartPointIds[i])
                {
                    nextId = m_EdgeEndPointIds[i];
                    connectedList.push_back( nextId );
                    m_EgdeHandleFlag[i] = true;
                    foundNext = true;
                    break;
                }
            }
        }
    }
    void FrontSearch(int startIndex, std::vector<int>& connectedList)
    {
        if ( nullptr == m_EdgeSegments || m_EdgeSegments->GetNumberOfCells() < 1 )
        {
            printf( "%s, %d: bad data", __FILE__, __LINE__ );
            return;
        }

        bool findPre = true;
        int preId = m_EdgeStartPointIds[startIndex];
        while ( findPre )
        {
            findPre = false;
            for (int i = 0; i < m_EdgeSegments->GetNumberOfCells(); i++ )
            {
                if (m_EgdeHandleFlag[i])
                {
                    continue;
                }
                if (preId == m_EdgeStartPointIds[i])
                {
                    preId = m_EdgeEndPointIds[i];
                    connectedList.push_back(preId);
                    m_EgdeHandleFlag[i] = true;
                    findPre = true;
                    break;

                }
                else if (preId == m_EdgeEndPointIds[i])
                {
                    preId = m_EdgeStartPointIds[i];
                    connectedList.push_back(preId);
                    m_EgdeHandleFlag[i] = true;
                    findPre = true;
                    break;
                }
            }
        }
    }
    void HandleEdges()
    {
        if ( nullptr == m_EdgeSegments || m_EdgeSegments->GetNumberOfCells() < 1 )
        {
            printf( "%s, %d: bad data", __FILE__, __LINE__ );
            return;
        }

        int edgeIndex = 0;
        while ( edgeIndex !=-1 )
        {
            std::vector<int> connectedEdge0;
            BackSearch( edgeIndex, connectedEdge0 );

            std::vector<int> connectedEdge1;
            FrontSearch( edgeIndex, connectedEdge1 );

            // connect both list
            vtkSPtrNew( connectedList, vtkIdList );
            for( int i = connectedEdge0.size()-1; i >= 0; --i )
            {
                connectedList->InsertNextId( connectedEdge0[i] );
            }
            for( int i = 0; i < connectedEdge1.size(); ++i )
            {
                connectedList->InsertNextId( connectedEdge1[i] );
            }

            m_ConnectedLists.push_back( connectedList );
            edgeIndex = -1;
            int numberOfSegments = m_EgdeHandleFlag.size();
            for (int i = 0; i<numberOfSegments; i++)
            {
                if (m_EgdeHandleFlag[i] == false)
                {
                    edgeIndex = i;
                    break;
                }
            }
        }
    }
    vtkSPtr<vtkIdList> GetLongestList()
    {
        vtkSPtrNew( longestList, vtkIdList );
        int max = -1;
        for (int i = 0; i < m_ConnectedLists.size(); i++)
        {
            vtkIdList *list = m_ConnectedLists[i];
            vtkIdType listSize = list->GetNumberOfIds();
            if ( listSize > max )
            {
                max = listSize;
                longestList = list;
            }
        }
        return longestList;
    }
protected:
    vtkSPtr<vtkCellArray>               m_EdgeSegments;

    std::vector<bool>           m_EgdeHandleFlag;
    std::vector<int>            m_EdgeStartPointIds;
    std::vector<int>            m_EdgeEndPointIds;

    std::vector<vtkSPtr<vtkIdList>>     m_ConnectedLists;

};
