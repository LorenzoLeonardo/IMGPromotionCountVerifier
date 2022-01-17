
#if !defined(AFX_DROPEDIT_H__1D8BBDC1_784C_11D1_8159_444553540000__INCLUDED_)
#define AFX_DROPEDIT_H__1D8BBDC1_784C_11D1_8159_444553540000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

// DropEdit.h : header file
//

#include <shlobj.h>
#include "stdafx.h"
// CDropEdit window

#define WM_SEND_PATH (WM_USER + 1)

class CDropEdit : public CEdit
{
// Construction
public:
	CDropEdit();

// Attributes
public:

// Operations
public:
	inline void SetUseDir(bool u)			{m_bUseDir=u;}
	inline bool IsUseDir() const			{return m_bUseDir;}

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDropEdit)
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CDropEdit();
	void SetCallBackWindow(HWND hWnd, INT nID)
	{ 
		m_hWndParent = hWnd;
		m_nID = nID;
	}
protected:
	CString ExpandShortcut(CString &inFile);

	// Generated message map functions
protected:
	//{{AFX_MSG(CDropEdit)
	afx_msg void OnDropFiles(HDROP dropInfo);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

	bool		m_bUseDir;
	HWND		m_hWndParent;
	INT			m_nID;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DROPEDIT_H__1D8BBDC1_784C_11D1_8159_444553540000__INCLUDED_)
