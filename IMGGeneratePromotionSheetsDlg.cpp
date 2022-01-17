
// IMGGeneratePromotionSheetsDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "IMGGeneratePromotionSheetsDlg.h"
#include "CPrintDialogCustomized.h"
#include "afxdialogex.h"
#include <algorithm>  
#include "CFileDirectory.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


CIMGGeneratePromotionSheetsDlg::CIMGGeneratePromotionSheetsDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_IMGGENERATEPROMOTIONSHEETS_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_hBrushPrevBackGround = CreateSolidBrush(ENZO_COLOR_DARKGRAY);
	m_hBrushBackGround = CreateSolidBrush(ENZO_COLOR_NAVYBLUE);
	m_nPage = 0;
	m_nPagesToPrint = 0;
}

CIMGGeneratePromotionSheetsDlg::~CIMGGeneratePromotionSheetsDlg()
{
	for (int i = 0; i < (int)m_vCBitmaps.size(); i++)
	{
		m_vCBitmaps[i]->DeleteObject();
		delete m_vCBitmaps[i];
	}
	m_vCBitmaps.clear();
	DeleteObject(m_hBrushPrevBackGround);
	DeleteObject(m_hBrushBackGround);
}

void CIMGGeneratePromotionSheetsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_PRINT_PREVIEW, m_ctrlPreview);
	DDX_Control(pDX, IDC_BUTTON_PREV, m_btnPrevious);
	DDX_Control(pDX, IDC_BUTTON_NEXT, m_btnNext);
	DDX_Control(pDX, IDC_BUTTON_PRINT, m_btnPrint);
	DDX_Control(pDX, IDC_STATIC_PAGE, m_staticTextPage);
	DDX_Control(pDX, IDC_BUTTON_REFRESH, m_btnRefresh);
}

BEGIN_MESSAGE_MAP(CIMGGeneratePromotionSheetsDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BUTTON_PREV, &CIMGGeneratePromotionSheetsDlg::OnBnClickedButtonPrev)
	ON_BN_CLICKED(IDC_BUTTON_PRINT, &CIMGGeneratePromotionSheetsDlg::OnBnClickedButtonPrint)
	ON_BN_CLICKED(IDC_BUTTON_NEXT, &CIMGGeneratePromotionSheetsDlg::OnBnClickedButtonNext)
	ON_WM_MOUSEWHEEL()
	ON_BN_CLICKED(IDC_BUTTON_REFRESH, &CIMGGeneratePromotionSheetsDlg::OnBnClickedButtonRefresh)
END_MESSAGE_MAP()

BOOL CIMGGeneratePromotionSheetsDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	// Add "About..." menu item to system menu.
	SetWindowSize();
	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CString csDialogName = _T("Print Preview");

	//csDialogName.Format(_T("IMG Print Promotion Sheets - %s"), m_csVersion);
	this->SetWindowText(csDialogName);

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	SetBackgroundColor(ENZO_COLOR_NAVYBLUE);


	if (!LoadIMGData())//Load IMG Promotee data for submission
	{
		TCHAR buf[265];
		ZeroMemory(buf, sizeof(buf));
		FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL, GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			buf, (sizeof(buf) / sizeof(wchar_t)), NULL);
		MessageBox(buf, _T("Error : LoadIMGData()"), MB_ICONERROR);
		OnCancel();
		return FALSE;
	}
	if (!InitializePrintMatrix())
	{
		TCHAR buf[265];
		ZeroMemory(buf, sizeof(buf));
		FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL, GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			buf, (sizeof(buf) / sizeof(wchar_t)), NULL);
		MessageBox(buf, _T("Error : InitializePrintMatrix()"), MB_ICONERROR);
		OnCancel();
		return FALSE;
	}
	if (!LoadBMPTemplates()) // Load form templates
	{
		TCHAR buf[265];
		ZeroMemory(buf, sizeof(buf));
		FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL, GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			buf, (sizeof(buf) / sizeof(wchar_t)), NULL);
		MessageBox(buf, _T("Error : LoadBMPTemplates()"), MB_ICONERROR);
		OnCancel();
		return FALSE;
	}
	WriteDataToImage();

	m_vListForm.push_back(_T("MD"));
	m_vListForm.push_back(_T("SMD"));
	m_vListForm.push_back(_T("GJ"));
	m_vListForm.push_back(_T("CEO"));
	m_vListForm.push_back(_T("EVC"));
	m_vListForm.push_back(_T("SEVC"));
	m_vListForm.push_back(_T("OFFICE"));
	m_vListForm.push_back(_T("HMO"));
	m_vListForm.push_back(_T("LIMITEDPIN"));
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CIMGGeneratePromotionSheetsDlg::SetWindowSize()
{
	RECT rect;
	int nMarginX = 25, nMarginY = 10 , nXPosView = 0;
	double lfWidth = 0.0 , lfHeight = 0.0, lfHeightPreview = 0.0, lfWidthPreview = 0.0;

	GetDesktopWindow()->GetClientRect(&rect);

	lfHeight = ((double)rect.bottom + 35);
	lfWidth = (int)(lfHeight * 0.9); //set the width at 90% of the window height;

	this->SetWindowPos(this, 0, 0, (int)lfWidth, (int)lfHeight, SWP_NOMOVE | SWP_NOZORDER | SWP_NOOWNERZORDER);

	lfHeightPreview = lfHeight * 0.875; //set the height at 87.5% of the parent window;
	lfWidthPreview = lfWidth * 0.875; //set the width at 87.5% of the parent window;
	nXPosView = (int)((lfWidth - lfWidthPreview) * 0.4); // put the horizontal at the center

	m_ctrlPreview.SetWindowPos(this, nXPosView, nMarginY, (int)lfWidthPreview, (int)lfHeightPreview, SWP_NOZORDER | SWP_NOOWNERZORDER);
	
	m_btnPrevious.SetWindowPos(this, nXPosView, nMarginY + (int)lfHeightPreview + 10, 60, 20, SWP_NOZORDER | SWP_NOOWNERZORDER);
	m_btnNext.SetWindowPos(this, nXPosView + 70, nMarginY + (int)lfHeightPreview + 10, 60, 20, SWP_NOZORDER | SWP_NOOWNERZORDER);
	m_btnPrint.SetWindowPos(this, nXPosView + 140, nMarginY + (int)lfHeightPreview + 10, 60, 20, SWP_NOZORDER | SWP_NOOWNERZORDER);
	m_btnRefresh.SetWindowPos(this, nXPosView + 210, nMarginY + (int)lfHeightPreview + 10, 60, 20, SWP_NOZORDER | SWP_NOOWNERZORDER);
	m_staticTextPage.SetWindowPos(this, nXPosView + (int)lfWidthPreview - 60, nMarginY + (int)lfHeightPreview + 10, 60, 20, SWP_NOZORDER | SWP_NOOWNERZORDER);
}

void CIMGGeneratePromotionSheetsDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{

	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

void CIMGGeneratePromotionSheetsDlg::OnPaint()
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
		DisplayPreview();
		CDialogEx::OnPaint();
	}
}

HCURSOR CIMGGeneratePromotionSheetsDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

