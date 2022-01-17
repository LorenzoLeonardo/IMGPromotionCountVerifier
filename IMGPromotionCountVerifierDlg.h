
// IMGPromotionCountVerifierDlg.h : header file
//

#pragma once
#include "CListCtrlCustomized.h"
#include "CDialogReportRecruitsDlg.h"
#include "CDialogReportSalesDlg.h"
#include "CustomHeader.h"
#include "DropEdit.h"
#include "CTimeManipulation.h"
#include "CCustomClock.h"
#include "CParentPropertySheet.h"
// CIMGPromotionCountVerifierDlg dialog
class CIMGPromotionCountVerifierDlg : public CDialogEx
{
// Construction
public:
	CIMGPromotionCountVerifierDlg(CWnd* pParent = nullptr);	// standard constructor
	~CIMGPromotionCountVerifierDlg();
// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_IMGPROMOTIONCOUNTVERIFIER_DIALOG };
#endif
	void SetCMDLine(CString lpCmdLine)
	{
		m_csPVAFilePath = lpCmdLine;
	}
// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	DECLARE_MESSAGE_MAP()

	afx_msg virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg LRESULT OnSendPath(WPARAM wParam, LPARAM lParam);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnMcnSelchangeMonthcalendar1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBnClickedButtonVerify();
	afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnCbnSelchangeComboRundays();

private:

	INT CheckRawDataType(CString csRawData, INT *pnRows);
	void LoadLogo();
	void LaunchReportDlg(BOOL bIsPVA=FALSE);
	void DrawOpenIcon();

	CToolTipCtrl *m_tooltip;
	CComboBox m_ctrlComboBoxDays;
	CMonthCalCtrl m_ctrlStartCalendar;
	CMonthCalCtrl m_ctrlEndCalendar;
	CTimeManipulation m_timeManipulation;
	HICON m_hIcon;
	CCustomClock m_customClock;
	CToolBar m_ToolBar;
	HANDLE ghMutex;
	RECT m_rectOpenIcon;
	RECT m_rectIMGLogo;
	CString m_csPVAFilePath;
public:
	afx_msg void OnNMClickSyslink(NMHDR* pNMHDR, LRESULT* pResult);
	BOOL IsStopped()
	{
		return m_bStopThread;
	}
	void UpdateTime();
	void SetVersion(CString csVersion)
	{
		m_csVersion = csVersion;
	}
protected:
	CLinkCtrl m_ctrlSysHttps;
	CWinThread* m_pThread;
	CString m_csVersion;
public:
	afx_msg void OnClose();
	BOOL m_bStopThread;
protected:

	HBRUSH  m_hBrushBackGround;
	HBRUSH  m_hBrushEditCtrl;


	CDropEdit m_editRecruitSrc;
	CDropEdit m_editSalesSrc;
	CEdit m_ctrlEditRecPath;
	CEdit m_ctrlEditSalesPath;
	CString m_csRecRawData;
	CString m_csSalesRawData;


public:
	afx_msg void OnBnClickedButtonHelp();
	afx_msg void OnBnClickedButtonClear();
	afx_msg void OnEnChangeEditRecruit();
	afx_msg void OnEnChangeEditSale();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
private:
	TOOLINFO g_toolItem;
	HWND m_hwndTrackingTT;
	BOOL m_bTrackingMouse;
	CStatic m_staticVersion;
	HWND CreateTrackingToolTip(int toolID, HWND hDlg, TCHAR* pText);
public:
	afx_msg void OnBnClickedButtonBrowserecruit();
	afx_msg void OnBnClickedButtonBrowsesales();
	afx_msg void OnMouseHover(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg LRESULT OnNcHitTest(CPoint point);
	int ParsePVAFile(CString csFilePath);
private:
	CButton m_btnVerify;
public:
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
};