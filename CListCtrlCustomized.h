/*******************************************************************************
	Author						: Aravindan Premkumar
	Unregistered Copyright 2003	: Aravindan Premkumar
	All Rights Reserved

	This piece of code does not have any registered copyright and is free to be
	used as necessary. The user is free to modify as per the requirements. As a
	fellow developer, all that I expect and request for is to be given the
	credit for intially developing this reusable code by not removing my name as
	the author.
*******************************************************************************/

#if !defined(AFX_COMBOLISTCTRL_H__9089600F_374F_4BFC_9482_DEAC0E7133E8__INCLUDED_)
#define AFX_COMBOLISTCTRL_H__9089600F_374F_4BFC_9482_DEAC0E7133E8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "CustomHeader.h"
class CListCtrlCustomized : public CListCtrl
{
public:

	CListCtrlCustomized();

	virtual ~CListCtrlCustomized();

	void SetSelectedRow(INT nRow)
	{
		m_nSelectedRow = nRow;
	}
protected:

	afx_msg void OnNMHover(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult);

	INT m_nSelectedRow;
	DECLARE_MESSAGE_MAP()

public:
	void EnableHighlighting(int row, bool bHighlight);
	bool  IsRowSelected(int row);
	bool  IsRowHighlighted(int row);


};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COMBOLISTCTRL_H__9089600F_374F_4BFC_9482_DEAC0E7133E8__INCLUDED_)
