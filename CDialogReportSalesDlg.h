#pragma once
#include "CListCtrlCustomized.h"
#include "CTimeManipulation.h"
#include "CustomHeader.h"
#include <vector>
#include <algorithm>
#include <map>
#include "Resource.h"
#include "CQuickSort.h"

using namespace std;



class CDialogReportSalesDlg : public CMFCPropertyPage
{
	DECLARE_DYNAMIC(CDialogReportSalesDlg)

public:
	CDialogReportSalesDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CDialogReportSalesDlg();
	void CDialogReportSalesDlg::SetRawData(CString csRawData, BOOL bIsPVA)
	{
		m_csRawData = csRawData;
		m_bIsPVA = bIsPVA;
	}
	BOOL IsVisited()
	{
		return m_bIsVisited;
	}
	vector<vector<CString>> GetData()
	{
		return m_vSalesDataStorage;
	}
// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_REPORT_SALES };
#endif
	void GetDataToPrint(vector<vector<CString>>& data);
protected:
	DECLARE_MESSAGE_MAP()
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnClose();

	CString m_csRawData;
	CMonthCalCtrl m_ctrlStartCalendar;
	CMonthCalCtrl m_ctrlEndCalendar;
	CComboBox m_ctrlComboRunningDays;
	CListCtrlCustomized m_ctrlListSale;
	CEdit m_editCtrlDisplayCounter;
	CMenu* pm_obMenu;
	CMenu* m_pPopupMenu;
	BOOL m_bIsVisited;
	BOOL m_bIsPVA;

private:
	vector<vector<CString>> ParseRawData(CString csRawData);
	void DisplayRecData(CString csStartDate, CString csEndDate);
	void TallyCount(CString csStartDate, CString csEndDate);
	BOOL IsSMDNameExist(vector< SALESPOINTCOUNTER> vLegCounter, CString csSMDName, INT* pIndex)
	{
		BOOL bExist = FALSE;

		for (int i = 0; i < (int)vLegCounter.size(); i++)
		{
			if (!vLegCounter[i].csSMDName.Compare(csSMDName))
			{
				bExist = TRUE;
				*pIndex = i;
				break;
			}
		}
		return bExist;
	}
	vector<vector<CString>> m_vSalesDataStorage;
	CQuickSort m_qSort;
	CTimeManipulation m_timeManipulation;
	vector<SALESPOINTCOUNTER> m_vSMDLegCounter;

	HBRUSH  m_hBrushBackGround;
	HBRUSH  m_hBrushEditCtrl;

public:
	afx_msg void OnMcnSelchangeMonthcalendarStart(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnMcnSelectMonthcalendarStart(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCbnEditchangeComboRunDays();
	afx_msg void OnCbnSelchangeComboRunDays();
	afx_msg void OnNMHoverListSale(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnNMDblclkListSale(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
protected:
	CStatic m_staticItems;
};
