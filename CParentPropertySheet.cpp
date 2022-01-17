#include "pch.h"
#include "CParentPropertySheet.h"
#include "CustomHeader.h"
#include "CPromoteeInfoDlg.h"
#include "CFileDirectory.h"
#include "IMGGeneratePromotionSheetsDlg.h"

CParentPropertySheet::CParentPropertySheet()
	:CPropertySheet()
{
}
CParentPropertySheet::CParentPropertySheet(CString csCaption)
	: CPropertySheet(csCaption)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}
CParentPropertySheet::~CParentPropertySheet()
{
	DeleteObject(m_hBrushBackGround);
}

BEGIN_MESSAGE_MAP(CParentPropertySheet, CPropertySheet)
	ON_COMMAND(IDOK, &CParentPropertySheet::OnPrint)
	ON_COMMAND(IDHELP, &CParentPropertySheet::OnHelp)
	ON_COMMAND(ID_APPLY_NOW, &CParentPropertySheet::OnSave)
	ON_WM_CTLCOLOR()
	ON_WM_PAINT()
END_MESSAGE_MAP()

void CParentPropertySheet::OnSave()
{
	CDialogReportRecruitsDlg* recruitDlg = (CDialogReportRecruitsDlg*)GetPage(0);
	CDialogReportSalesDlg* salesDlg = (CDialogReportSalesDlg*)GetPage(1);

	if (!recruitDlg->IsVisited() || !salesDlg->IsVisited())
	{
		MessageBox(_T("We cannot proceed in saving the file because you have not yet checked all the reports. To proceed, please check the reports thoroughly."), _T("Warning : Cannot Proceed In Saving The File"), MB_ICONEXCLAMATION);
		return;
	}
	CFileDirectory cFile;
	CString csPath = cFile.SaveFile();
	vector<vector<CString>> vData;

	if (csPath.IsEmpty())
		return;

	FILE* fStream;
	errno_t errCode = _tfopen_s(&fStream, csPath, _T("wb, ccs=UTF-8"));
	if (0 != errCode)
	{
		return;
	}
	fclose(fStream);

	errCode = _tfopen_s(&fStream, csPath, _T("w, ccs=UTF-8"));
	if (0 != errCode)
	{
		return;
	}
	CStdioFile File(fStream);

	File.WriteString(_T("<RECRUITS>"));
	File.WriteString(_T("\n"));
	CString csLine = _T("");

	vData = recruitDlg->GetData();
	for (int nRow = 0; nRow < vData.size(); nRow++)
	{
		for (int nCol = 0; nCol < vData[nRow].size(); nCol++)
		{
			if (nCol < (vData[nRow].size() - 1))
				csLine += vData[nRow][nCol] + _T("\t");
			else
				csLine += recruitDlg->GetData()[nRow][nCol];
		}
		File.WriteString(csLine);
		File.WriteString(_T("\n"));
		csLine.Empty();
	}
	File.WriteString(_T("</RECRUITS>"));
	File.WriteString(_T("\n"));
	csLine.Empty();

	File.WriteString(_T("<SALES>"));
	File.WriteString(_T("\n"));

	vData = salesDlg->GetData();
	for (int nRow = 0; nRow < vData.size(); nRow++)
	{
		for (int nCol = 0; nCol < vData[nRow].size(); nCol++)
		{
			if (nCol < (vData[nRow].size() - 1))
				csLine += vData[nRow][nCol] + _T("\t");
			else
				csLine += vData[nRow][nCol];
		}
		File.WriteString(csLine);
		File.WriteString(_T("\n"));
		csLine.Empty();
	}
	File.WriteString(_T("</SALES>"));
	File.WriteString(_T("\n"));
	File.Close();

}

void CParentPropertySheet::GetAllWindowsFromProcessID(DWORD dwProcessID, std::vector <HWND>& vhWnds)
{
	// find all hWnds (vhWnds) associated with a process id (dwProcessID)
	HWND hCurWnd = nullptr;
	do
	{
		hCurWnd = ::FindWindowEx(nullptr, hCurWnd, nullptr, nullptr);
		DWORD checkProcessID = 0;
		GetWindowThreadProcessId(hCurWnd, &checkProcessID);
		if (checkProcessID == dwProcessID)
		{
			vhWnds.push_back(hCurWnd);  // add the found hCurWnd to the vector
			//wprintf(L"Found hWnd %d\n", hCurWnd);
		}
	} while (hCurWnd != nullptr);
}