BOOL CIMGGeneratePromotionSheetsDlg::DisplayPreview()
{
	if (m_vCBitmaps.empty())
		return FALSE;

	CClientDC dc(&m_ctrlPreview);
	CDC bmDC;
	CString csPage;
	RECT rect;
	CBitmap* pOldbmp = NULL;
	BITMAP  bi;

	bmDC.CreateCompatibleDC(&dc);
	pOldbmp = bmDC.SelectObject(m_vCBitmaps[m_nPage]);
	m_vCBitmaps[m_nPage]->GetBitmap(&bi);
	dc.SetStretchBltMode(HALFTONE);
	GetRectRatio(m_nPage , &rect);
	dc.StretchBlt(rect.left, rect.top, rect.right, rect.bottom, &bmDC, 0, 0, bi.bmWidth, bi.bmHeight, SRCCOPY);
	bmDC.SelectObject(pOldbmp);
	bmDC.DeleteDC();

	csPage.Format(_T("Page %d/%d"), m_nPage + 1, m_vCBitmaps.size());
	m_staticTextPage.SetWindowText(csPage);

	return TRUE;
}
void CIMGGeneratePromotionSheetsDlg::GetRectRatio(int nItem, LPRECT lpRect)
{
	RECT rectContainer;
	BITMAP  bi;
	ZeroMemory(lpRect, sizeof(RECT));

	if (m_vCBitmaps.empty())
		return;

	m_ctrlPreview.GetClientRect(&rectContainer);

	m_vCBitmaps[nItem]->GetBitmap(&bi);

	int imageWidth = bi.bmWidth;
	int imageHeight = bi.bmHeight;

	double fimageWidth = (double)imageWidth;
	double fimageHeight = (double)imageHeight;

	// Compute the coordinates of the upper left corner of the centered bitmap.
	double fContainerWidth = (double)rectContainer.right;
	double fContainerHeight = (double)rectContainer.bottom;
	double fbitmapAspectRatio = fimageWidth / fimageHeight;

	if (fimageWidth < fimageHeight)
	{
		fbitmapAspectRatio = fimageWidth / fimageHeight;
		fimageHeight = fContainerHeight;
		fimageWidth = fimageHeight * fbitmapAspectRatio;
	}
	else
	{
		fbitmapAspectRatio = fimageHeight / fimageWidth;
		fimageWidth = fContainerWidth;
		fimageHeight = fimageWidth * fbitmapAspectRatio;
	}
	int newX = (int)((fimageWidth));
	int newY = (int)((fimageHeight));

	int nPosX = ((int)((fContainerWidth - fimageWidth))) / 2;
	int nPosY = ((int)((fContainerHeight - fimageHeight))) / 2;

	double xMargin = 5;
	double yMargin = ((double)xMargin * fbitmapAspectRatio);
	lpRect->left = (double)nPosX + xMargin;
	lpRect->top = (double)nPosY + yMargin;

	lpRect->right = newX - (xMargin * 2);
	lpRect->bottom = newY - (yMargin * 2);
}
HBRUSH CIMGGeneratePromotionSheetsDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	switch (nCtlColor)
	{
		case CTLCOLOR_STATIC:
		{
			if ((pWnd->GetDlgCtrlID() == IDC_STATIC_PRINT_PREVIEW))
			{
				pDC->SetBkMode(TRANSPARENT);
				pDC->SetTextColor(ENZO_COLOR_BLACK);
				pDC->SetBkColor(ENZO_COLOR_DARKGRAY);
				return m_hBrushPrevBackGround;
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
			DisplayPreview();
			return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
		}
	}
	return hbr;
}

