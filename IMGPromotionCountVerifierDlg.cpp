
// IMGPromotionCountVerifierDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "IMGPromotionCountVerifier.h"
#include "IMGPromotionCountVerifierDlg.h"
#include "CFileDirectory.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	void SetBackGroundColor(HBRUSH hColor)
	{
	 m_hBackColor = hColor;
	}
	void SetVersion(CString csVersion)
	{
		m_csVersion = csVersion;
	}
// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	HBRUSH m_hBackColor;
	CString m_csVersion;
// Implementation
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnNMClickSyslinkSite(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

//	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual BOOL OnInitDialog();
private:
	CStatic m_staticVersion;
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_VERSION, m_staticVersion);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
	ON_NOTIFY(NM_CLICK, IDC_SYSLINK_SITE, &CAboutDlg::OnNMClickSyslinkSite)
	ON_WM_CTLCOLOR()
	ON_WM_PAINT()
//	ON_WM_CREATE()
END_MESSAGE_MAP()

void CAboutDlg::OnNMClickSyslinkSite(NMHDR* pNMHDR, LRESULT* pResult)
{
	// TODO: Add your control notification handler code here
	ShellExecute(NULL, _T("open"), _T("https://7607hf.imgcorp.com/header/mybackground"), NULL, NULL, SW_SHOWNORMAL);
	*pResult = 0;
}

BOOL CAboutDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_staticVersion.SetWindowText(m_csVersion);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

HBRUSH CAboutDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	switch (nCtlColor)
	{
		case CTLCOLOR_STATIC:
		{
			pDC->SetTextColor(ENZO_COLOR_WHITE);
			pDC->SetBkColor(ENZO_COLOR_NAVYBLUE);
			return m_hBackColor;
		}
		case CTLCOLOR_DLG:
		{
			pDC->SetTextColor(ENZO_COLOR_WHITE);
			pDC->SetBkColor(ENZO_COLOR_NAVYBLUE);
			return m_hBackColor;
		}
	}
	return hbr;
}

CIMGPromotionCountVerifierDlg::CIMGPromotionCountVerifierDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_IMGPROMOTIONCOUNTVERIFIER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_csRecRawData = _T("");
	m_csSalesRawData = _T("");
	m_hBrushBackGround = CreateSolidBrush(ENZO_COLOR_NAVYBLUE);
	m_hBrushEditCtrl = CreateSolidBrush(ENZO_COLOR_DIRTYWHITE);
	m_tooltip = new CToolTipCtrl();
	m_tooltip->Create(this); // Made by Wizard
	m_customClock.SetFontStyle(_T("Yu Gothic UI"));
	m_customClock.SetFontSize(50);
	m_customClock.SetFontWeight(FW_NORMAL);
	m_customClock.SetTextColor(ENZO_COLOR_WHITE);
	m_customClock.SetTextBKColor(ENZO_COLOR_NAVYBLUE);
	m_customClock.CreateClock();
}
CIMGPromotionCountVerifierDlg::~CIMGPromotionCountVerifierDlg()
{
	delete m_tooltip;
	m_tooltip = NULL;
	DeleteObject(m_hBrushBackGround);
	DeleteObject(m_hBrushEditCtrl);
	m_customClock.DestroyClock();
	::DestroyWindow(m_hwndTrackingTT);
}

void CIMGPromotionCountVerifierDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_EDIT_RECRUIT, m_editRecruitSrc);
	DDX_Control(pDX, IDC_EDIT_PATH, m_ctrlEditRecPath);
	DDX_Control(pDX, IDC_COMBO_RUNDAYS, m_ctrlComboBoxDays);
	DDX_Control(pDX, IDC_MONTHCALENDAR1, m_ctrlStartCalendar);
	DDX_Control(pDX, IDC_MONTHCALENDAR_FUTURE, m_ctrlEndCalendar);
	DDX_Control(pDX, IDC_SYSLINK, m_ctrlSysHttps);
	DDX_Control(pDX, IDC_EDIT_SALE, m_editSalesSrc);
	DDX_Control(pDX, IDC_EDIT_PATH_SALES, m_ctrlEditSalesPath);
	DDX_Control(pDX, IDC_STATIC_VERSION, m_staticVersion);
	DDX_Control(pDX, IDC_BUTTON_VERIFY, m_btnVerify);
}

