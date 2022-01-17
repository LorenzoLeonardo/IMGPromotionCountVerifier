
// IMGPromotionCountVerifier.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CIMGPromotionCountVerifierApp:
// See IMGPromotionCountVerifier.cpp for the implementation of this class
//

class CIMGPromotionCountVerifierApp : public CWinApp
{
public:
	CIMGPromotionCountVerifierApp();

// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
// Implementation
	CString GetVersionString();
	CString GetCurrentDirectory();
	DECLARE_MESSAGE_MAP()
};

extern CIMGPromotionCountVerifierApp theApp;