BOOL CIMGGeneratePromotionSheetsDlg::LoadBMPTemplates()
{
	for (int i = 0; i < (int)m_vCBitmaps.size(); i++)
	{
		m_vCBitmaps[i]->DeleteObject();
		delete m_vCBitmaps[i];
		m_vCBitmaps[i] = NULL;
	}
	m_vCBitmaps.clear();

	// TODO: Add your control notification handler code here
	CString szPath(GetCurrentDirectory());
	CString szFilename;

	switch (m_enumPromotionType)
	{
		case MD:
		{
			szPath += _T("\\Templates\\MD\\MD%d.bmp");
			break;
		}
		case SMD: 
		{
			szPath += _T("\\Templates\\SMD\\SMD%d.bmp");
			break;
		}
		case EMD:
		{
			szPath += _T("\\Templates\\EMD\\EMD%d.bmp");
			break;
		}
		case GJ:
		{
			szPath += _T("\\Templates\\GJ\\GJ%d.bmp");
			break;
		}
		case CEO:
		{
			szPath += _T("\\Templates\\CEO\\CEO%d.bmp");
			break;
		}
		case EVC:
		{
			szPath += _T("\\Templates\\EVC\\EVC%d.bmp");
			break;
		}
		case SEVC:
		{
			szPath += _T("\\Templates\\SEVC\\SEVC%d.bmp");
			break;
		}
		case OFFICE:
		{
			szPath += _T("\\Templates\\OFFICE\\OFFICE%d.bmp");
			break;
		}
		case HMO:
		{
			szPath += _T("\\Templates\\HEALTHCARE\\HEALTHCARE%d.bmp");
			break;
		}
		case LIMITEDPIN:
		{
			szPath += _T("\\Templates\\LIMITEDPINS\\PIN%d.bmp");
			break;
		}
	}

	for (int i = 0; i < m_tagDocumentMatrix.nPageCount; i++)
	{
		szFilename.Format(szPath, i + 1);
		HBITMAP hBmp = (HBITMAP)::LoadImage((HINSTANCE)NULL, szFilename,
			IMAGE_BITMAP, 0, 0,
			LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE);
		if (hBmp == NULL)
			return FALSE;
		CBitmap *bmp = new CBitmap();
		bmp->Attach(hBmp);
		m_vCBitmaps.push_back(bmp);
	}

	switch (m_enumPromotionType)
	{
		case MD:
		{
			if (m_vIMGData.size() > 5)
				m_nPagesToPrint = m_vCBitmaps.size();
			else
				m_nPagesToPrint = m_vCBitmaps.size() - 1;
			break;
		}
		case SMD:
		{
			if (m_vIMGData.size() > 30)
				m_nPagesToPrint = m_vCBitmaps.size();
			else
				m_nPagesToPrint = m_vCBitmaps.size() - 1;
			break;
		}
		case EMD:
		{
			if (m_vIMGData.size() > 90)
				m_nPagesToPrint = m_vCBitmaps.size();
			else
				m_nPagesToPrint = m_vCBitmaps.size() - 1;
			break;
		}
		case GJ:
		{
			if (m_vIMGData.size() > 100)
				m_nPagesToPrint = m_vCBitmaps.size();
			else
				m_nPagesToPrint = m_vCBitmaps.size() - 1;
			break;
		}
		case CEO:
		{
			if (m_vIMGData.size() > 150)
				m_nPagesToPrint = m_vCBitmaps.size();
			else
				m_nPagesToPrint = m_vCBitmaps.size() - 1;
			break;
		}
		case HMO:
		{
			if (m_vIMGData.size() > 60)
				m_nPagesToPrint = m_vCBitmaps.size();
			else
				m_nPagesToPrint = m_vCBitmaps.size() - 1;
			break;
		}
		case LIMITEDPIN:
		{
			if (m_vIMGData.size() > 9)
				m_nPagesToPrint = m_vCBitmaps.size();
			else
				m_nPagesToPrint = m_vCBitmaps.size() - 1;
			break;
		}
		case OFFICE:
		{
			if (m_vIMGData.size() > 20)
				m_nPagesToPrint = m_vCBitmaps.size();
			else
				m_nPagesToPrint = m_vCBitmaps.size() - 1;
			break;
		}
	}

	return TRUE;
}

BOOL CIMGGeneratePromotionSheetsDlg::InitializePrintMatrix()
{
	CPrintMatrix objPrnMatrix;

	if (!objPrnMatrix.LoadINIFile(m_enumPromotionType))
	{
		return FALSE;
	}

	m_tagDocumentMatrix = objPrnMatrix.GetDocumentMatrix();
	return TRUE;
}