BEGIN_MESSAGE_MAP(CIMGPromotionCountVerifierDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_MESSAGE(WM_SEND_PATH, &CIMGPromotionCountVerifierDlg::OnSendPath)
	ON_WM_CTLCOLOR()
	ON_WM_DROPFILES()
	ON_CBN_SELCHANGE(IDC_COMBO_RUNDAYS, &CIMGPromotionCountVerifierDlg::OnCbnSelchangeComboRundays)
	ON_NOTIFY(MCN_SELCHANGE, IDC_MONTHCALENDAR1, &CIMGPromotionCountVerifierDlg::OnMcnSelchangeMonthcalendar1)
	ON_BN_CLICKED(IDC_BUTTON_VERIFY, &CIMGPromotionCountVerifierDlg::OnBnClickedButtonVerify)
	ON_WM_SIZING()
	ON_WM_SIZE()
	ON_NOTIFY(NM_CLICK, IDC_SYSLINK, &CIMGPromotionCountVerifierDlg::OnNMClickSyslink)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON_HELP, &CIMGPromotionCountVerifierDlg::OnBnClickedButtonHelp)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR, &CIMGPromotionCountVerifierDlg::OnBnClickedButtonClear)
	ON_EN_CHANGE(IDC_EDIT_RECRUIT, &CIMGPromotionCountVerifierDlg::OnEnChangeEditRecruit)
	ON_EN_CHANGE(IDC_EDIT_SALE, &CIMGPromotionCountVerifierDlg::OnEnChangeEditSale)
	ON_WM_CREATE()
	ON_BN_CLICKED(IDC_BUTTON_BROWSERECRUIT, &CIMGPromotionCountVerifierDlg::OnBnClickedButtonBrowserecruit)
	ON_BN_CLICKED(IDC_BUTTON_BROWSESALES, &CIMGPromotionCountVerifierDlg::OnBnClickedButtonBrowsesales)
	ON_WM_MOUSEHOVER()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_NCHITTEST()
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()

UINT MyThreadProc(LPVOID pParam)
{
	CIMGPromotionCountVerifierDlg* pObject = (CIMGPromotionCountVerifierDlg*)pParam;

	if (pObject == NULL)
		return 1;   // if pObject is not valid

	while (!pObject->IsStopped())
	{
		pObject->UpdateTime();
		SleepEx(60, FALSE);
	}
		// do something with 'pObject'
	return 0;   // thread completed successfully
}

BOOL CIMGPromotionCountVerifierDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.
	CString csDialogName;
	m_staticVersion.SetWindowText(m_csVersion);

	csDialogName.Format(_T("IMG Promotion Verifier Automation Tool - %s"), m_csVersion);
	this->SetWindowText(csDialogName);

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	m_hwndTrackingTT = CreateTrackingToolTip(IDD_IMGPROMOTIONCOUNTVERIFIER_DIALOG, this->GetSafeHwnd(), L"");
	m_bTrackingMouse = FALSE;
	::SendMessage(m_hwndTrackingTT, TTM_TRACKACTIVATE, (WPARAM)TRUE, (LPARAM)&g_toolItem);
	TCHAR coords[32];
	_tcscpy_s(coords, ARRAYSIZE(coords), _T("Opens a PVA File"));
	g_toolItem.lpszText = coords;
	::SendMessage(m_hwndTrackingTT, TTM_SETTOOLINFO, 0, (LPARAM)&g_toolItem);
	
	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	SetBackgroundColor(ENZO_COLOR_NAVYBLUE);
	m_editRecruitSrc.SetCallBackWindow(this->GetSafeHwnd(), IDC_EDIT_RECRUIT);
	m_editSalesSrc.SetCallBackWindow(this->GetSafeHwnd(), IDC_EDIT_SALE);
	m_ctrlComboBoxDays.AddString(_T("30"));
	m_ctrlComboBoxDays.AddString(_T("60"));
	m_ctrlComboBoxDays.AddString(_T("90"));
	m_ctrlComboBoxDays.AddString(_T("365"));
	m_ctrlComboBoxDays.SetCurSel(2);

	SYSTEMTIME todayTime;
	CString csString;

	m_ctrlComboBoxDays.GetWindowText(csString);
	m_ctrlStartCalendar.GetCurSel(&todayTime);
	m_timeManipulation.AddDays(&todayTime, _ttoi(csString));
	m_ctrlEndCalendar.SetCurSel(&todayTime);
	
	ghMutex = CreateMutex(
		NULL,              // default security attributes
		FALSE,             // initially not owned
		NULL);             // unnamed mutex

	m_bStopThread = FALSE;
	m_pThread = AfxBeginThread(MyThreadProc, this);

	if (!m_csPVAFilePath.IsEmpty())
		OnBnClickedButtonVerify();
	return TRUE;
}

