#include "pch.h"
#include "CPrintDialogCustomized.h"

CPrintDialogCustomized::CPrintDialogCustomized(BOOL bPrintSetupOnly,
	DWORD dwFlags,
	CWnd* pParentWnd) : CPrintDialog(bPrintSetupOnly, dwFlags, pParentWnd)
{
	m_hBrushPrevBackGround = CreateSolidBrush(ENZO_COLOR_DARKGRAY);
	m_hBrushBackGround = CreateSolidBrush(ENZO_COLOR_DIRTYWHITE);
}
CPrintDialogCustomized::~CPrintDialogCustomized()
{
	DeleteObject(m_hBrushPrevBackGround);
	DeleteObject(m_hBrushBackGround);
}

BEGIN_MESSAGE_MAP(CPrintDialogCustomized, CPrintDialog)
	ON_WM_CTLCOLOR()
	ON_WM_CLOSE()

	ON_UPDATE_COMMAND_UI(IDOK, &CPrintDialogCustomized::OnUpdateIdok)
	ON_COMMAND(ID_FILE_SAVE, &CPrintDialogCustomized::OnFileSave)
	ON_COMMAND(ID_FILE_SAVE_AS, &CPrintDialogCustomized::OnFileSaveAs)
	ON_COMMAND(ID_FILE_PRINT, &CPrintDialogCustomized::OnFilePrint)
END_MESSAGE_MAP()


HBRUSH CPrintDialogCustomized::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CPrintDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}

BOOL CPrintDialogCustomized::OnInitDialog()
{
	CPrintDialog::OnInitDialog();


	m_pd.lpPrintTemplateName;

	// TODO:  Add extra initialization here
	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}


void CPrintDialogCustomized::OnClose()
{
	// TODO: Add your message handler code here and/or call default
	

	CPrintDialog::OnClose();
}


void CPrintDialogCustomized::DoDataExchange(CDataExchange* pDX)
{
	// TODO: Add your specialized code here and/or call the base class

	CPrintDialog::DoDataExchange(pDX);
}


BOOL CPrintDialogCustomized::OnCommand(WPARAM wParam, LPARAM lParam)
{
	// TODO: Add your specialized code here and/or call the base class
	m_pd.lpPrintTemplateName;
	return CPrintDialog::OnCommand(wParam, lParam);
}





void CPrintDialogCustomized::OnUpdateIdok(CCmdUI* pCmdUI)
{
	// TODO: Add your command update UI handler code here
	m_pd.lpPrintTemplateName;
}


void CPrintDialogCustomized::OnFileSave()
{
	// TODO: Add your command handler code here
	m_pd.lpPrintTemplateName;

}


void CPrintDialogCustomized::OnFileSaveAs()
{
	// TODO: Add your command handler code here
	m_pd.lpPrintTemplateName;

}


void CPrintDialogCustomized::OnFilePrint()
{
	// TODO: Add your command handler code here
	m_pd.lpPrintTemplateName;

}