void CIMGGeneratePromotionSheetsDlg::WriteDataToImage()
{
	if (m_vCBitmaps.empty())
		return;

	CClientDC dc(&m_ctrlPreview);
	CDC bmDC;
	CBitmap* pOldbmp;

	CFont font, headerFont;

	font.CreateFont(
		32,                       // nHeight
		0,                        // nWidth
		0,                        // nEscapement
		0,                        // nOrientation
		FW_NORMAL,                // nWeight
		FALSE,                    // bItalic
		FALSE,                    // bUnderline
		0,                        // cStrikeOut
		ANSI_CHARSET,             // nCharSet
		OUT_DEFAULT_PRECIS,       // nOutPrecision
		CLIP_DEFAULT_PRECIS,      // nClipPrecision
		DEFAULT_QUALITY,          // nQuality
		DEFAULT_PITCH | FF_SWISS, // nPitchAndFamily
		_T("ARIAL"));

	headerFont.CreateFont(
		60,                       // nHeight
		0,                        // nWidth
		0,                        // nEscapement
		0,                        // nOrientation
		FW_BOLD,                // nWeight
		FALSE,                    // bItalic
		FALSE,                    // bUnderline
		0,                        // cStrikeOut
		ANSI_CHARSET,             // nCharSet
		OUT_DEFAULT_PRECIS,       // nOutPrecision
		CLIP_DEFAULT_PRECIS,      // nClipPrecision
		DEFAULT_QUALITY,          // nQuality
		DEFAULT_PITCH | FF_SWISS, // nPitchAndFamily
		_T("ARIAL"));

	// select your font
	bmDC.CreateCompatibleDC(&dc);

	CFont* pOldFont;
	INT nStartPosX = 0, nStartPosY = 0, nItems = 0, nYDistance = 0;
	INT nVectorDataIndex = 0;

	//Print Promotee Name, Code, Date Completed at the first page
	pOldbmp = bmDC.SelectObject(m_vCBitmaps[0]);
	pOldFont = bmDC.SelectObject(&headerFont);
	bmDC.SetBkMode(TRANSPARENT);
	bmDC.TextOut(m_tagDocumentMatrix.nNamePos.x, m_tagDocumentMatrix.nNamePos.y, m_csNamePromotee);
	bmDC.TextOut(m_tagDocumentMatrix.nCodePos.x, m_tagDocumentMatrix.nCodePos.y, m_csCodeNumber);
	bmDC.TextOut(m_tagDocumentMatrix.nDatePos.x, m_tagDocumentMatrix.nDatePos.y, m_csDateCompletion);
	bmDC.SelectObject(pOldFont);
	bmDC.SelectObject(pOldbmp);

	//Promotee Signature at the last page
	pOldbmp = bmDC.SelectObject(m_vCBitmaps[m_vCBitmaps.size() - 2]);
	pOldFont = bmDC.SelectObject(&headerFont);
	bmDC.SetBkMode(TRANSPARENT);
	bmDC.TextOut(m_tagDocumentMatrix.nSignaturePos.x, m_tagDocumentMatrix.nSignaturePos.y, m_csNamePromotee);
	bmDC.SelectObject(pOldFont);
	bmDC.SelectObject(pOldbmp);

	if (m_enumPromotionType == SMD || (m_enumPromotionType == OFFICE))
	{
		pOldbmp = bmDC.SelectObject(m_vCBitmaps[m_vCBitmaps.size() - 2]);
		pOldFont = bmDC.SelectObject(&headerFont);
		bmDC.SetBkMode(TRANSPARENT);
		bmDC.TextOut(m_tagDocumentMatrix.nUplineNamePos.x, m_tagDocumentMatrix.nUplineNamePos.y, m_csUplineName);
		bmDC.SelectObject(pOldFont);
		bmDC.SelectObject(pOldbmp);
	}
	if ((m_enumPromotionType == LIMITEDPIN) || (m_enumPromotionType == HMO) || (m_enumPromotionType == OFFICE))
	{
		pOldbmp = bmDC.SelectObject(m_vCBitmaps[m_vCBitmaps.size() - 2]);
		pOldFont = bmDC.SelectObject(&headerFont);
		bmDC.SetBkMode(TRANSPARENT);
		bmDC.TextOut(m_tagDocumentMatrix.nRankPos.x, m_tagDocumentMatrix.nRankPos.y, m_csCurrentRank);
		bmDC.SelectObject(pOldFont);
		bmDC.SelectObject(pOldbmp);
	}
	for (int nPage = 0; nPage < (int)m_vCBitmaps.size(); nPage++)
	{
		pOldbmp = bmDC.SelectObject(m_vCBitmaps[nPage]);
		pOldFont = bmDC.SelectObject(&font);
		
		for (int nRow = 0; ((nRow < (int)m_tagDocumentMatrix.vPrintPageMatrix[nPage].nItemsPerPage) && (nVectorDataIndex < (int)m_vIMGData.size())); nRow++)
		{
			bmDC.SetBkMode(TRANSPARENT);
			bmDC.TextOut(m_tagDocumentMatrix.vPrintPageMatrix[nPage].nStartPos.x + m_tagDocumentMatrix.vPrintPageMatrix[nPage].nColSpacing[0], m_tagDocumentMatrix.vPrintPageMatrix[nPage].nStartPos.y + (m_tagDocumentMatrix.vPrintPageMatrix[nPage].nRowSpacing * nRow), m_vIMGData[nVectorDataIndex][0]); //REC
			if((m_enumPromotionType == HMO) || (m_enumPromotionType == LIMITEDPIN))
				bmDC.TextOut(m_tagDocumentMatrix.vPrintPageMatrix[nPage].nStartPos.x + m_tagDocumentMatrix.vPrintPageMatrix[nPage].nColSpacing[1], m_tagDocumentMatrix.vPrintPageMatrix[nPage].nStartPos.y + (m_tagDocumentMatrix.vPrintPageMatrix[nPage].nRowSpacing * nRow), m_vIMGData[nVectorDataIndex][1]); //Rec dte
			bmDC.TextOut(m_tagDocumentMatrix.vPrintPageMatrix[nPage].nStartPos.x + m_tagDocumentMatrix.vPrintPageMatrix[nPage].nColSpacing[2], m_tagDocumentMatrix.vPrintPageMatrix[nPage].nStartPos.y + (m_tagDocumentMatrix.vPrintPageMatrix[nPage].nRowSpacing * nRow), m_vIMGData[nVectorDataIndex][2]);
			bmDC.TextOut(m_tagDocumentMatrix.vPrintPageMatrix[nPage].nStartPos.x + m_tagDocumentMatrix.vPrintPageMatrix[nPage].nColSpacing[3], m_tagDocumentMatrix.vPrintPageMatrix[nPage].nStartPos.y + (m_tagDocumentMatrix.vPrintPageMatrix[nPage].nRowSpacing * nRow), m_vIMGData[nVectorDataIndex][3]);
			bmDC.TextOut(m_tagDocumentMatrix.vPrintPageMatrix[nPage].nStartPos.x + m_tagDocumentMatrix.vPrintPageMatrix[nPage].nColSpacing[4], m_tagDocumentMatrix.vPrintPageMatrix[nPage].nStartPos.y + (m_tagDocumentMatrix.vPrintPageMatrix[nPage].nRowSpacing * nRow), m_vIMGData[nVectorDataIndex][4]);
			bmDC.TextOut(m_tagDocumentMatrix.vPrintPageMatrix[nPage].nStartPos.x + m_tagDocumentMatrix.vPrintPageMatrix[nPage].nColSpacing[5], m_tagDocumentMatrix.vPrintPageMatrix[nPage].nStartPos.y + (m_tagDocumentMatrix.vPrintPageMatrix[nPage].nRowSpacing * nRow), m_vIMGData[nVectorDataIndex][5]);
			nVectorDataIndex++;
		}
		bmDC.SelectObject(pOldFont);
		bmDC.SelectObject(pOldbmp);
	}
	headerFont.DeleteObject();
	font.DeleteObject();
	bmDC.DeleteDC();
}

