#pragma once

class vtkCellArray;
class vtkIdList;

#include <vtkSmartPointer.h>
#include <vector>

#define vSP vtkSmartPointer
#define vSPNew(Var, Type) vSP<Type> Var = vSP<Type>::New();

class CULineSegmentsToConnectedList
{
public:
	CULineSegmentsToConnectedList();
	~CULineSegmentsToConnectedList();

	void SetLineSegments(vtkCellArray* inSegments)
	{
		m_LineSegments = inSegments;
	}

	void Transform(bool merge=true);

	size_t	GetNumberOfConnectList()
	{
		return m_ConnectedLists.size();
	}

	vSP<vtkIdList> GetList(int index)
	{
		if (index<0 || index>GetNumberOfConnectList())
			return NULL;
		return m_ConnectedLists[index];
	}


	vSP<vtkIdList> GetLongestList();



	vSP<vtkIdList> GetListFromRange(int startPointId, int endPointId, bool shorterOne = true);
	void			GetPartList(int startPointId, int endPointId, std::vector<vtkIdList*> &outLists);
private:
	void Init();
	bool IsProcessFinish();
	//first search afterward, the pointId in the start line is added
	void SearchAfterward(int startLineIndex,std::vector<int>& connectedList);

	//frontward, the startPoint is not included
	void SearchFrontward(int startPointId, std::vector<int>& connectedList);

	void	Merge();
	void	OrderConnectedList();

	vtkCellArray*				m_LineSegments;
	std::vector<vSP<vtkIdList>>		m_ConnectedLists;
	std::vector<bool>			m_SegmentProcessFlag;
	std::vector<int>			m_LineStartPointIds;
	std::vector<int>			m_LineEndPointIds;
			
};
