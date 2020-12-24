#pragma once

#include <vtkCellArray.h>
#include <QMap>
#include <QVector>
#include <QSharedPointer>
#include <vtkPolyData.h>
#include <vtkRenderer.h>
#include <vtkSmartPointer.h>
#include <vtkProperty.h>
#include <vtkTextSource.h>
#include <vtkTransformPolyDataFilter.h>
#include <vtkCoordinate.h>
#include <vtkPolyDataMapper2D.h>
#include <vtkActor2D.h>
#include <vtkProperty.h>

#define vtkSPtr vtkSmartPointer
#define vtkSPtrNew(Var, Type) vtkSPtr<Type> Var = vtkSPtr<Type>::New();

#define IsDebug 0

class ListsFilter
{
public:
    struct EdgePt
    {
        EdgePt(): next( nullptr ), last( nullptr ), ptId( -1 ) {}
        ~EdgePt(){ next = nullptr; last = nullptr; ptId = -1; }

        EdgePt *next;
        EdgePt *last;
        vtkIdType ptId;
    };
    enum SearchType{ E_Backward, E_Forward };

    ListsFilter( vtkCellArray *lines );
    ~ListsFilter();
    void Init();
    void Update();
    int GetListsCount();
    vtkSPtr<vtkIdList> GetList( const int &index );

    // === debug function ===
    void SetPolyData( vtkPolyData *_data );
    void SetRenderer( vtkRenderer *_Renderer );
    void ShowPoint( vtkIdType ptId );
    void ShowOriginalPoints();
    void ShowPointsByListOrder( vtkSPtr<vtkIdList> list );
protected:
    void Search(const vtkIdType &ptId, vtkSPtr<vtkIdList> list);
    vtkIdType SearchBackwards( const vtkIdType &ptId, vtkSPtr<vtkIdList> list );
    vtkIdType SearchForwards( const vtkIdType &ptId, vtkSPtr<vtkIdList> list );

    QVector<vtkSPtr<vtkIdList>> m_SegmentLists;
    vtkCellArray *m_Lines;
    QMap<vtkIdType, QSharedPointer<EdgePt>> m_EdgePts;
    QVector<vtkIdType> m_VisPtIds;

    // ===== debug actor ======
    vtkSPtr<vtkPolyData> m_PolyData;
    vtkSPtr<vtkRenderer> m_Renderer;
};