void CIMGGeneratePromotionSheetsDlg::OnBnClickedButtonPrint()
{
	// TODO: Add your control notification handler code here
	CClientDC dc(&m_ctrlPreview);
	CDC bmDC;

	bmDC.CreateCompatibleDC(&dc);
	CBitmap* pOldbmp;
	BITMAP  bi;

	CPrintDialogCustomized dlg(FALSE,
		PD_ALLPAGES |
		PD_HIDEPRINTTOFILE |
		PD_NOPAGENUMS |
		PD_RETURNDC |
		PD_USEDEVMODECOPIES);
	dlg.GetDefaults();
	dlg.m_pd.Flags &= ~PD_RETURNDEFAULT;

	DEVMODE* pDevMode = dlg.GetDevMode();
	pDevMode->dmFields |= DM_PAPERSIZE;
	pDevMode->dmPaperSize = DMPAPER_A4;
	::GlobalUnlock(dlg.m_pd.hDevMode);

	if (dlg.DoModal() == IDOK)
	{
		CDC printerDc;
		CString csPDFOutput;
		CString csPrinterName = dlg.GetDeviceName();
		printerDc.Attach(dlg.m_pd.hDC);

		// Zero and then initialize the members of a DOCINFO structure.
		DOCINFO di = { 0 };
		di.cbSize = sizeof(DOCINFO);
		di.lpszDocName = _T("IMG Promotion Sheets");
		di.lpszOutput = (LPTSTR)NULL;
		di.lpszDatatype = (LPTSTR)NULL;
		di.fwType = 0;

		// Begin a print job by calling the StartDoc function.
		printerDc.StartDoc(&di);

		// Inform the driver that the application is about to begin sending data.
		
		for(int i = 0; i < m_nPagesToPrint; i++)
		{
			pOldbmp = bmDC.SelectObject(m_vCBitmaps[i]);
			m_vCBitmaps[i]->GetBitmap(&bi);

			printerDc.StartPage();

			int imageWidth = bi.bmWidth;
			int imageHeight = bi.bmHeight;

			float fimageWidth = (float)imageWidth;
			float fimageHeight = (float)imageHeight;

			// Compute the coordinates of the upper left corner of the centered bitmap.
			float fprntPaperWidth = (float)GetDeviceCaps(printerDc.GetSafeHdc(), HORZRES);
			float fprntPaperHeight = (float)GetDeviceCaps(printerDc.GetSafeHdc(), VERTRES);
			float fbitmapAspectRatio = fimageWidth / fimageHeight;

			fimageWidth = fprntPaperWidth;
			fimageHeight = fimageWidth / fbitmapAspectRatio;

			int newX = (int)((fimageWidth));
			int newY = (int)((fimageHeight));

			int nPosX = ((int)((fprntPaperWidth - fimageWidth)))/2;
			int nPosY = ((int)((fprntPaperHeight - fimageHeight)))/2;

			printerDc.StretchBlt(nPosX, nPosY, newX, newY,	&bmDC, 0, 0, imageWidth, imageHeight, SRCCOPY);

			printerDc.EndPage();
			bmDC.SelectObject(pOldbmp);
		}
		// Inform the driver that document has ended.
		CFileDirectory cFile;
		csPDFOutput=cFile.GetPDFFilePath();
			
		printerDc.EndDoc();
		bmDC.DeleteDC();
	
		if (csPrinterName.Find(_T("PDF")) != -1)
		{
			Sleep(1000);
			ShellExecute(NULL, _T("open"), csPDFOutput, NULL, NULL, SW_SHOWNORMAL);
		}
	}
}

