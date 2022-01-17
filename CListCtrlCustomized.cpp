#include "pch.h"
#include "CListCtrlCustomized.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//#defines
#define FIRST_COLUMN				0
#define MIN_COLUMN_WIDTH			10
#define MAX_DROP_DOWN_ITEM_COUNT	10

/////////////////////////////////////////////////////////////////////////////
// CComboListCtrl

CListCtrlCustomized::CListCtrlCustomized()
{
	m_nSelectedRow = -1;
}

CListCtrlCustomized::~CListCtrlCustomized()
{
}

BEGIN_MESSAGE_MAP(CListCtrlCustomized, CListCtrl)

	ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, OnCustomDraw)
	//ON_NOTIFY_REFLECT(NM_HOVER, &CDialogReportRecruitsDlg::OnNMHover)

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CComboListCtrl message handlers




void CListCtrlCustomized::OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMLVCUSTOMDRAW  lplvcd = (LPNMLVCUSTOMDRAW)pNMHDR;
	size_t iRow = lplvcd->nmcd.dwItemSpec;
	bool bHighlighted = false;

	switch (lplvcd->nmcd.dwDrawStage)
	{
		case CDDS_PREPAINT:
		{
			*pResult = CDRF_NOTIFYITEMDRAW;
			break;
		}
		// Modify item text and or background
		case CDDS_ITEMPREPAINT:
		case CDDS_SUBITEM:
		{
			if (lplvcd->nmcd.uItemState & CDIS_SELECTED)
			{
				lplvcd->nmcd.uItemState = ((lplvcd->nmcd.uItemState) & (~CDIS_SELECTED));
				lplvcd->clrTextBk = RGB(0, 255, 255);
			}
			else
			{
				if (iRow % 2)
				{
					lplvcd->clrTextBk = ENZO_COLOR_WHITE;
				}
				else
				{
					lplvcd->clrTextBk = ENZO_COLOR_DIRTYWHITE;
				}

				CString csString;
				csString = this->GetItemText(iRow, (REC_HEADER_BAP + 1));
				if (csString.Compare(_T("YES")) == 0)
				{
					lplvcd->clrTextBk = ENZO_COLOR_YELLOW;
				}
			}

			*pResult = CDRF_SKIPPOSTPAINT;
			break;
		}
	}
}

void CListCtrlCustomized::OnNMHover(NMHDR* pNMHDR, LRESULT* pResult)
{
	// TODO: Add your control notification handler code here
	*pResult = 0;
}

void  CListCtrlCustomized::EnableHighlighting(int row, bool bHighlight)
{
	SetItemState(row, bHighlight ? 0xff : 0, LVIS_SELECTED);
}
bool  CListCtrlCustomized::IsRowSelected(int row)
{
	return GetItemState(row, LVIS_SELECTED) != 0;
}
bool  CListCtrlCustomized::IsRowHighlighted(int row)
{
	// We check if row is selected.
	// We also check if window has focus. This was because the original listview
	//  control I created did not have style LVS_SHOWSELALWAYS. So if the listview
	//  does not have focus, then there is no highlighting.
	return IsRowSelected(row) && (::GetFocus() == m_hWnd);
}
