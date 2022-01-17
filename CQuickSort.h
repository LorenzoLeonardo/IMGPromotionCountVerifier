#pragma once
#include <vector>
#include "CustomHeader.h"

using namespace std;

enum
{
	DATA_CSTRING = 0,
	DATA_DATE,
	DATA_DOUBLE
};
class CQuickSort
{
public:
	CQuickSort();
	~CQuickSort();
	void SetDataToSort(vector<vector<CString>> *pData, INT nColumn)
	{
		m_pVectorArray = pData;
		m_nColumn = nColumn;
		if ((nColumn == REC_HEADER_DATE) || (nColumn == SALE_HEADER_BUSINESSDATE))
			m_nDataType = DATA_DATE;
		else if (nColumn == 1)
			m_nDataType = DATA_DOUBLE;
		else
			m_nDataType = DATA_CSTRING;
	}
	void SortVector(INT , INT );
	INT PartitionVector(INT, INT);
	INT CompareDate(CString, CString);

protected:
	INT Comparator(CString, CString);

private:

	vector<vector<CString>> * m_pVectorArray;
	INT m_nColumn;
	INT m_nDataType;

};