void CIMGGeneratePromotionSheetsDlg::OnBnClickedButtonNext()
{
	if (m_nPage < (m_nPagesToPrint - 1))
	{
		m_btnNext.SetFocus();
		m_nPage++;
		if (m_nPage == (m_nPagesToPrint - 1))
		{
			SendMessage(DM_SETDEFID, IDC_BUTTON_PREV, 0);
			m_btnNext.EnableWindow(FALSE);
		}
		else
		{
			SendMessage(DM_SETDEFID, IDC_BUTTON_NEXT, 0);
			m_btnPrevious.EnableWindow(TRUE);
		}
		DisplayPreview();
	}
}

void CIMGGeneratePromotionSheetsDlg::OnBnClickedButtonPrev()
{
	if (m_nPage > 0)
	{
		m_btnPrevious.SetFocus();
		m_nPage--;
		if (m_nPage == 0)
		{
			SendMessage(DM_SETDEFID, IDC_BUTTON_NEXT, 0);
			m_btnPrevious.EnableWindow(FALSE);
		}
		else
		{
			SendMessage(DM_SETDEFID, IDC_BUTTON_PREV, 0);
			m_btnNext.EnableWindow(TRUE);
		}
		DisplayPreview();
	}
}

BOOL CIMGGeneratePromotionSheetsDlg::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: Add your message handler code here and/or call default
	if (zDelta > 0)
		OnBnClickedButtonPrev();
	else if( zDelta < 0)
		OnBnClickedButtonNext();

	return CDialogEx::OnMouseWheel(nFlags, zDelta, pt);
}

BOOL CIMGGeneratePromotionSheetsDlg::LoadIMGData()
{
	LRESULT lRes = 0;
	CString csPath(GetCurrentDirectory());
	CString csRawData;

	 csPath += _T("\\Data.txt");

	FILE* fStream;
	errno_t errCode = _tfopen_s(&fStream, csPath, _T("r, ccs=UTF-8"));
	if (0 != errCode)
	{
		CString csErr;
		csErr.Format(_T("Path :%s\r\nError Data File Creation."), csPath);
		MessageBox(csErr, _T("Error : CIMGGeneratePromotionSheetsDlg::LoadIMGData()"), MB_ICONERROR);
		return FALSE;
	}

	CStdioFile File(fStream);
	CString strLine;
	csRawData = _T("");

	//Header
	if (!File.ReadString(strLine))
	{
		CString csErr;
		csErr.Format(_T("Path :%s\r\nError Data File Creation."), csPath);
		MessageBox(csErr, _T("Error : CIMGGeneratePromotionSheetsDlg::LoadIMGData()"), MB_ICONERROR);
		return FALSE;
	}

	INT nTabIndex = 0;
	CString csItem;
	vector<CString> vLine;

	while ((nTabIndex = strLine.Find(_T("\t"))) != -1)
	{
		csItem = strLine.Left(nTabIndex);
		strLine = strLine.Mid(nTabIndex + 1);
		vLine.push_back(csItem);
	}
	if (!strLine.IsEmpty())
		vLine.push_back(strLine);


	m_csNamePromotee = vLine[INDEX_NAME];
	m_csCodeNumber = vLine[INDEX_CODE];
	m_csUplineName = vLine[INDEX_UPLINE];
	m_enumPromotionType = (ENUMPROMOTIONTYPE)_ttoi(vLine[INDEX_RANK]);
	m_csDateCompletion = vLine[INDEX_DATE];
	m_csCurrentRank = vLine[INDEX_CURRENTRANK];

	//Body
	while (File.ReadString(strLine))
	{
		strLine += _T("\r\n");
		csRawData += strLine;
	}
	File.Close();

	ParseRawData(csRawData);

	return TRUE;
}