void CParentPropertySheet::OnPrint()
{
	CDialogReportRecruitsDlg* recruitDlg = (CDialogReportRecruitsDlg*)GetPage(0);
	CDialogReportSalesDlg* salesDlg = (CDialogReportSalesDlg*)GetPage(1);
	vector<vector<CString>> vRec;
	vector<vector<CString>> vSales;

	if (!recruitDlg->IsVisited() || !salesDlg->IsVisited())
	{
		MessageBox(_T("We cannot proceed in printing because you have not yet checked all the reports. To proceed, please check the reports thoroughly."), _T("Warning : Cannot Proceed In Printing"), MB_ICONEXCLAMATION);
		return;
	}
	recruitDlg->GetDataToPrint(vRec);
	salesDlg->GetDataToPrint(vSales);
	CQuickSort qSort;

	if (qSort.CompareDate(vRec[vRec.size() - 1][2], vSales[vSales.size() - 1][2]) > 0)
	{
		m_vPromoteeInfo[INDEX_DATE] = vRec[vRec.size() - 1][2];
	}
	else
	{
		m_vPromoteeInfo[INDEX_DATE] = vSales[vSales.size() - 1][2];
	}
	

	CPromoteeInfoDlg promoteeDlg;

	promoteeDlg.SetInformation(m_vPromoteeInfo);

	if (promoteeDlg.DoModal() != IDOK)
		return;

	m_vPromoteeInfo = promoteeDlg.GetInformation();



	CString csHeader = _T("");
	CString csPath = GetCurrentDirectory();	

	csHeader.Format(_T("%s\t%s\t%s\t%s\t%s\t%s"), m_vPromoteeInfo[INDEX_NAME], m_vPromoteeInfo[INDEX_CODE],
		m_vPromoteeInfo[INDEX_UPLINE], m_vPromoteeInfo[INDEX_RANK],
		m_vPromoteeInfo[INDEX_DATE], m_vPromoteeInfo[INDEX_CURRENTRANK]);

	csPath += _T("\\Data.txt");
	FILE* fStream;
	errno_t errCode = _tfopen_s(&fStream, csPath, _T("wb, ccs=UTF-8"));
	if (0 != errCode)
	{
		CString csErr;
		csErr.Format(_T("Path :%s\r\nError Data File Creation."), csPath);
		MessageBox(csErr, _T("Error : CParentPropertySheet::OnPrint()"), MB_ICONERROR);
		return;
	}

	fclose(fStream);

	errCode = _tfopen_s(&fStream, csPath, _T("w, ccs=UTF-8"));
	if (0 != errCode)
	{
		CString csErr;
		csErr.Format(_T("Path :%s\r\nError Data File Creation."), csPath);
		MessageBox(csErr, _T("Error : CParentPropertySheet::OnPrint()"), MB_ICONERROR);
		return;
	}

	CStdioFile File(fStream);

	File.WriteString(csHeader);
	File.WriteString(_T("\n"));
	int nRecCount = (int)vRec.size();
	int nSaleCount = (int)vSales.size();
	int nMaxCount = max(nRecCount, nSaleCount);

	CString csLine = _T("");
	for (int i = 0; i < nMaxCount; i++)
	{
		if (i < nRecCount)
		{
			csLine += vRec[i][0] + _T("\t");
			csLine += vRec[i][1] + _T("\t");
			csLine += vRec[i][2] + _T("\t");
		}
		else
		{
			csLine += _T("\t");
			csLine += _T("\t");
			csLine += _T("\t");
		}

		if (i < nSaleCount)
		{
			csLine += vSales[i][0] + _T("\t");
			csLine += vSales[i][1] + _T("\t");
			csLine += vSales[i][2];
		}
		else
		{
			csLine += _T("\t");
			csLine += _T("\t");
			csLine += _T("\t");
		}
		File.WriteString(csLine);
		File.WriteString(_T("\n"));
		csLine = _T("");
	}
	File.Close();

	this->ShowWindow(FALSE);
	LaunchPrintModule3();
	this->ShowWindow(TRUE);

	csPath = GetCurrentDirectory();
	csPath += _T("\\Data.txt");
	DeleteFile(csPath);
}

INT CParentPropertySheet::LaunchPrintModule1()
{
	CString csPath = GetCurrentDirectory();
	csPath += _T("\\IMGGeneratePromotionSheets.exe");

	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));

	this->EnableWindow(FALSE);
	// Start the child process. 
	if (!CreateProcess(NULL,   // No module name (use command line)
		(LPTSTR)csPath.GetString(),        // Command line
		NULL,           // Process handle not inheritable
		NULL,           // Thread handle not inheritable
		FALSE,          // Set handle inheritance to FALSE
		0,              // No creation flags
		NULL,           // Use parent's environment block
		NULL,           // Use parent's starting directory 
		&si,            // Pointer to STARTUPINFO structure
		&pi)           // Pointer to PROCESS_INFORMATION structure
		)
	{
		TCHAR buf[265];
		CString csErr;

		ZeroMemory(buf, sizeof(buf));
		FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL, GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			buf, (sizeof(buf) / sizeof(TCHAR)), NULL);

		csErr.Format(_T("%s\r\nPrinting Path: %s"), buf, csPath);
		MessageBox(csErr, _T("Printing Module Error"), MB_ICONERROR);
		return FALSE;
	}

	// Wait until child process exits.
	WaitForSingleObject(pi.hProcess, INFINITE);

	// Close process and thread handles. 
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);
	this->EnableWindow(TRUE);
	return TRUE;
}

