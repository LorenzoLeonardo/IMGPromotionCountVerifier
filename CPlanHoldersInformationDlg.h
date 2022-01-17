#pragma once
#include "Resource.h"
#include "CustomHeader.h"
#include <vector>
// CPlanHoldersInformationDlg dialog

using namespace std;

class CPlanHoldersInformationDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CPlanHoldersInformationDlg)

public:
	CPlanHoldersInformationDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CPlanHoldersInformationDlg();
	void EditProfile(vector<CString>* pData)
	{
		m_pVData = pData;
	}
// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_CLIENT_INFO };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	vector<CString>* m_pVData;

	DECLARE_MESSAGE_MAP()
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
protected:
	CEdit m_ctrlEditPlanHolderName;
	CEdit m_ctrlEditApplicationNumber;
	CEdit m_ctrlEditPolicyNumber;
	CEdit m_ctrlEditPlanName;
	CEdit m_ctrlEditAgent1;
	CEdit m_ctrlEditAgent2;
	CEdit m_ctrlEditBaseshop;
	CEdit m_ctrlEditHierarchy;
	CButton m_btnBAP;
	HBRUSH  m_hBrushBackGround;
	HBRUSH  m_hBrushEditCtrl;
	CEdit m_ctrlEditSMDName;
};
