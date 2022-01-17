// CPlanHoldersInformationDlg.cpp : implementation file
//

#include "pch.h"
#include "CPlanHoldersInformationDlg.h"
#include "afxdialogex.h"


// CPlanHoldersInformationDlg dialog

IMPLEMENT_DYNAMIC(CPlanHoldersInformationDlg, CDialogEx)

CPlanHoldersInformationDlg::CPlanHoldersInformationDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_CLIENT_INFO, pParent)
{

}

CPlanHoldersInformationDlg::~CPlanHoldersInformationDlg()
{
	DeleteObject(m_hBrushBackGround);
	DeleteObject(m_hBrushEditCtrl);
}

void CPlanHoldersInformationDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_NAME, m_ctrlEditPlanHolderName);
	DDX_Control(pDX, IDC_EDIT_APPNUM, m_ctrlEditApplicationNumber);
	DDX_Control(pDX, IDC_EDIT_POLICYNUM, m_ctrlEditPolicyNumber);
	DDX_Control(pDX, IDC_EDIT_PLANNAME, m_ctrlEditPlanName);
	DDX_Control(pDX, IDC_EDIT_AGENT1, m_ctrlEditAgent1);
	DDX_Control(pDX, IDC_EDIT_AGENT2, m_ctrlEditAgent2);
	DDX_Control(pDX, IDC_EDIT_BASESHOP, m_ctrlEditBaseshop);
	DDX_Control(pDX, IDC_EDIT_HIERARCHY, m_ctrlEditHierarchy);
	DDX_Control(pDX, IDC_CHECK_BAP, m_btnBAP);
	DDX_Control(pDX, IDC_EDIT_AGENT_SMD, m_ctrlEditSMDName);
}


BEGIN_MESSAGE_MAP(CPlanHoldersInformationDlg, CDialogEx)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDOK, &CPlanHoldersInformationDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CPlanHoldersInformationDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// CPlanHoldersInformationDlg message handlers
BOOL CPlanHoldersInformationDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	m_hBrushBackGround = CreateSolidBrush(ENZO_COLOR_NAVYBLUE);
	m_hBrushEditCtrl = CreateSolidBrush(ENZO_COLOR_DIRTYWHITE);

	SetBackgroundColor(ENZO_COLOR_NAVYBLUE);
	m_ctrlEditPlanHolderName.SetWindowText((*m_pVData)[SALE_HEADER_PLANHOLDER]);
	m_ctrlEditApplicationNumber.SetWindowText((*m_pVData)[SALE_HEADER_APPNUMBER]);
	m_ctrlEditPolicyNumber.SetWindowText((*m_pVData)[SALE_HEADER_POLICYNUMBER]);
	m_ctrlEditPlanName.SetWindowText((*m_pVData)[SALE_HEADER_PLANNAME]);
	m_ctrlEditAgent1.SetWindowText((*m_pVData)[SALE_HEADER_AGENTNAME1]);
	m_ctrlEditAgent2.SetWindowText((*m_pVData)[SALE_HEADER_AGENTNAME2]);
	m_ctrlEditBaseshop.SetWindowText((*m_pVData)[SALE_HEADER_BASESHOP]);
	m_ctrlEditHierarchy.SetWindowText((*m_pVData)[SALE_HEADER_HIERARCHY]);
	m_ctrlEditSMDName.SetWindowText((*m_pVData)[SALE_HEADER_SMD]);

	if ((*m_pVData)[SALE_HEADER_BAP].Compare(_T("YES")) == 0)
	{
		m_btnBAP.SetCheck(TRUE);
	}
	else
	{
		m_btnBAP.SetCheck(FALSE);
	}

	return TRUE;
}

HBRUSH CPlanHoldersInformationDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	switch (nCtlColor)
	{
	case CTLCOLOR_STATIC:

		if ((pWnd->GetDlgCtrlID() == IDC_EDIT_NAME) ||
			(pWnd->GetDlgCtrlID() == IDC_EDIT_APPNUM) ||
			(pWnd->GetDlgCtrlID() == IDC_EDIT_POLICYNUM) ||
			(pWnd->GetDlgCtrlID() == IDC_EDIT_PLANNAME) ||
			(pWnd->GetDlgCtrlID() == IDC_EDIT_AGENT1) ||
			(pWnd->GetDlgCtrlID() == IDC_EDIT_AGENT2) ||
			(pWnd->GetDlgCtrlID() == IDC_EDIT_BASESHOP) ||
			(pWnd->GetDlgCtrlID() == IDC_EDIT_HIERARCHY)
			)
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
			return m_hBrushBackGround;
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


void CPlanHoldersInformationDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	CDialogEx::OnOK();

	if (m_btnBAP.GetCheck())
	{
		(*m_pVData)[SALE_HEADER_BAP] = _T("YES");
	}
	else
	{
		(*m_pVData)[SALE_HEADER_BAP] = _T("NO");
	}
	CString csSMDName;

	m_ctrlEditSMDName.GetWindowText(csSMDName);
	(*m_pVData)[SALE_HEADER_SMD] = csSMDName;
}


void CPlanHoldersInformationDlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	CDialogEx::OnCancel();
}