void CIMGPromotionCountVerifierDlg::UpdateTime()
{
	DWORD dwWaitResult = WaitForSingleObject(
		ghMutex,    // handle to mutex
		INFINITE);

	CClientDC cdc(this);

	m_customClock.DrawClock(&cdc, 1080, 40);

	ReleaseMutex(ghMutex);
}

void CIMGPromotionCountVerifierDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		CString csDialogName;
		this->GetWindowText(csDialogName);

		dlgAbout.SetVersion(csDialogName);
		dlgAbout.SetBackGroundColor(m_hBrushBackGround);
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

void CIMGPromotionCountVerifierDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;
		
		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		UpdateTime();
		LoadLogo();
		DrawOpenIcon();
		CDialogEx::OnPaint();
	}
}

HCURSOR CIMGPromotionCountVerifierDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CIMGPromotionCountVerifierDlg::LoadLogo()
{
	CBitmap bmpHello;
	bmpHello.LoadBitmap(IDB_BITMAP_IMG_LOGO);
	CDC* pDC = NULL;
	
	pDC = this->GetDC();

	// Calculate bitmap size using a BITMAP structure.
	BITMAP bm;
	bmpHello.GetObject(sizeof(BITMAP), &bm);

	m_rectIMGLogo.left = 15;
	m_rectIMGLogo.top = 50;
	m_rectIMGLogo.right = ((double)bm.bmWidth * 1);
	m_rectIMGLogo.bottom = ((double)bm.bmHeight * 1);


	// Create a memory DC, select the bitmap into the
	// memory DC, and BitBlt it into the view.
	CDC dcMem;
	dcMem.CreateCompatibleDC(pDC);
	CBitmap* pbmpOld = dcMem.SelectObject(&bmpHello);
	pDC->SetStretchBltMode(HALFTONE);

	CBrush *pBrush = new CBrush();
	pBrush->Attach(GetStockObject(BLACK_BRUSH));

	pDC->SelectObject(pBrush);

	pDC->Ellipse(m_rectIMGLogo.left+5, m_rectIMGLogo.top+5, m_rectIMGLogo.right + m_rectIMGLogo.left-5, m_rectIMGLogo.bottom + m_rectIMGLogo.top -5);

	pDC->StretchBlt(m_rectIMGLogo.left, m_rectIMGLogo.top, m_rectIMGLogo.right, m_rectIMGLogo.bottom,
		&dcMem, 0, 0, bm.bmWidth, bm.bmHeight, SRCPAINT);

	delete pBrush;

	// Reselect the original bitmap into the memory DC.
	dcMem.SelectObject(pbmpOld);
	dcMem.DeleteDC();
	bmpHello.DeleteObject();
}

