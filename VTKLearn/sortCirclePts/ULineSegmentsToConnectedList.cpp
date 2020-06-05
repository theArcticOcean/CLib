#include "ULineSegmentsToConnectedList.h"
#include "vtkIdList.h"
#include "vtkCellArray.h"

CULineSegmentsToConnectedList::CULineSegmentsToConnectedList()
{

}


CULineSegmentsToConnectedList::~CULineSegmentsToConnectedList()
{
	for (int i = 0; i < GetNumberOfConnectList(); i++)
		m_ConnectedLists[i]->Delete();
	m_ConnectedLists.clear();
}


void CULineSegmentsToConnectedList::Transform(bool bMerge)
{
	int numberOfSegments = m_LineSegments->GetNumberOfCells();
	if (numberOfSegments == 0)
		return;
	Init();
	int segmentIndex = 0;
	while (segmentIndex!=-1)
	{
		std::vector<int> connectedAfterwardList;
		SearchAfterward(segmentIndex, connectedAfterwardList);
		
		std::vector<int> connectedFrontwardList;
		SearchFrontward(m_LineStartPointIds[segmentIndex], connectedFrontwardList);

		vtkIdList *list = vtkIdList::New();
		
		//std::cout << "List id:" << numberOfConnectedList - 1 <<std::endl;
		for (int i = connectedFrontwardList.size()-1; i>=0; i--)
		{
			list->InsertNextId(connectedFrontwardList[i]);
			//std::cout << connectedFrontwardList[i] << ",";
		}
		for (int i = 0; i < connectedAfterwardList.size(); i++)
		{
			list->InsertNextId(connectedAfterwardList[i]);
			//std::cout << connectedAfterwardList[i] << ",";
		}
		//std::cout <<std::endl;
		m_ConnectedLists.push_back(list);

		segmentIndex = -1;
		int numberOfSegments = m_SegmentProcessFlag.size();
		for (int i = 0; i<numberOfSegments; i++)
		{
			if (m_SegmentProcessFlag[i] == false)
			{
				segmentIndex = i;
				break;
			}
		}
	}

    if (bMerge)
        Merge();
    OrderConnectedList();
	//int numberOfConnectedList = m_ConnectedLists.size();
	//if (numberOfConnectedList == 2)
	//{
	//	for (int i = 0; i < 2; i++)
	//	{
	//		std::cout << "List id:" << i<<std::endl;
	//		vtkIdList *list = m_ConnectedLists[i];
	//		for(int j = 0; j < list->GetNumberOfIds(); j++)
	//		{
	//			int id = list->GetId(j);
	//			std::cout << id << ",";
	//		}
	//		std::cout <<std::endl;
	//	}
	//}
}


void CULineSegmentsToConnectedList::Init()
{
	m_LineSegments->InitTraversal();
	vSPNew(cellList,vtkIdList);
	while (m_LineSegments->GetNextCell(cellList))
	{
		m_SegmentProcessFlag.push_back(false);
		m_LineStartPointIds.push_back(cellList->GetId(0));
		m_LineEndPointIds.push_back(cellList->GetId(1));
	}
}


bool CULineSegmentsToConnectedList::IsProcessFinish()
{
	bool bEnd = true;
	int numberOfSegments = m_SegmentProcessFlag.size();
	for (int i = 0; i<numberOfSegments; i++)
	{
		if (m_SegmentProcessFlag[i] == false)
		{
			bEnd = false;
			break;
		}
	}
	return bEnd;
}


void CULineSegmentsToConnectedList::SearchAfterward(int startLineIndex, std::vector<int>& connectedList)
{
	int numberOfSegments = m_LineSegments->GetNumberOfCells();
	if (numberOfSegments == 0)
		return;
	bool findNextId = true;
	int segmentStartPointId = m_LineStartPointIds[startLineIndex];
	int segmentEndPointId = m_LineEndPointIds[startLineIndex];
	connectedList.push_back(segmentStartPointId);
	connectedList.push_back(segmentEndPointId);
	m_SegmentProcessFlag[startLineIndex] = true;

	int nextId = segmentEndPointId;
	while (findNextId)
	{
		findNextId = false;
		for (int i = 0; i<numberOfSegments; i++)
		{
			if (m_SegmentProcessFlag[i])
				continue;
			if (m_LineEndPointIds[i] == nextId)
			{
				nextId = m_LineStartPointIds[i];
				connectedList.push_back(nextId);
				m_SegmentProcessFlag[i] = true;
				findNextId = true;
				break;

			}
			else if (m_LineStartPointIds[i] == nextId)
			{
				nextId = m_LineEndPointIds[i];
				connectedList.push_back(nextId);
				m_SegmentProcessFlag[i] = true;
				findNextId = true;
				break;
			}
		}
	}
}


