
// IMGPromotionCountVerifier.cpp : Defines the class behaviors for the application.
//

#include "pch.h"
#include "framework.h"
#include "IMGPromotionCountVerifier.h"
#include "IMGPromotionCountVerifierDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CIMGPromotionCountVerifierApp

BEGIN_MESSAGE_MAP(CIMGPromotionCountVerifierApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CIMGPromotionCountVerifierApp construction

CIMGPromotionCountVerifierApp::CIMGPromotionCountVerifierApp()
{
	// support Restart Manager
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CIMGPromotionCountVerifierApp object

CIMGPromotionCountVerifierApp theApp;


// CIMGPromotionCountVerifierApp initialization

BOOL CIMGPromotionCountVerifierApp::InitInstance()
{
	// for CDropEdit!!
	//::CoInitialize(NULL);
	HANDLE mutex = CreateMutex(NULL, FALSE, _T("IMGPROMOTIONCOUNTVERIFIER"));
	if (mutex == NULL)
	{
		return FALSE;
	}

	if (GetLastError() == ERROR_ALREADY_EXISTS)
	{
		/* You only need the message box if you wish to notify the user
		   that the process is running*/
		MessageBox(NULL, _T("Another instance is already running."), _T("A Program Already Running"), MB_ICONWARNING);
		return FALSE;
	}

	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}
	AfxEnableControlContainer();

	// Create the shell manager, in case the dialog contains
	// any shell tree view or shell list view controls.
	CShellManager *pShellManager = new CShellManager;

	// Activate "Windows Native" visual manager for enabling themes in MFC controls
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("IMG Promotion Verifier Automation Tool"));

	CString csVersion = GetVersionString();
	CString csCMDLine;

	CIMGPromotionCountVerifierDlg dlg;

	csCMDLine.Format(_T("%s"), m_lpCmdLine);
	dlg.SetCMDLine(csCMDLine);
	dlg.SetVersion(csVersion);
	m_pMainWnd = &dlg;

	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}
	else if (nResponse == -1)
	{
		TRACE(traceAppMsg, 0, "Warning: dialog creation failed, so application is terminating unexpectedly.\n");
		TRACE(traceAppMsg, 0, "Warning: if you are using MFC controls on the dialog, you cannot #define _AFX_NO_MFC_CONTROLS_IN_DIALOGS.\n");
	}

	// Delete the shell manager created above.
	if (pShellManager != nullptr)
	{
		delete pShellManager;
	}

#if !defined(_AFXDLL) && !defined(_AFX_NO_MFC_CONTROLS_IN_DIALOGS)
	ControlBarCleanUp();
#endif

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}

CString CIMGPromotionCountVerifierApp::GetCurrentDirectory()
{
	DWORD dwSize = 0;
	LPTSTR lpStr = NULL;
	CString csString;

	lpStr = new TCHAR[MAX_PATH];
	dwSize = GetModuleFileName(NULL, lpStr, sizeof(TCHAR[MAX_PATH]));

	csString.Format(_T("%s"), lpStr);
	delete[] lpStr;
	lpStr = NULL;
	dwSize = 0;

	return csString;
}

CString CIMGPromotionCountVerifierApp::GetVersionString()
{
	DWORD dwHandle, dwLen;
	UINT BufLen;
	CString csPath;
	CString csDisplayVersion;
	LPTSTR lpData;
	VS_FIXEDFILEINFO* pFileInfo;

	csPath = GetCurrentDirectory(); /* try first the current directory */
	dwLen = GetFileVersionInfoSize(csPath, &dwHandle);

	if (!dwLen) 
	{
		return _T("");
	}
	lpData = new TCHAR[dwLen];
	if (!lpData) 
	{
		return _T("");
	}
	if (!GetFileVersionInfo(csPath, dwHandle, dwLen, lpData))
	{
		delete[] lpData;
		lpData = NULL;
		return _T("");
	}
	if (!VerQueryValue(lpData, _T("\\"), (LPVOID *)&pFileInfo, (PUINT)&BufLen)) 
	{
		delete[] lpData;
		lpData = NULL;
		return _T("");
	}
	else 
	{
		csDisplayVersion.Format(_T("Version %d.%d.%d.%d"), HIWORD(pFileInfo->dwFileVersionMS), LOWORD(pFileInfo->dwFileVersionMS), HIWORD(pFileInfo->dwFileVersionLS), LOWORD(pFileInfo->dwFileVersionLS));
	}
	delete[] lpData;
	lpData = NULL;

	return csDisplayVersion;
}

int CIMGPromotionCountVerifierApp::ExitInstance()
{
	// for CDropEdit!!
	//::CoUninitialize();

	return CWinApp::ExitInstance();
}