#pragma once
#include <vector>
#include "CustomHeader.h"
using namespace std;

// CMembersProfileDlg dialog

class CMembersProfileDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CMembersProfileDlg)

public:
	CMembersProfileDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CMembersProfileDlg();
	void EditProfile(vector<CString>* pData)
	{
		m_pVData = pData;
	}
// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_PERSON_INFO };
#endif

protected:
	virtual HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	vector<CString> *m_pVData;
	DECLARE_MESSAGE_MAP()

protected:
	CEdit m_editSMD;
	CEdit m_editMemberCode;
	CEdit m_editMemberName;
	CButton m_btnBAP;
	CStatic m_staticName;
	CStatic m_staticCode;
	CStatic m_staticSMD;
	HBRUSH  m_hBrushBackGround;
	HBRUSH  m_hBrushEditCtrl;
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
};
