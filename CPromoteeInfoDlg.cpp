// CPromoteeInfoDlg.cpp : implementation file
//

#include "pch.h"
#include "IMGPromotionCountVerifier.h"
#include "CPromoteeInfoDlg.h"
#include "afxdialogex.h"
#include "CTimeManipulation.h"

// CPromoteeInfoDlg dialog

IMPLEMENT_DYNAMIC(CPromoteeInfoDlg, CDialogEx)

CPromoteeInfoDlg::CPromoteeInfoDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_PROMOTEE_INFO, pParent)
{

}

CPromoteeInfoDlg::~CPromoteeInfoDlg()
{
	DeleteObject(m_hBrushBackGround);
	DeleteObject(m_hBrushEditCtrl);
}

void CPromoteeInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_PROMOTEE_NAME, m_editCtrlName);
	DDX_Control(pDX, IDC_EDIT_PROMOTEE_CODE, m_editCtrlCode);
	DDX_Control(pDX, IDC_EDIT_UPLINE_NAME, m_editCtrlUplineName);
	DDX_Control(pDX, IDC_COMBO_RANK, m_dropListForms);
	DDX_Control(pDX, IDC_MONTHCALENDAR_COMPLETED, m_calendarDataCompleted);
	DDX_Control(pDX, IDC_COMBO_CURRENTRANK, m_dropListCurrentRank);
}


BEGIN_MESSAGE_MAP(CPromoteeInfoDlg, CDialogEx)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDOK, &CPromoteeInfoDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CPromoteeInfoDlg message handlers


HBRUSH CPromoteeInfoDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  Change any attributes of the DC here
	switch (nCtlColor)
	{
		case CTLCOLOR_STATIC:
		{
			if ((pWnd->GetDlgCtrlID() == IDC_EDIT_PROMOTEE_NAME) ||
				(pWnd->GetDlgCtrlID() == IDC_EDIT_PROMOTEE_CODE) || 
				(pWnd->GetDlgCtrlID() == IDC_EDIT_UPLINE_NAME))
			{
				pDC->SetTextColor(ENZO_COLOR_BLACK);
				pDC->SetBkColor(ENZO_COLOR_DIRTYWHITE);
				pDC->SetBkMode(TRANSPARENT);

				return m_hBrushEditCtrl;
			}
			else
			{
				pDC->SetTextColor(ENZO_COLOR_WHITE);
				pDC->SetBkColor(ENZO_COLOR_NAVYBLUE);
				return m_hBrushBackGround;
			}
		}
		case CTLCOLOR_DLG:
		{
			pDC->SetTextColor(ENZO_COLOR_WHITE);
			pDC->SetBkColor(ENZO_COLOR_NAVYBLUE);
			return m_hBrushBackGround;
		}

		default:
			return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	}
}


