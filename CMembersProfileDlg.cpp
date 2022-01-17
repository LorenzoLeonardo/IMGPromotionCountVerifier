// CMembersProfileDlg.cpp : implementation file
//

#include "pch.h"
#include "IMGPromotionCountVerifier.h"
#include "CMembersProfileDlg.h"
#include "afxdialogex.h"

IMPLEMENT_DYNAMIC(CMembersProfileDlg, CDialogEx)

CMembersProfileDlg::CMembersProfileDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_PERSON_INFO, pParent)
{

}

CMembersProfileDlg::~CMembersProfileDlg()
{
	DeleteObject(m_hBrushBackGround);
	DeleteObject(m_hBrushEditCtrl);
}

void CMembersProfileDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECK_BAP, m_btnBAP);
	DDX_Control(pDX, IDC_EDIT_SMD, m_editSMD);
	DDX_Control(pDX, IDC_EDIT_CODE, m_editMemberCode);
	DDX_Control(pDX, IDC_EDIT_NAME, m_editMemberName);
	DDX_Control(pDX, IDC_STATIC_NAME, m_staticName);
	DDX_Control(pDX, IDC_STATIC_CODE, m_staticCode);
	DDX_Control(pDX, IDC_STATIC_SMD, m_staticSMD);
}


BEGIN_MESSAGE_MAP(CMembersProfileDlg, CDialogEx)
	ON_WM_CTLCOLOR()

	ON_BN_CLICKED(IDOK, &CMembersProfileDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CMembersProfileDlg::OnBnClickedCancel)
END_MESSAGE_MAP()

// CMembersProfileDlg message handlers
BOOL CMembersProfileDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_hBrushBackGround = CreateSolidBrush(ENZO_COLOR_NAVYBLUE);
	m_hBrushEditCtrl = CreateSolidBrush(ENZO_COLOR_DIRTYWHITE);

	SetBackgroundColor(ENZO_COLOR_NAVYBLUE);
	m_editSMD.SetWindowText((*m_pVData)[REC_HEADER_SMD]);
	m_editMemberCode.SetWindowText((*m_pVData)[REC_HEADER_AGENTCODE]);
	m_editMemberName.SetWindowText((*m_pVData)[REC_HEADER_NAME]);

	if ((*m_pVData)[REC_HEADER_BAP].Compare(_T("YES")) == 0)
	{
		m_btnBAP.SetCheck(TRUE);
	}
	else
	{
		m_btnBAP.SetCheck(FALSE);
	}

	return TRUE;
}

HBRUSH CMembersProfileDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	switch (nCtlColor)
	{
	case CTLCOLOR_STATIC:
		
		if ((pWnd->GetDlgCtrlID() == IDC_EDIT_NAME) ||
			(pWnd->GetDlgCtrlID() == IDC_EDIT_CODE) ||
			(pWnd->GetDlgCtrlID() == IDC_EDIT_SMD))
		{
			pDC->SetTextColor(ENZO_COLOR_BLACK);
			pDC->SetBkColor(ENZO_COLOR_DIRTYWHITE);
			pDC->SetBkMode(TRANSPARENT);
			return m_hBrushEditCtrl;
		}
		else if ((pWnd->GetDlgCtrlID() == IDC_CHECK_BAP))
		{
			pDC->SetTextColor(ENZO_COLOR_WHITE);
			pDC->SetBkColor(ENZO_COLOR_NAVYBLUE);
			return m_hBrushEditCtrl;
		}
		else
		{
			pDC->SetTextColor(ENZO_COLOR_WHITE);
			pDC->SetBkColor(ENZO_COLOR_NAVYBLUE);
			return m_hBrushBackGround;
		}

	default:
		return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	}
}

void CMembersProfileDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	CDialogEx::OnOK();
	m_editSMD.GetWindowText((*m_pVData)[REC_HEADER_SMD]);
	m_editMemberCode.GetWindowText((*m_pVData)[REC_HEADER_AGENTCODE]);
	m_editMemberName.GetWindowText((*m_pVData)[REC_HEADER_NAME]);

	if (m_btnBAP.GetCheck())
	{
		(*m_pVData)[REC_HEADER_BAP] = _T("YES");
	}
	else
	{
		(*m_pVData)[REC_HEADER_BAP] = _T("NO");
	}
}

void CMembersProfileDlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	CDialogEx::OnCancel();
}