void CULineSegmentsToConnectedList::SearchFrontward(int startPointId, std::vector<int>& connectedList)
{
	int numberOfSegments = m_LineSegments->GetNumberOfCells();
	bool findPreId = true;
	int preId = startPointId;
	while (findPreId)
	{
		findPreId = false;
		for (int i = 0; i<numberOfSegments; i++)
		{
			if (m_SegmentProcessFlag[i])
				continue;
			if (m_LineStartPointIds[i] == preId)
			{
				preId = m_LineEndPointIds[i];
				connectedList.push_back(preId);
				m_SegmentProcessFlag[i] = true;
				findPreId = true;
				break;

			}
			else if (m_LineEndPointIds[i] == preId)
			{
				preId = m_LineStartPointIds[i];
				connectedList.push_back(preId);
				m_SegmentProcessFlag[i] = true;
				findPreId = true;
				break;
			}
		}
	}
}


vSP<vtkIdList> CULineSegmentsToConnectedList::GetListFromRange(int startPointId, int endPointId, bool shorterOne)
{
	vSP<vtkIdList> retList = vSP<vtkIdList>::New();
	int numberOfList = GetNumberOfConnectList();
	for (int i = 0; i < numberOfList; i++)
	{
		vtkIdList *list = m_ConnectedLists[i];
		int startIndex = list->IsId(startPointId);
		int endIndex = list->IsId(endPointId);
		if (startIndex == -1 || endIndex == -1)
			continue;
		if (endIndex < startIndex)
		{
			int tmp = startIndex;
			startIndex = endIndex;
			endIndex = tmp;
		}

		int listStartPointId = list->GetId(0);
		int listSize = list->GetNumberOfIds();
		int listEndPointId = list->GetId(listSize - 1);
		retList = vtkIdList::New();

		if (startIndex == endIndex)
		{
			retList->DeepCopy(list);
			break;
		}
		if (listStartPointId == listEndPointId)
		{
			int halfListSize = listSize / 2;
			int index=-1;
			for (int j = 0; index != endIndex; j++)
			{
				index = (startIndex + j) % listSize;
				if (index == listSize-1)
					continue;
				if (index == endIndex)
					continue;
				retList->InsertNextId(list->GetId(index));
			}
			retList->InsertNextId(list->GetId(endIndex));
			//retList->InsertNextId(list->GetId(endIndex));
			int numberOfRetList = retList->GetNumberOfIds();
			if ((numberOfRetList > halfListSize&&shorterOne) || (numberOfRetList < halfListSize&&!shorterOne))
			{
				retList->Initialize();
				index = -1;
				for (int j = 0; index != endIndex; j--)
				{
					index = (startIndex + j + listSize) % listSize;
					if (index == listSize-1)
						continue;
					if (index == endIndex)
						continue;

					retList->InsertNextId(list->GetId(index));
				}
				retList->InsertNextId(list->GetId(endIndex));
				//retList->InsertNextId(list->GetId(endIndex));
			}
		}
		else
		{
			if (endIndex > startIndex)
			{
				for (int j = startIndex; j <= endIndex; j++)
				{
					retList->InsertNextId(list->GetId(j));
				}
			}
			else
			{
				int index = -1;
				for (int j = 0; index != endIndex; j--)
				{
					index = (startIndex + j + listSize) % listSize;
					retList->InsertNextId(list->GetId(index));
				}
				//retList->InsertNextId(list->GetId(endIndex));
			}
		}
		break;
	}
	return retList;
}


void CULineSegmentsToConnectedList::Merge()
{

	int numberOfList = GetNumberOfConnectList();
	if (numberOfList < 2)
		return;
	std::vector<bool> merged;
	for (int i = 0; i < numberOfList; i++)
		merged.push_back(false);
	for (int i = 0; i < numberOfList; i++)
	{
		if (merged[i])
			continue;
		vtkIdList *curList = m_ConnectedLists[i];
		int listStartPointId = curList->GetId(0);
		int curListSize = curList->GetNumberOfIds();
		int listEndPointId = curList->GetId(curListSize - 1);
		if (listStartPointId == listEndPointId)
		{
			for (int j = 0; j < numberOfList; j++)
			{
				if (j == i)
					continue;
				if (merged[j] == true)
					continue;
				vtkIdList *list = m_ConnectedLists[j];
				int listSize = list->GetNumberOfIds();

				vtkIdList *bigList = list;
				vtkIdList *smallList = curList;
				int mergeId = i;
				if (listSize < curListSize)
				{
					bigList = curList;
					smallList = list;
					mergeId = j;
				}

				int numberOfSmallList = smallList->GetNumberOfIds();
				int indexInSmallList = -1;
				int indexInBigList = -1;
				for (int k = 0; k < numberOfSmallList; k++)
				{
					int id = smallList->GetId(k);
					int index = bigList->IsId(id);
					if (index != -1)
					{
						indexInSmallList = k;
						indexInBigList = index;
						break;
					}
				}
				if (indexInBigList == -1)
					continue;

				vSPNew(newList, vtkIdList);
				for (int i = 0; i <= indexInBigList; i++)
				{
					int id = bigList->GetId(i);
					newList->InsertNextId(id);
				}
				int num = 0;
				//int addIndex;
				while (num < numberOfSmallList)
				{
					int index = (indexInSmallList + 1 + num) % numberOfSmallList;
					if (index != 0)
					{
						int id = smallList->GetId(index);
						newList->InsertNextId(id);
					}

					num++;
				}
				int numberOfBigList = bigList->GetNumberOfIds();
				for (int i = indexInBigList + 1; i < numberOfBigList; i++)
				{
					int id = bigList->GetId(i);
					newList->InsertNextId(id);
				}
				bigList->DeepCopy(newList);
				merged[mergeId] = true;
				break;
			}
		}
	}
	
	std::vector<vSP<vtkIdList>>::iterator it = m_ConnectedLists.begin();
	int i = 0;
	while (it != m_ConnectedLists.end())
	{
		if (merged[i] == true)
			it = m_ConnectedLists.erase(it);
		else
			it++;

		i++;
	}
}