BOOL CPromoteeInfoDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	m_hBrushBackGround = CreateSolidBrush(ENZO_COLOR_NAVYBLUE);
	m_hBrushEditCtrl = CreateSolidBrush(ENZO_COLOR_DIRTYWHITE);
	
	m_dropListForms.AddString(_T("MD FORM"));
	m_dropListForms.AddString(_T("SMD FORM"));
	m_dropListForms.AddString(_T("EMD FORM"));
	m_dropListForms.AddString(_T("GJ FORM"));
	m_dropListForms.AddString(_T("CEO FORM"));
	m_dropListForms.AddString(_T("LIMITED EDITION PIN FORM"));
	m_dropListForms.AddString(_T("FREE HMO FORM"));
	m_dropListForms.AddString(_T("FREE OFFICE FORM"));
	CString csSelected;

	switch (_ttoi(m_vDetails[INDEX_RANK]))
	{
	case MD:
		csSelected = _T("MD FORM");
		break;
	case SMD:
		csSelected = _T("SMD FORM");
		break;
	case EMD:
		csSelected = _T("EMD FORM");
		break;
	case GJ:
		csSelected = _T("GJ FORM");
		break;
	case CEO:
		csSelected = _T("CEO FORM");
		break;
	case EVC:
		csSelected = _T("EVC FORM");
		break;
	case SEVC:
		csSelected = _T("SEVC FORM");
		break;
	case HMO:
		csSelected = _T("FREE HMO FORM");
		break;
	case LIMITEDPIN:
		csSelected = _T("LIMITED EDITION PIN FORM");
		break;
	case OFFICE:
		csSelected = _T("FREE OFFICE FORM");
		break;
	}
	m_dropListForms.SetCurSel(m_dropListForms.FindString(0, csSelected));

	m_dropListCurrentRank.AddString(_T("TA"));
	m_dropListCurrentRank.AddString(_T("A"));
	m_dropListCurrentRank.AddString(_T("MD"));
	m_dropListCurrentRank.AddString(_T("SMD"));
	m_dropListCurrentRank.AddString(_T("EMD"));
	m_dropListCurrentRank.AddString(_T("CEO"));
	m_dropListCurrentRank.AddString(_T("EVC"));
	m_dropListCurrentRank.AddString(_T("SEVC"));
	m_dropListCurrentRank.SetCurSel(m_dropListCurrentRank.FindString(0, m_vDetails[INDEX_CURRENTRANK]));

	m_editCtrlName.SetWindowText(m_vDetails[INDEX_NAME]);
	m_editCtrlCode.SetWindowText(m_vDetails[INDEX_CODE]);
	m_editCtrlUplineName.SetWindowText(m_vDetails[INDEX_UPLINE]);

	SYSTEMTIME sysTime;
	CTimeManipulation cTime;
	cTime.CStringToTime(m_vDetails[INDEX_DATE], &sysTime);
	m_calendarDataCompleted.SetCurSel(&sysTime);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}


void CPromoteeInfoDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	CString csData;

	m_editCtrlName.GetWindowText(csData);
	m_vDetails[INDEX_NAME] = csData;
	m_editCtrlCode.GetWindowText(csData);
	m_vDetails[INDEX_CODE] = csData;
	m_editCtrlUplineName.GetWindowText(csData);
	m_vDetails[INDEX_UPLINE] = csData;

	m_dropListForms.GetWindowText(csData);
	if (csData.Compare(_T("MD FORM")) == 0)
	{
		csData.Format(_T("%d"), MD);
	}
	else if (csData.Compare(_T("SMD FORM")) == 0)
	{
		csData.Format(_T("%d"), SMD);
	}
	else if (csData.Compare(_T("EMD FORM")) == 0)
	{
		csData.Format(_T("%d"), EMD);
	}
	else if (csData.Compare(_T("GJ FORM")) == 0)
	{
		csData.Format(_T("%d"), GJ);
	}
	else if (csData.Compare(_T("CEO FORM")) == 0)
	{
		csData.Format(_T("%d"), CEO);
	}
	else if (csData.Compare(_T("LIMITED EDITION PIN FORM")) == 0)
	{
		csData.Format(_T("%d"), LIMITEDPIN);
	}
	else if (csData.Compare(_T("FREE HMO FORM")) == 0)
	{
		csData.Format(_T("%d"), HMO);
	}
	else if (csData.Compare(_T("FREE OFFICE FORM")) == 0)
	{
		csData.Format(_T("%d"), OFFICE);
	}
	m_vDetails[INDEX_RANK] = csData;
			
	SYSTEMTIME completedDate;
	m_calendarDataCompleted.GetCurSel(&completedDate);
	csData.Format(_T("%02d/%02d/%04d"), completedDate.wMonth, completedDate.wDay, completedDate.wYear);
	m_vDetails[INDEX_DATE] = csData;

	m_dropListCurrentRank.GetWindowText(csData);
	m_vDetails[INDEX_CURRENTRANK] = csData;

	CDialogEx::OnOK();
}