INT CParentPropertySheet::LaunchPrintModule2()
{


	CString csPath = GetCurrentDirectory();
	CString csParentHWND;
	csPath += _T("\\IMGGeneratePromotionSheets.exe");
	BOOL bInit = SUCCEEDED(CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE));
	SHELLEXECUTEINFO ShExecInfo;

	ZeroMemory(&ShExecInfo, sizeof(SHELLEXECUTEINFO));
		
	csParentHWND.Format(_T("%d"), this->GetSafeHwnd());

	ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
	ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
	ShExecInfo.hwnd = this->GetSafeHwnd();
	ShExecInfo.lpFile = csPath.GetString();
	ShExecInfo.lpParameters = csParentHWND.GetString();
	ShExecInfo.lpDirectory = NULL;
	ShExecInfo.nShow = SW_SHOW;
	ShExecInfo.hInstApp = NULL;
	

	if (!ShellExecuteEx(&ShExecInfo))
	{
		if (bInit)
			CoUninitialize();

		TCHAR buf[265];
		CString csErr;

		ZeroMemory(buf, sizeof(buf));
		FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL, GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			buf, (sizeof(buf) / sizeof(TCHAR)), NULL);

		csErr.Format(_T("%s\r\nPrinting Path: %s"), buf, csPath);
		MessageBox(csErr, _T("Printing Module Error"), MB_ICONERROR);

		return FALSE;
	}

	WaitForSingleObject(ShExecInfo.hProcess, INFINITE);

	if (bInit)
		CoUninitialize();

	return TRUE;
}

INT CParentPropertySheet::LaunchPrintModule3()
{
	CIMGGeneratePromotionSheetsDlg dlg;

	INT_PTR nRet = dlg.DoModal();

	if (nRet == IDOK)
		return TRUE;
	else
		return FALSE;
}

void CParentPropertySheet::OnHelp()
{
	ShellExecute(NULL, _T("open"), _T("https://m.me/Lorenzo.Leonardo.92"), NULL, NULL, SW_SHOWNORMAL);
}
CString CParentPropertySheet::GetCurrentDirectory()
{
	DWORD dwSize = 0;
	TCHAR lpStr[MAX_PATH];
	CString csString;

	ZeroMemory(lpStr, sizeof(lpStr));

	dwSize = GetModuleFileName(NULL, lpStr, MAX_PATH);
	PathRemoveFileSpec(lpStr);

	csString.Format(_T("%s"), lpStr);

	return csString;
}
BOOL CParentPropertySheet::OnInitDialog()
{
	RECT rect = { 0 };

	BOOL bResult = CPropertySheet::OnInitDialog();

	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

    int ids[] = { IDOK, IDCANCEL };//, ID_APPLY_NOW, IDHELP };

    // Hide Apply and Help buttons
	this->SetWindowText(_T("Members And Productions Tally Sheets"));
    CWnd* pWnd = GetDlgItem(ID_APPLY_NOW);
    pWnd->SetWindowText(_T("Save"));
	pWnd->EnableWindow();

	CString csData;
    CRect rectBtn;
    int nSpacing = 6;        // space between two buttons...
    m_hBrushBackGround = CreateSolidBrush(ENZO_COLOR_NAVYBLUE);
    pWnd = GetDlgItem(IDOK);
    pWnd->SetWindowText(_T("Print"));
    SetBkColor(this->GetDC()->m_hDC, ENZO_COLOR_NAVYBLUE);

	csData = _T("<Input Promotee's Name>");
	m_vPromoteeInfo.push_back(csData);
	csData = _T("<Input Code Number>");
	m_vPromoteeInfo.push_back(csData);
	csData = _T("<Input Upline SMD/CEO Name>");
	m_vPromoteeInfo.push_back(csData);
	csData.Format(_T("%d"), SMD);
	m_vPromoteeInfo.push_back(csData);

	SYSTEMTIME completedDate;
	GetLocalTime(&completedDate);
	csData.Format(_T("%02d/%02d/%04d"), completedDate.wMonth, completedDate.wDay, completedDate.wYear);
	m_vPromoteeInfo.push_back(csData);

	csData = _T("TA");
	m_vPromoteeInfo.push_back(csData);

	//WS_EX_APPWINDOW
	//cs.dwExStyle |= WS_EX_TOOLWINDOW | WS_EX_APPWINDOW;
	

	SetWindowLong(this->GetSafeHwnd(), GWL_EXSTYLE, WS_EX_APPWINDOW);


    return bResult;
}

HBRUSH CParentPropertySheet::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CPropertySheet::OnCtlColor(pDC, pWnd, nCtlColor);

	switch (nCtlColor)
	{
	    case CTLCOLOR_DLG:
	    {
		    pDC->SetTextColor(ENZO_COLOR_WHITE);
		    pDC->SetBkColor(ENZO_COLOR_NAVYBLUE);
		    return m_hBrushBackGround;
	    }

	default:
		return CPropertySheet::OnCtlColor(pDC, pWnd, nCtlColor);
	}
	return hbr;
}

void CParentPropertySheet::OnPaint()
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
		CPropertySheet::OnPaint();
	}
}


BOOL CParentPropertySheet::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Add your specialized code here and/or call the base class


	if (!CPropertySheet::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle |= WS_EX_TOOLWINDOW | WS_EX_APPWINDOW;
	cs.style |= WS_SYSMENU;

	return TRUE;
}
