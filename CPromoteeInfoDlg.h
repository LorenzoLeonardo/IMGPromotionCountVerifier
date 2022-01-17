#pragma once
#include "CustomHeader.h"
#include <vector>
// CPromoteeInfoDlg dialog

using namespace std;


class CPromoteeInfoDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CPromoteeInfoDlg)

public:
	CPromoteeInfoDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CPromoteeInfoDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_PROMOTEE_INFO };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
	CEdit m_editCtrlName;
	CEdit m_editCtrlCode;
	CEdit m_editCtrlUplineName;
	CComboBox m_dropListForms;
	CMonthCalCtrl m_calendarDataCompleted;
	HBRUSH 	m_hBrushBackGround;
	HBRUSH  m_hBrushEditCtrl;
	vector<CString> m_vDetails;
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	vector<CString> GetInformation()
	{
		return m_vDetails;
	}
	void SetInformation(vector<CString> vDetails)
	{
		m_vDetails = vDetails;
	}
protected:
	CComboBox m_dropListCurrentRank;
};
