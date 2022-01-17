#include "pch.h"
#include "CQuickSort.h"


CQuickSort::CQuickSort()
{

}
CQuickSort::~CQuickSort()
{

}

void CQuickSort::SortVector(INT nLow, INT nHigh)
{
    if (nLow < nHigh)
    {
        /* pi is partitioning index, arr[p] is now
           at right place */
        int pi = PartitionVector(nLow, nHigh);

        // Separately sort elements before 
        // partition and after partition 
        SortVector(nLow, pi - 1);
        SortVector(pi + 1, nHigh);
    }
}

INT CQuickSort::PartitionVector(INT nLow, INT nHigh)
{
    vector<CString> pivot = (*m_pVectorArray)[nHigh];    // pivot 
    int i = (nLow - 1);  // Index of smaller element 

    for (int j = nLow; j <= nHigh - 1; j++)
    {
        // If current element is smaller than or 
        // equal to pivot 
        if (Comparator((*m_pVectorArray)[j][m_nColumn], pivot[m_nColumn]) <= 0)
        {
            i++;    // increment index of smallCer element 
           (*m_pVectorArray)[i].swap((*m_pVectorArray)[j]);
        }
    }
    (*m_pVectorArray)[i + 1].swap((*m_pVectorArray)[nHigh]);
    return (i + 1);
}

INT CQuickSort::Comparator(CString csA, CString csB)
{
    switch (m_nDataType)
    {
        case DATA_CSTRING:
        {
            return csA.Compare(csB);
        }

        case DATA_DATE:
        {
            return CompareDate(csA, csB);
        }

        case DATA_DOUBLE:
        {
            return _ttof(csA) - _ttof(csB);
        }
        default:
        {
            return csA.Compare(csB);
        }
    }
}

INT CQuickSort::CompareDate(CString a, CString b)
{
    // Comparing the years 
    INT nIndex1 = a.ReverseFind(_T('/'));
    INT nIndex2 = b.ReverseFind(_T('/'));
    CString str1 = a.Mid(nIndex1 + 1);
    CString str2 = b.Mid(nIndex2 + 1);
    INT nArg1 = _ttoi(str1);
    INT nArg2 = _ttoi(str2);

    if (nArg1 != nArg2)
    {
        return (nArg1 - nArg2);
    }

    // Comparing the months 
    CString month_sub_a = a.Left(a.Find(_T("/")));
    CString month_sub_b = b.Left(b.Find(_T("/")));

    nArg1 = _ttoi(month_sub_a);
    nArg2 = _ttoi(month_sub_b);
    if (nArg1 != nArg2)
    {
        return (nArg1 - nArg2);
    }

    // Comparing the days 
    a = a.Mid(a.Find(_T("/"))+1);
    b = b.Mid(b.Find(_T("/"))+1);

    CString day_a = a.Left(a.Find(_T("/")));
    CString day_b = b.Left(b.Find(_T("/")));

    nArg1 = _ttoi(day_a);
    nArg2 = _ttoi(day_b);

    return (nArg1 - nArg2);
}