void CULineSegmentsToConnectedList::OrderConnectedList()
{
	int numberOfConnectedList = m_ConnectedLists.size();
	std::vector<vSP<vtkIdList>> copidLists;
	for (int i = 0; i < numberOfConnectedList; i++)
	{
		vtkIdList *list = m_ConnectedLists[i];
		int listSize = list->GetNumberOfIds();
		std::vector<vSP<vtkIdList>>::iterator itr = copidLists.begin();
		while (itr != copidLists.end())
		{
			vtkIdList *curList = *itr;
			if (curList->GetNumberOfIds() < listSize)
				break;
			itr++;
		}
		copidLists.insert(itr,list);
	}
	m_ConnectedLists.clear();
	for (int i = 0; i < numberOfConnectedList; i++)
	{
		m_ConnectedLists.push_back(copidLists[i]);
	}
	copidLists.clear();
}


vSP<vtkIdList> CULineSegmentsToConnectedList::GetLongestList()
{
	vSP<vtkIdList> longestList = vSP<vtkIdList>::New();
	int numberOfConnectedList = m_ConnectedLists.size();
	if (numberOfConnectedList == 0)
		return NULL;
	int max = -1;
	for (int i = 0; i < numberOfConnectedList; i++)
	{
		vtkIdList *list = m_ConnectedLists[i];
		int listSize = list->GetNumberOfIds();
		if (listSize > max)
		{
			max = listSize;
			longestList = list;
		}
	}
	return longestList;
}


void CULineSegmentsToConnectedList::GetPartList(int startPointId, int endPointId, std::vector<vtkIdList*> &outLists)
{
	int numberOfList = GetNumberOfConnectList();
	for (int i = 0; i < numberOfList; i++)
	{
		vtkIdList *list = m_ConnectedLists[i];
		int startIndex = list->IsId(startPointId);
		int endIndex = list->IsId(endPointId);
		if (startIndex == -1 || endIndex == -1)
			continue;


		int listStartPointId = list->GetId(0);
		int listSize = list->GetNumberOfIds();
		int listEndPointId = list->GetId(listSize - 1);
		vtkIdList *retList = vtkIdList::New();

		if (startIndex == endIndex)
		{
			retList->DeepCopy(list);
			outLists.push_back(retList);
			continue;
		}
		if (listStartPointId == listEndPointId)
		{
			int index = -1;
			for (int j = 0; index != endIndex; j++)
			{
				index = (startIndex + j) % listSize;
				if (index == listSize-1)
					continue;
				if (index == endIndex)
					continue;
				retList->InsertNextId(list->GetId(index));
			}
			retList->InsertNextId(list->GetId(endIndex));
			outLists.push_back(retList);

			vtkIdList *retList1 = vtkIdList::New();
			index = -1;
			for (int j = 0; index != endIndex; j--)
			{
				index = (startIndex + j + listSize) % listSize;
				if (index == listSize-1)
					continue;
				if (index == endIndex)
					continue;

				retList1->InsertNextId(list->GetId(index));
			}
			retList1->InsertNextId(list->GetId(endIndex));
			outLists.push_back(retList1);
		}
		else
		{
			if (endIndex > startIndex)
			{
				for (int j = startIndex; j <= endIndex; j++)
				{
					retList->InsertNextId(list->GetId(j));
				}
			}
			else
			{
				int index = -1;
				for (int j = 0; index != endIndex; j--)
				{
					index = (startIndex + j + listSize) % listSize;
					retList->InsertNextId(list->GetId(index));
				}
				//retList->InsertNextId(list->GetId(endIndex));
			}
			outLists.push_back(retList);
		}
	}
}