void CIMGPromotionCountVerifierDlg::DrawOpenIcon()
{
	CClientDC dc(this);
	CBitmap bmpIcon, bmpIconMask;
	BITMAP bm;
	CDC dcMem;
	CBitmap* pbmpOld = NULL;

	bmpIcon.LoadBitmap(IDB_BITMAP_OPEN_ICON2);
	bmpIconMask.LoadBitmap(IDB_BITMAP_OPEN_ICON_MASK2);
	bmpIcon.GetObject(sizeof(BITMAP), &bm);

	m_rectOpenIcon.left = 5;
	m_rectOpenIcon.top = 5;
	m_rectOpenIcon.right = 40;
	m_rectOpenIcon.bottom = 40;

	dcMem.CreateCompatibleDC(&dc);

	pbmpOld = dcMem.SelectObject(&bmpIconMask);
	dc.SetStretchBltMode(HALFTONE);

	dc.StretchBlt(m_rectOpenIcon.left, m_rectOpenIcon.top, m_rectOpenIcon.right, m_rectOpenIcon.bottom,
		&dcMem, 0, 0, bm.bmWidth, bm.bmHeight, SRCAND);
	dcMem.SelectObject(pbmpOld);

	pbmpOld = dcMem.SelectObject(&bmpIcon);

	dc.StretchBlt(m_rectOpenIcon.left, m_rectOpenIcon.top, m_rectOpenIcon.right, m_rectOpenIcon.bottom,
		&dcMem, 0, 0, bm.bmWidth, bm.bmHeight, SRCPAINT);

	// Reselect the original bitmap into the memory DC.
	dcMem.SelectObject(pbmpOld);
	dcMem.DeleteDC();
	bmpIcon.DeleteObject();
	bmpIconMask.DeleteObject();
}

HBRUSH CIMGPromotionCountVerifierDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	switch (nCtlColor)
	{
		case CTLCOLOR_STATIC:
		{
			if ((pWnd->GetDlgCtrlID() == IDC_EDIT_PATH) || (pWnd->GetDlgCtrlID() == IDC_EDIT_PATH_SALES))
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
		default:
		{
			DrawOpenIcon();
			return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
		}
	}

	return hbr;
}

void CIMGPromotionCountVerifierDlg::OnDropFiles(HDROP hDropInfo)
{
	// TODO: Add your message handler code here and/or call default

	CDialogEx::OnDropFiles(hDropInfo);
}

LRESULT CIMGPromotionCountVerifierDlg::OnSendPath(WPARAM wParam, LPARAM lParam)
{
	LRESULT lRes = 0;
	LPTSTR csPath = (LPTSTR)wParam;

	if (IDC_EDIT_RECRUIT == lParam)
	{
		m_csRecRawData.Empty();
		m_ctrlEditRecPath.SetWindowText(csPath);
	}
	else if (IDC_EDIT_SALE == lParam)
	{
		m_csSalesRawData.Empty();
		m_ctrlEditSalesPath.SetWindowText(csPath);
	}

	FILE* fStream;
	errno_t errCode = _tfopen_s(&fStream, csPath, _T("r, ccs=UTF-8"));
	if (0 != errCode)
	{
		return FALSE;
	}

	CStdioFile File(fStream);
	CString strLine;

	while (File.ReadString(strLine))
	{
		strLine += _T("\r\n");
		switch (lParam)
		{
		case IDC_EDIT_RECRUIT:
			m_csRecRawData += strLine;
			break;
		case IDC_EDIT_SALE:
			m_csSalesRawData += strLine;
			break;
		}

	}
	File.Close();

	m_editRecruitSrc.SetWindowText(m_csRecRawData);
	m_editSalesSrc.SetWindowText(m_csSalesRawData);

	return lRes;
}

