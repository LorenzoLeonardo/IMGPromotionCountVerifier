#pragma once

#include "resource.h"
#include <afxdlgs.h>
#include "CDialogReportRecruitsDlg.h"
#include "CDialogReportSalesDlg.h"
#include <vector>

using namespace std;

class CParentPropertySheet :
	public CPropertySheet
{
	//DECLARE_DYNAMIC(CParentPropertySheet)

public:
	CParentPropertySheet();
	CParentPropertySheet(CString csString);
	virtual ~CParentPropertySheet();

protected:
	DECLARE_MESSAGE_MAP()
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnPrint();
	afx_msg void OnHelp();
	afx_msg void OnSave();
	virtual BOOL OnInitDialog();
	CString GetCurrentDirectory();
	void GetAllWindowsFromProcessID(DWORD dwProcessID, std::vector <HWND>& vhWnds);
	INT LaunchPrintModule1();
	INT LaunchPrintModule2();
	INT LaunchPrintModule3();
	HBRUSH m_hBrushBackGround;
	vector<CString> m_vPromoteeInfo;
	HICON m_hIcon;
public:
	afx_msg void OnPaint();
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
};

