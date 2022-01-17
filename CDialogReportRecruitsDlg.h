#pragma once
#include "CListCtrlCustomized.h"
#include "CTimeManipulation.h"
#include "CustomHeader.h"
#include <vector>
#include <algorithm>
#include <map>
#include "Resource.h"
#include "CQuickSort.h"
#include <afxdlgs.h>
using namespace std;

// CDialogReportRecruitsDlg dialog




class CDialogReportRecruitsDlg : public CMFCPropertyPage
{
	DECLARE_DYNAMIC(CDialogReportRecruitsDlg)

public:
	CDialogReportRecruitsDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CDialogReportRecruitsDlg();
	void SetRawData(CString, BOOL);
// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_REPORT_RECRUITS};
#endif
	void GetDataToPrint(vector<vector<CString>>& data);
	BOOL IsVisited()
	{
		return m_bIsVisited;
	}
	vector<vector<CString>> GetData()
	{
		return m_vRecruitDataStorage;
	}
protected:
	DECLARE_MESSAGE_MAP()
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	virtual afx_msg void OnNMClickListRecruit(NMHDR* pNMHDR, LRESULT* pResult);
	virtual afx_msg void OnRclickList(NMHDR* pNMHDR, LRESULT* pResult);
	virtual afx_msg void OnLvnItemchangedListRecruit(NMHDR* pNMHDR, LRESULT* pResult);
	virtual afx_msg void OnPaint();
	afx_msg void OnNMHoverListRecruit(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnNMDblclkListRecruit(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnMcnSelectMonthcalendarStart(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnMcnViewChangeMonthcalendarStart(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnMcnSelchangeMonthcalendarStart(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnCbnSelchangeComboRunDays();
	afx_msg void OnClose();
	afx_msg void OnBAP();

private:
	void DisplayRecData(CString csStartDate, CString csEndDate);
	void DisplaySMDLegCount(CString csStartDate, CString csEndDate);
	BOOL IsSMDNameExist(vector< SMDLEGCOUNTER>, CString, INT*);

	CEdit m_editCtrlDisplayCounter;
	CListCtrlCustomized m_ctrlListRecruit;
	CMonthCalCtrl m_ctrlStartCalendar;
	CMonthCalCtrl m_ctrlEndCalendar;
	CComboBox m_ctrlComboRunningDays;
	CTimeManipulation m_timeManipulation;
	CQuickSort m_qSort;
	//CMenu *pm_obMenu;
	//CMenu* m_pPopupMenu;
	CString m_csRawData;
	vector<SMDLEGCOUNTER> m_vSMDLegCounter;
	vector<vector<CString>> m_vRecruitDataStorage;
	vector<vector<CString>> ParseRecruitData(CString csRawData);
	CString m_szTabTitle;
	HBRUSH  m_hBrushBackGround;
	HBRUSH  m_hBrushEditCtrl;
	BOOL	m_bIsVisited;
	BOOL m_bIsPVA;

public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnBnClickedStaticTallyBoard();
protected:
	CStatic m_staticItems;
};