void CIMGPromotionCountVerifierDlg::LaunchReportDlg(BOOL bIsPVA)
{
	if (m_csRecRawData.IsEmpty() || m_csSalesRawData.IsEmpty())
	{
		MessageBox(_T("There is no data entered. Please double check the format inside your IMG Portal."), _T("Format Error"), MB_ICONERROR);
		return;
	}
	if (!bIsPVA)
	{
		INT nTotalRows = 0;
		INT nTotalItems = CheckRawDataType(m_csRecRawData, &nTotalRows);
		INT nColumns = nTotalItems / nTotalRows;

		if (nTotalItems == 0)
		{
			MessageBox(_T("There is no data entered in the Recruit Side. Please double check the format inside your IMG Portal."), _T("Format Error"), MB_ICONERROR);
			return;
		}
		if (!(nColumns == (REC_HEADER_END - 2)) || (nColumns == (REC_HEADER_END - 1)))
		{
			MessageBox(_T("The data is invalid in the Recruit Side. Please double check the format that you copied from your IMG Portal."), _T("Format Error"), MB_ICONERROR);
			return;
		}

		nTotalItems = CheckRawDataType(m_csSalesRawData, &nTotalRows);
		nColumns = nTotalItems / nTotalRows;

		if (nTotalItems == 0)
		{
			MessageBox(_T("There is no data entered in the Sales Side. Please double check the format inside your IMG Portal."), _T("Format Error"), MB_ICONERROR);
			return;
		}
		if (nColumns < (SALE_HEADER_END - 2))
		{
			MessageBox(_T("The data is invalid in the Recruit Side. Please double check the format that you copied from your IMG Portal."), _T("Format Error"), MB_ICONERROR);
			return;
		}

		if (nColumns == (SALE_HEADER_END - 2))
		{
			MessageBox(_T("You have copied the IMG Productions table directly which doesn't have an SMD column. You can edit it through the MS-Excel file by adding a last column for SMD Name. You can also proceed to edit here manually."), _T("Warning : No SMD Column"), MB_ICONWARNING);
		}
	}
	CParentPropertySheet mySheet(_T("Reports Dialog"));
	CDialogReportRecruitsDlg dlgRecruit;
	CDialogReportSalesDlg dlgSales;

	mySheet.m_psh.dwFlags |= PSH_PROPTITLE;
	mySheet.m_psh.pszCaption = _T("Members and Productions Tally Board");

	dlgRecruit.SetRawData(m_csRecRawData, bIsPVA);
	dlgSales.SetRawData(m_csSalesRawData, bIsPVA);

	mySheet.AddPage(&dlgRecruit);
	mySheet.AddPage(&dlgSales);

	this->ShowWindow(FALSE);
	if (mySheet.DoModal() != IDOK)
	{
		m_csPVAFilePath.Empty();
		m_csRecRawData.Empty();
		m_csSalesRawData.Empty();
	}
	this->ShowWindow(TRUE);
}

INT CIMGPromotionCountVerifierDlg::CheckRawDataType(CString csRawData, INT *pnRows)
{
	if (csRawData.IsEmpty())
		return 0;

	INT nIndex = csRawData.Find(_T("\r\n"));
	INT nTotalItemCount = 0, nRows = 0;
	CString csLine;

	if (nIndex == -1)
	{
		while ((nIndex = csRawData.Find(_T("\t"))) != -1)
		{
			csRawData = csRawData.Mid(nIndex + 1);
			nTotalItemCount++;
			nIndex = csRawData.Find(_T("\t"));
		}
		if (!csRawData.IsEmpty())
			nTotalItemCount++;
		nRows++;
		*pnRows = nRows;
		return nTotalItemCount;
	}
	else
	{
		INT nNextLineIndex = 0;
		while ((nNextLineIndex = csRawData.Find(_T("\r\n"))) != -1)
		{
			csLine = csRawData.Left(nNextLineIndex);
			csRawData = csRawData.Mid(nNextLineIndex + 2);
			
			while ((nIndex = csLine.Find(_T("\t"))) != -1)
			{
				csLine = csLine.Mid(nIndex + 1);
				nTotalItemCount++;
				nIndex = csLine.Find(_T("\t"));
			}
			if (!csLine.IsEmpty())
				nTotalItemCount++;
			nRows++;
			*pnRows = nRows;
		}
		if (!csRawData.IsEmpty())
		{
			nIndex = csRawData.Find(_T("\t"));
			while (nIndex != -1)
			{
				csRawData = csRawData.Mid(nIndex + 1);
				nTotalItemCount++;
				nIndex = csRawData.Find(_T("\t"));
			}
			if (!csRawData.IsEmpty())
				nTotalItemCount++;
			nRows++;
			*pnRows = nRows;
		}
		return nTotalItemCount;
	}
}