INT CIMGGeneratePromotionSheetsDlg::ParseRawData(CString csRawData)
{
	CString csRawDataLine, csItem;
	INT nNextLineIndex = 0, nTabIndex = 0;
	vector<CString> vLine;

	csRawDataLine.Empty();
	csItem.Empty();
	vLine.clear();
	m_vIMGData.clear();
	csRawData.MakeUpper();

	while ((nNextLineIndex = csRawData.Find(_T("\r\n"))) != -1)
	{
		csRawDataLine = csRawData.Left(nNextLineIndex);
		csRawData = csRawData.Mid(nNextLineIndex + 2);
		while ((nTabIndex = csRawDataLine.Find(_T("\t"))) != -1)
		{
			csItem = csRawDataLine.Left(nTabIndex);
			csRawDataLine = csRawDataLine.Mid(nTabIndex + 1);
			vLine.push_back(csItem);
		}

		if (!csRawDataLine.IsEmpty())
			vLine.push_back(csRawDataLine);

		m_vIMGData.push_back(vLine);
		vLine.clear();
	}
	if (!csRawData.IsEmpty())
	{
		csRawDataLine = csRawData;
		while ((nTabIndex = csRawDataLine.Find(_T("\t"))) != -1)
		{
			csItem = csRawDataLine.Left(nTabIndex);
			csRawDataLine = csRawDataLine.Mid(nTabIndex + 1);
			vLine.push_back(csItem);
		}

		if (!csRawDataLine.IsEmpty())
			vLine.push_back(csRawDataLine);

		m_vIMGData.push_back(vLine);
		vLine.clear();
	}
	return m_vIMGData.size();
}

CString CIMGGeneratePromotionSheetsDlg::GetCurrentDirectory()
{
	DWORD dwSize = 0;
	TCHAR lpStr[MAX_PATH];
	CString csString;
	
	ZeroMemory(lpStr, sizeof(lpStr));

	dwSize = GetModuleFileName(NULL,lpStr, MAX_PATH);
	PathRemoveFileSpec(lpStr);

	csString.Format(_T("%s"), lpStr);

	return csString;
}

void CIMGGeneratePromotionSheetsDlg::OnBnClickedButtonRefresh()
{
	m_nPage = 0;
	m_ctrlPreview.RedrawWindow();
	m_btnPrevious.EnableWindow(FALSE);
	m_btnNext.EnableWindow(TRUE);
	if (!LoadIMGData())//Load IMG Promotee data for submission
	{
		TCHAR buf[265];
		ZeroMemory(buf, sizeof(buf));
		FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL, GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			buf, (sizeof(buf) / sizeof(wchar_t)), NULL);
		MessageBox(buf, _T("Error : LoadIMGData()"), MB_ICONERROR);
		OnCancel();
		return;
	}
	if (!InitializePrintMatrix())
	{
		TCHAR buf[265];
		ZeroMemory(buf, sizeof(buf));
		FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL, GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			buf, (sizeof(buf) / sizeof(wchar_t)), NULL);
		MessageBox(buf, _T("Error : InitializePrintMatrix()"), MB_ICONERROR);
		OnCancel();
		return;
	}
	if (!LoadBMPTemplates()) // Load form templates
	{
		TCHAR buf[265];
		ZeroMemory(buf, sizeof(buf));
		FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL, GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			buf, (sizeof(buf) / sizeof(wchar_t)), NULL);
		MessageBox(buf, _T("Error : LoadBMPTemplates()"), MB_ICONERROR);
		OnCancel();
		return;
	}
	WriteDataToImage();
}
