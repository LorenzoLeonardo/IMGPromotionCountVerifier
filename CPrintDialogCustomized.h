#pragma once
#include "CustomHeader.h"
#include <afxdlgs.h>
class CPrintDialogCustomized :
	public CPrintDialog
{
	//DECLARE_DYNAMIC(CPrintDialogCustomized)
public:
	CPrintDialogCustomized(BOOL ,DWORD , CWnd* pParentWnd = NULL);
	~CPrintDialogCustomized();

public:
	DECLARE_MESSAGE_MAP()
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();
protected:
	HBRUSH m_hBrushPrevBackGround;
	HBRUSH m_hBrushBackGround;
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
public:

	afx_msg void OnUpdateIdok(CCmdUI* pCmdUI);
	afx_msg void OnFileSave();
	afx_msg void OnFileSaveAs();
	afx_msg void OnFilePrint();
};