void CIMGPromotionCountVerifierDlg::OnCbnSelchangeComboRundays()
{
	SYSTEMTIME todayTime;
	CString csString;

	m_ctrlComboBoxDays.GetWindowText(csString);
	m_ctrlStartCalendar.GetCurSel(&todayTime);
	m_timeManipulation.AddDays(&todayTime, _ttoi(csString));
	m_ctrlEndCalendar.SetCurSel(&todayTime);
}

void CIMGPromotionCountVerifierDlg::OnMcnSelchangeMonthcalendar1(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMSELCHANGE pSelChange = reinterpret_cast<LPNMSELCHANGE>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;

	SYSTEMTIME todayTime;
	CString csString;

	m_ctrlComboBoxDays.GetWindowText(csString);
	m_ctrlStartCalendar.GetCurSel(&todayTime);
	m_timeManipulation.AddDays(&todayTime, _ttoi(csString));
	m_ctrlEndCalendar.SetCurSel(&todayTime);
}

void CIMGPromotionCountVerifierDlg::OnBnClickedButtonVerify()
{
	if (!m_csPVAFilePath.IsEmpty())
	{
		m_csPVAFilePath.Replace(_T("\""), _T(""));
		if (ParsePVAFile(m_csPVAFilePath))
			LaunchReportDlg(TRUE);
		else
		{
			CString csErr;
			csErr.Format(_T("Path : %s\r\nPVA file format is invalid.\r\nPlease open another PVA file."), m_csPVAFilePath);
			MessageBox(csErr, _T("Error : Invalid File Format"), MB_ICONERROR);
		}
	}
	else
	{
		CString csRawData;

		m_editRecruitSrc.GetWindowText(csRawData);

		if (csRawData.GetLength() >= m_csRecRawData.GetLength())
			m_csRecRawData = csRawData;

		m_editSalesSrc.GetWindowText(csRawData);
		if (csRawData.GetLength() >= m_csSalesRawData.GetLength())
			m_csSalesRawData = csRawData;

		LaunchReportDlg();
	}
}

void CIMGPromotionCountVerifierDlg::OnSizing(UINT fwSide, LPRECT pRect)
{
	CDialogEx::OnSizing(fwSide, pRect);

	// TODO: Add your message handler code here
}

void CIMGPromotionCountVerifierDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);
}

void CIMGPromotionCountVerifierDlg::OnNMClickSyslink(NMHDR* pNMHDR, LRESULT* pResult)
{
	// TODO: Add your control notification handler code here
	ShellExecute(NULL, _T("open"), _T("www.img-corp.net"), NULL, NULL, SW_SHOWNORMAL);

	*pResult = 0;
}

void CIMGPromotionCountVerifierDlg::OnClose()
{
	// TODO: Add your message handler code here and/or call default
	m_bStopThread = TRUE;

	if (WaitForSingleObject(m_pThread->m_hThread, INFINITE) == WAIT_OBJECT_0)
	{
		CloseHandle(ghMutex);
		CDialogEx::OnClose();
	}
	else
	{
		CloseHandle(ghMutex);
		MessageBox(_T("Thread Exception!!!!"), _T("Thread Error"), MB_ICONERROR);
		CDialogEx::OnClose();
	}//report error
}

void CIMGPromotionCountVerifierDlg::OnBnClickedButtonHelp()
{
	ShellExecute(NULL, _T("open"), _T("https://m.me/Lorenzo.Leonardo.92"), NULL, NULL, SW_SHOWNORMAL);
}

void CIMGPromotionCountVerifierDlg::OnBnClickedButtonClear()
{
	m_csRecRawData.Empty();
	m_ctrlEditRecPath.SetWindowText(_T(""));
	m_csSalesRawData.Empty();
	m_ctrlEditSalesPath.SetWindowText(_T(""));
	m_editRecruitSrc.SetWindowText(_T(""));
	m_editSalesSrc.SetWindowText(_T(""));
	m_csPVAFilePath.Empty();
}

void CIMGPromotionCountVerifierDlg::OnEnChangeEditRecruit()
{
	m_csRecRawData.Empty();
	m_ctrlEditRecPath.SetWindowText(_T(""));
}


void CIMGPromotionCountVerifierDlg::OnEnChangeEditSale()
{
	m_csSalesRawData.Empty();
	m_ctrlEditSalesPath.SetWindowText(_T(""));
}

int CIMGPromotionCountVerifierDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	
	return 0;
}

void CIMGPromotionCountVerifierDlg::OnBnClickedButtonBrowserecruit()
{
	CFileDirectory cFile;
	CString csFile = cFile.OpenFile();

	::SendMessage(this->GetSafeHwnd(), WM_SEND_PATH, (WPARAM)csFile.GetString(), IDC_EDIT_RECRUIT);
}

void CIMGPromotionCountVerifierDlg::OnBnClickedButtonBrowsesales()
{
	CFileDirectory cFile;
	CString csFile = cFile.OpenFile();

	::SendMessage(this->GetSafeHwnd(), WM_SEND_PATH, (WPARAM)csFile.GetString(), IDC_EDIT_SALE);
}

void CIMGPromotionCountVerifierDlg::OnMouseHover(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	CDialogEx::OnMouseHover(nFlags, point);
}



void CIMGPromotionCountVerifierDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	HWND hDlg = this->GetSafeHwnd();
	// TODO: Add your message handler code here and/or call default
	if ((m_rectOpenIcon.left <= point.x) && (point.x <= m_rectOpenIcon.right) &&
		(m_rectOpenIcon.top <= point.y) && (point.y <= m_rectOpenIcon.bottom))
	{
		SetCursor(LoadCursor(NULL, IDC_HAND));

		if (!m_bTrackingMouse)   // The mouse has just entered the window.
		{                       // Request notification when the mouse leaves.
			POINT pt = { m_rectOpenIcon.right, m_rectOpenIcon.bottom };
			
			::SendMessage(m_hwndTrackingTT, TTM_TRACKACTIVATE, (WPARAM)TRUE, (LPARAM)&g_toolItem);
					::ClientToScreen(hDlg, &pt);
			::SendMessage(m_hwndTrackingTT, TTM_TRACKPOSITION, 0, (LPARAM)MAKELONG(pt.x, pt.y));
			m_bTrackingMouse = TRUE;
		}
	}
	else
	{
		SetCursor(LoadCursor(NULL, IDC_ARROW));
		::SendMessage(m_hwndTrackingTT, TTM_TRACKACTIVATE, (WPARAM)FALSE, (LPARAM)&g_toolItem);
		m_bTrackingMouse = FALSE;
	}
	CDialogEx::OnMouseMove(nFlags, point);
}


void CIMGPromotionCountVerifierDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	if ((m_rectOpenIcon.left <= point.x) && (point.x <= m_rectOpenIcon.right) &&
		(m_rectOpenIcon.top <= point.y) && (point.y <= m_rectOpenIcon.bottom))
	{
		::SendMessage(m_hwndTrackingTT, TTM_TRACKACTIVATE, (WPARAM)FALSE, (LPARAM)&g_toolItem);
		m_bTrackingMouse = TRUE;

		CFileDirectory cFile;
		CString csFile = cFile.OpenPVAFile();

		if (csFile.IsEmpty())
			return;

		m_csRecRawData.Empty();
		m_csSalesRawData.Empty();
		m_editRecruitSrc.SetWindowText(_T(""));
		m_editSalesSrc.SetWindowText(_T(""));
		m_ctrlEditRecPath.SetWindowText(_T(""));
		m_ctrlEditSalesPath.SetWindowText(_T(""));

		if(ParsePVAFile(csFile))
			LaunchReportDlg(TRUE);
		else
			MessageBox(_T("PVA file format is invalid.\r\nPlease open another PVA file."), _T("Error : Invalid File Format"), MB_ICONERROR);
	}
	CDialogEx::OnLButtonDown(nFlags, point);
}


LRESULT CIMGPromotionCountVerifierDlg::OnNcHitTest(CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	return CDialogEx::OnNcHitTest(point);
}


int CIMGPromotionCountVerifierDlg::ParsePVAFile(CString csFilePath)
{
	// TODO: Add your implementation code here.
	FILE* fStream;
	errno_t errCode = _tfopen_s(&fStream, csFilePath, _T("r, ccs=UTF-8"));
	if (0 != errCode)
	{
		TCHAR buf[265];
		ZeroMemory(buf, sizeof(buf));
		FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL, GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			buf, (sizeof(buf) / sizeof(wchar_t)), NULL);

		CString csErr;
		csErr.Format(_T("Path : %s\r\n\r\n%s"), csFilePath, buf);

		MessageBox(csErr, _T("Error : ParsePVAFile()"), MB_ICONERROR);
		return FALSE;
	}

	CStdioFile File(fStream);
	CString strLine;


	//Header
	if (!File.ReadString(strLine))
	{
		File.Close();
		return FALSE;
	}
	strLine.Trim();
	if (strLine.CompareNoCase(_T("<RECRUITS>")) != 0)
	{
		File.Close();
		return FALSE;
	}
	strLine = _T("");
	m_csRecRawData = _T("");
	while (File.ReadString(strLine))
	{
		strLine.Trim();
		if (!strLine.CompareNoCase(_T("</RECRUITS>")))
			break;
		m_csRecRawData += strLine + _T("\r\n");
	}

	if (!File.ReadString(strLine))
	{
		File.Close();
		return FALSE;
	}
	strLine.Trim();
	if (strLine.CompareNoCase(_T("<SALES>")) != 0)
	{
		File.Close();
		return FALSE;
	}
	strLine = _T("");
	m_csSalesRawData = _T("");
	while (File.ReadString(strLine))
	{
		strLine.Trim();
		if (!strLine.CompareNoCase(_T("</SALES>")))
			break;
		m_csSalesRawData += strLine + _T("\r\n");
	}

	File.Close();

	return TRUE;
}


void CIMGPromotionCountVerifierDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);
}


HWND CIMGPromotionCountVerifierDlg::CreateTrackingToolTip(int toolID, HWND hDlg, WCHAR* pText)
{
	// Create a tooltip.
	HWND hwndTT = CreateWindowEx(WS_EX_TOPMOST, TOOLTIPS_CLASS, NULL,
		WS_POPUP | TTS_NOPREFIX | TTS_ALWAYSTIP,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		hDlg, NULL, AfxGetApp()->m_hInstance, NULL);

	if (!hwndTT)
	{
		return NULL;
	}

	// Set up the tool information. In this case, the "tool" is the entire parent window.

	g_toolItem.cbSize = sizeof(TOOLINFO);
	g_toolItem.uFlags = TTF_IDISHWND | TTF_TRACK | TTF_ABSOLUTE;
	g_toolItem.hwnd = hDlg;
	g_toolItem.hinst = AfxGetApp()->m_hInstance;
	g_toolItem.lpszText = pText;
	g_toolItem.uId = (UINT_PTR)hDlg;

	::GetClientRect(this->GetSafeHwnd(), &g_toolItem.rect);

	// Associate the tooltip with the tool window.

	::SendMessage(hwndTT, TTM_ADDTOOL, 0, (LPARAM)(LPTOOLINFO)&g_toolItem);

	return hwndTT;
}
