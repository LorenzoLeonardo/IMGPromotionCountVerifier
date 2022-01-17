// CDialogReportView.cpp : implementation file
//

#include "pch.h"
#include "IMGPromotionCountVerifier.h"
#include "CDialogReportRecruitsDlg.h"
#include "CMembersProfileDlg.h"
#include "afxdialogex.h"


// CDialogReportView dialog

IMPLEMENT_DYNAMIC(CDialogReportRecruitsDlg, CMFCPropertyPage)

CDialogReportRecruitsDlg::CDialogReportRecruitsDlg(CWnd* pParent /*=nullptr*/)
	: CMFCPropertyPage(IDD_DIALOG_REPORT_RECRUITS)
{
	m_szTabTitle = _T("Members Report Sheet");
	m_bIsVisited = FALSE;
	m_hBrushBackGround = CreateSolidBrush(ENZO_COLOR_NAVYBLUE);
	m_hBrushEditCtrl = CreateSolidBrush(ENZO_COLOR_DIRTYWHITE);
}

CDialogReportRecruitsDlg::~CDialogReportRecruitsDlg()
{
	DeleteObject(m_hBrushBackGround);
	DeleteObject(m_hBrushEditCtrl);
	//delete pm_obMenu;
}

void CDialogReportRecruitsDlg::DoDataExchange(CDataExchange* pDX)
{
	CMFCPropertyPage::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_LIST_RECRUIT, m_ctrlListRecruit);
	DDX_Control(pDX, IDC_EDIT_DISPLAY_COUNT, m_editCtrlDisplayCounter);
	DDX_Control(pDX, IDC_MONTHCALENDAR_START, m_ctrlStartCalendar);
	DDX_Control(pDX, IDC_MONTHCALENDAR_END, m_ctrlEndCalendar);
	DDX_Control(pDX, IDC_COMBO_RUN_DAYS, m_ctrlComboRunningDays);
	DDX_Control(pDX, IDC_STATIC_ITEMS, m_staticItems);
}


BEGIN_MESSAGE_MAP(CDialogReportRecruitsDlg, CMFCPropertyPage)
	ON_NOTIFY(NM_CLICK, IDC_LIST_RECRUIT, &CDialogReportRecruitsDlg::OnNMClickListRecruit)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_RECRUIT, OnRclickList)
	ON_COMMAND(ID_BAP, OnBAP)
	ON_WM_PAINT()
	ON_NOTIFY(LVN_HOTTRACK, IDC_LIST_RECRUIT, &CDialogReportRecruitsDlg::OnNMHoverListRecruit)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_RECRUIT, &CDialogReportRecruitsDlg::OnNMDblclkListRecruit)
	ON_NOTIFY(MCN_SELECT, IDC_MONTHCALENDAR_START, &CDialogReportRecruitsDlg::OnMcnSelectMonthcalendarStart)
	ON_NOTIFY(MCN_VIEWCHANGE, IDC_MONTHCALENDAR_START, &CDialogReportRecruitsDlg::OnMcnViewChangeMonthcalendarStart)
	ON_NOTIFY(MCN_SELCHANGE, IDC_MONTHCALENDAR_START, &CDialogReportRecruitsDlg::OnMcnSelchangeMonthcalendarStart)
	ON_WM_CTLCOLOR()
	ON_CBN_SELCHANGE(IDC_COMBO_RUN_DAYS, &CDialogReportRecruitsDlg::OnCbnSelchangeComboRunDays)
	ON_WM_CLOSE()
	ON_WM_ERASEBKGND()
	ON_BN_CLICKED(IDC_STATIC_TALLY_BOARD, &CDialogReportRecruitsDlg::OnBnClickedStaticTallyBoard)
END_MESSAGE_MAP()

BOOL CDialogReportRecruitsDlg::OnInitDialog()
{
	CMFCPropertyPage::OnInitDialog();

	LPCTSTR lpcRecHeader[] = { _T("No."), _T("BAP"), _T("Name"),_T("Agent Code"),_T("Date Started"),_T("Rank"),_T("Sponsor"),_T("SMD"),_T("Accredited Until") };
	int nCol = 0;

	m_bIsVisited = TRUE;


	//pm_obMenu = new CMenu;
	//pm_obMenu->LoadMenu(IDR_LIST_POPUP);
    //m_pPopupMenu = pm_obMenu->GetSubMenu(0);


	m_ctrlListRecruit.InsertColumn(nCol, lpcRecHeader[nCol++], LVCFMT_FIXED_WIDTH, 30);
	m_ctrlListRecruit.InsertColumn(nCol, lpcRecHeader[nCol++], LVCFMT_FIXED_WIDTH, 35);
	m_ctrlListRecruit.InsertColumn(nCol, lpcRecHeader[nCol++], LVCFMT_LEFT, 180);
	m_ctrlListRecruit.InsertColumn(nCol, lpcRecHeader[nCol++], LVCFMT_LEFT, 80);
	m_ctrlListRecruit.InsertColumn(nCol, lpcRecHeader[nCol++], LVCFMT_LEFT, 80);
	m_ctrlListRecruit.InsertColumn(nCol, lpcRecHeader[nCol++], LVCFMT_LEFT, 80);
	m_ctrlListRecruit.InsertColumn(nCol, lpcRecHeader[nCol++], LVCFMT_LEFT, 250);
	m_ctrlListRecruit.InsertColumn(nCol, lpcRecHeader[nCol++], LVCFMT_LEFT, 250);
	m_ctrlListRecruit.InsertColumn(nCol, lpcRecHeader[nCol++], LVCFMT_FIXED_WIDTH, 0);
	m_ctrlListRecruit.SetExtendedStyle(LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT|LVS_EX_TRACKSELECT|LVS_EDITLABELS);
	 
	m_ctrlComboRunningDays.AddString(_T("30"));
	m_ctrlComboRunningDays.AddString(_T("60"));
	m_ctrlComboRunningDays.AddString(_T("90"));
	m_ctrlComboRunningDays.AddString(_T("365"));
	m_ctrlComboRunningDays.SetCurSel(2);

	m_csRawData.TrimLeft();
	m_csRawData.TrimRight();

	m_vRecruitDataStorage = ParseRecruitData(m_csRawData);
	m_qSort.SetDataToSort(&m_vRecruitDataStorage, REC_HEADER_AGENTCODE);
	m_qSort.SortVector(0, (INT)(m_vRecruitDataStorage.size() - 1));
	m_vRecruitDataStorage.erase(std::unique(m_vRecruitDataStorage.begin(), m_vRecruitDataStorage.end()), m_vRecruitDataStorage.end());
	m_qSort.SetDataToSort(&m_vRecruitDataStorage, REC_HEADER_DATE);
	m_qSort.SortVector(0, (INT)(m_vRecruitDataStorage.size() - 1));

	SYSTEMTIME startDate, endDate;
	m_ctrlStartCalendar.GetCurSel(&startDate);
	m_timeManipulation.CStringToTime(m_vRecruitDataStorage[0][REC_HEADER_DATE], &startDate);
	m_ctrlStartCalendar.SetCurSel(&startDate);
	m_timeManipulation.AddDays(&startDate, 90);
	m_ctrlEndCalendar.SetCurSel(&startDate);
	m_ctrlStartCalendar.GetCurSel(&startDate);
	m_ctrlEndCalendar.GetCurSel(&endDate);
	CString csStartDate = m_timeManipulation.TimeToCString(startDate);
	CString csEndDate = m_timeManipulation.TimeToCString(endDate);

	DisplayRecData(csStartDate, csEndDate);
	return TRUE;
}

vector<vector<CString>> CDialogReportRecruitsDlg::ParseRecruitData(CString csRawData)
{
	CString csRawDataLine, csItem;
	INT nNextLineIndex = 0, nTabIndex = 0;
	vector<CString> vLine;
	std::vector<vector<CString>> m_mappedData;

	csRawDataLine.Empty();
	csItem.Empty();
	vLine.clear();
	m_mappedData.clear();
	csRawData.MakeUpper();

	while ((nNextLineIndex = csRawData.Find(_T("\r\n"))) != -1)
	{
		csRawDataLine = csRawData.Left(nNextLineIndex);
		csRawData = csRawData.Mid(nNextLineIndex + 2);
		if(!m_bIsPVA)
			vLine.push_back(_T("NO")); // push NO for Bap Column
		while ((nTabIndex = csRawDataLine.Find(_T("\t"))) != -1)
		{
			csItem = csRawDataLine.Left(nTabIndex);
			csRawDataLine = csRawDataLine.Mid(nTabIndex + 1);
			vLine.push_back(csItem);
		}

		if (!csRawDataLine.IsEmpty())
			vLine.push_back(csRawDataLine);

		m_mappedData.push_back(vLine);
		vLine.clear();
	}
	if (!csRawData.IsEmpty())
	{
		csRawDataLine = csRawData;
		if(!m_bIsPVA)
			vLine.push_back(_T("NO")); // push NO for Bap Column
		while ((nTabIndex = csRawDataLine.Find(_T("\t"))) != -1)
		{
			csItem = csRawDataLine.Left(nTabIndex);
			csRawDataLine = csRawDataLine.Mid(nTabIndex + 1);
			vLine.push_back(csItem);
		}

		if (!csRawDataLine.IsEmpty())
			vLine.push_back(csRawDataLine);

		m_mappedData.push_back(vLine);
		vLine.clear();
	}
	return m_mappedData;
}

void CDialogReportRecruitsDlg::DisplayRecData(CString csStartDate, CString csEndDate)
{ 
	int nItem = 0;
	CString csItems;

	m_ctrlListRecruit.DeleteAllItems();

	for (size_t rowId = 0; rowId < (int)m_vRecruitDataStorage.size(); rowId++)
	{
		if((m_qSort.CompareDate(m_vRecruitDataStorage[rowId][REC_HEADER_DATE], csStartDate) >= 0) &&
		   (m_qSort.CompareDate(m_vRecruitDataStorage[rowId][REC_HEADER_DATE], csEndDate) <= 0))
		{
			CString csNumber;

			csNumber.Format(_T("%d"), nItem + 1);

			m_ctrlListRecruit.InsertItem(LVIF_TEXT | LVIF_STATE, nItem,
				csNumber, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED, 0, 0);

			for (int col = 0; col < REC_HEADER_ACCREDITATION; col++)
			{
				m_ctrlListRecruit.SetItemText(nItem, col + 1, m_vRecruitDataStorage[rowId][col]);
			}

			nItem++;
		}
	}
	csItems.Format(_T("Items : %d"), nItem);
	m_staticItems.SetWindowText(csItems);
	DisplaySMDLegCount(csStartDate, csEndDate);
}

void CDialogReportRecruitsDlg::DisplaySMDLegCount(CString csStartDate, CString csEndDate)
{
	SMDLEGCOUNTER t_smdLegCount;
	int nFoundIndex = 0;
	double lfWithInRangeCounter = 0.0;

	t_smdLegCount.csSMDName = _T("");
	t_smdLegCount.lfLegCount = 0;
	t_smdLegCount.nBapCount = 0;
	
	m_vSMDLegCounter.clear();
	m_editCtrlDisplayCounter.SetWindowTextW(_T(""));
	for (size_t i = 0; i < (int)m_vRecruitDataStorage.size(); i++)
	{
		if ((m_qSort.CompareDate(m_vRecruitDataStorage[i][REC_HEADER_DATE], csStartDate) >= 0) &&
			(m_qSort.CompareDate(m_vRecruitDataStorage[i][REC_HEADER_DATE], csEndDate) <= 0))
		{
			if (IsSMDNameExist(m_vSMDLegCounter, m_vRecruitDataStorage[i][REC_HEADER_SMD], &nFoundIndex))
			{
				if (m_vRecruitDataStorage[i][REC_HEADER_BAP].Compare(_T("YES")) == 0)
				{
					m_vSMDLegCounter[nFoundIndex].nBapCount++;
					m_vSMDLegCounter[nFoundIndex].lfLegCount += 0.5;
					lfWithInRangeCounter += 0.5;
				}
				else
				{
					m_vSMDLegCounter[nFoundIndex].lfLegCount++;
					lfWithInRangeCounter++;
				}
			}
			else
			{
				t_smdLegCount.csSMDName = m_vRecruitDataStorage[i][REC_HEADER_SMD];
				t_smdLegCount.nBapCount = 0;

				if (m_vRecruitDataStorage[i][REC_HEADER_BAP].Compare(_T("YES")) == 0)
				{
					t_smdLegCount.lfLegCount = 0.5;
					t_smdLegCount.nBapCount++;
					lfWithInRangeCounter+=0.5;
				}
				else
				{
					t_smdLegCount.lfLegCount = 1.0;
					lfWithInRangeCounter++;
				}
				m_vSMDLegCounter.push_back(t_smdLegCount);
			}
		
		}
	}
	CString csDisplayReport = _T("");

	csDisplayReport.Format(_T("Your Total Recruits Count : %.2lf Recruits\r\n\r\n"), lfWithInRangeCounter);
	csDisplayReport.AppendFormat(_T("Your Total SMD leg(s) : %d\r\n"), (int)m_vSMDLegCounter.size());

	vector<vector<CString>> vDisplaySMDLegs;
	vector<CString> vPerson;
	CString csPoints;

	for (int i = 0; i < (int)m_vSMDLegCounter.size(); i++)
	{
		vPerson.push_back(m_vSMDLegCounter[i].csSMDName);
		csPoints.Format(_T("%.2lf"), m_vSMDLegCounter[i].lfLegCount);
		vPerson.push_back(csPoints);
		vDisplaySMDLegs.push_back(vPerson);
		vPerson.clear();
	}

	m_qSort.SetDataToSort(&vDisplaySMDLegs, 1);
	m_qSort.SortVector(0, vDisplaySMDLegs.size() - 1);

	for (int i = vDisplaySMDLegs.size() - 1; i >= 0; i--)
	{
		csDisplayReport.AppendFormat(_T("   SMD %s's Leg Count : %s  Recruits\r\n"), vDisplaySMDLegs[i][0], vDisplaySMDLegs[i][1]);
	}

	m_editCtrlDisplayCounter.SetWindowTextW(csDisplayReport);
}

BOOL CDialogReportRecruitsDlg::IsSMDNameExist(vector< SMDLEGCOUNTER> vLegCounter, CString csSMDName, INT* pIndex)
{
	BOOL bExist = FALSE;

	for (int i = 0; i < (int)vLegCounter.size(); i++)
	{
		if (!vLegCounter[i].csSMDName.Compare(csSMDName))
		{
			bExist = TRUE;
			*pIndex = i;
			break;
		}
	}
	return bExist;
}

void CDialogReportRecruitsDlg::SetRawData(CString csRawData, BOOL bIsPVA)
{
	m_csRawData = csRawData;
	m_bIsPVA = bIsPVA;
}

void CDialogReportRecruitsDlg::OnLvnItemchangedListRecruit(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
}

void CDialogReportRecruitsDlg::OnNMClickListRecruit(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: Add your control notification handler code here

	NMLISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	*pResult = 0;
}

void CDialogReportRecruitsDlg::OnRclickList(NMHDR* pNMHDR, LRESULT* pResult)
{
	*pResult = 0;
}

void CDialogReportRecruitsDlg::OnBAP()
{
	POSITION SelectionPos = m_ctrlListRecruit.GetFirstSelectedItemPosition();
	int iCurSel = -1;

	if (NULL != SelectionPos)
	{
		CString csStringToFind = m_ctrlListRecruit.GetItemText((size_t)(SelectionPos - 1), REC_HEADER_AGENTCODE + 1);

		auto it = m_vRecruitDataStorage[0].begin();
		size_t nItemFound = -1;
		for (nItemFound = 0; nItemFound < m_vRecruitDataStorage.size(); ++nItemFound)
		{
			it = find(m_vRecruitDataStorage[nItemFound].begin(), m_vRecruitDataStorage[nItemFound].end(), csStringToFind);
			if (m_vRecruitDataStorage[nItemFound].end() != it)
			{
				break;
			}
		}
		if (nItemFound >= 0)
		{
			CString csString;
			//m_pPopupMenu->GetMenuString(ID_BAP, csString, 0);
			if (csString.Compare(_T("BAP")) == 0)
			{
				m_ctrlListRecruit.SetItemText((size_t)SelectionPos - 1, (REC_HEADER_BAP + 1), _T("YES"));
				m_vRecruitDataStorage[nItemFound][REC_HEADER_BAP] = _T("YES");
			}
			else
			{
				m_ctrlListRecruit.SetItemText((size_t)SelectionPos - 1, (REC_HEADER_BAP + 1), _T("NO"));
				m_vRecruitDataStorage[nItemFound][REC_HEADER_BAP] = _T("NO");
			}

			INT index = m_ctrlListRecruit.GetNextSelectedItem(SelectionPos);
			m_ctrlListRecruit.SetItemState(index, 0, LVIS_FOCUSED | LVIS_SELECTED);
		}

		SYSTEMTIME startDate, endDate;
		m_ctrlStartCalendar.GetCurSel(&startDate);
		m_ctrlEndCalendar.GetCurSel(&endDate);

		CString csStartDate = m_timeManipulation.TimeToCString(startDate);
		CString csEndDate = m_timeManipulation.TimeToCString(endDate);

		DisplaySMDLegCount(csStartDate, csEndDate);
	}
}

void CDialogReportRecruitsDlg::OnPaint()
{
	CMFCPropertyPage::OnPaint();
}

void CDialogReportRecruitsDlg::OnNMHoverListRecruit(NMHDR* pNMHDR, LRESULT* pResult)
{
	// TODO: Add your control notification handler code here
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	LPNMLVCUSTOMDRAW  lplvcd = (LPNMLVCUSTOMDRAW)pNMHDR;

	*pResult = 0;

	if (pNMLV->iItem >= 0)
	{
		m_ctrlListRecruit.SetItemState(pNMLV->iItem, (LVIS_SELECTED|LVIS_FOCUSED), (LVIS_SELECTED|LVIS_FOCUSED));

		*pResult = 1;
	}
}

void CDialogReportRecruitsDlg::OnNMDblclkListRecruit(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;

	if (pNMItemActivate->iItem >= 0)
	{
		POSITION SelectionPos = m_ctrlListRecruit.GetFirstSelectedItemPosition();
		CString csString = m_ctrlListRecruit.GetItemText((size_t)(SelectionPos - 1), REC_HEADER_AGENTCODE + 1);

		auto it = m_vRecruitDataStorage[0].begin();
		size_t nItemFound = -1;
		for (nItemFound = 0; nItemFound < m_vRecruitDataStorage.size(); ++nItemFound)
		{
			 it = find(m_vRecruitDataStorage[nItemFound].begin(), m_vRecruitDataStorage[nItemFound].end(), csString);
			 if (m_vRecruitDataStorage[nItemFound].end() != it)
			 {
				 break;
			 }
		}
		if (nItemFound >= 0)
		{
			CMembersProfileDlg dlg;

			dlg.EditProfile(&m_vRecruitDataStorage[nItemFound]);
			if (dlg.DoModal() == IDOK)
			{
				for (int col = 1; col < REC_HEADER_ACCREDITATION; col++)
				{
					m_ctrlListRecruit.SetItemText((size_t)SelectionPos - 1, col, m_vRecruitDataStorage[nItemFound][col - 1]);
				}
			}
			POSITION pos = m_ctrlListRecruit.GetFirstSelectedItemPosition();
			INT index = m_ctrlListRecruit.GetNextSelectedItem(pos);
			m_ctrlListRecruit.SetItemState(index, 0, LVIS_FOCUSED | LVIS_SELECTED);
		}

		SYSTEMTIME startDate, endDate;
		m_ctrlStartCalendar.GetCurSel(&startDate);
		m_ctrlEndCalendar.GetCurSel(&endDate);

		CString csStartDate = m_timeManipulation.TimeToCString(startDate);
		CString csEndDate = m_timeManipulation.TimeToCString(endDate);

		DisplaySMDLegCount(csStartDate, csEndDate);

	}
}

void CDialogReportRecruitsDlg::OnMcnSelectMonthcalendarStart(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMSELCHANGE pSelChange = reinterpret_cast<LPNMSELCHANGE>(pNMHDR);
	CString csString;

	m_ctrlComboRunningDays.GetWindowText(csString);
	m_timeManipulation.AddDays(&(pSelChange->stSelStart), _ttoi(csString));
	m_ctrlEndCalendar.SetCurSel(&(pSelChange->stSelStart));

	SYSTEMTIME startDate, endDate;

	m_ctrlStartCalendar.GetCurSel(&startDate);
	m_ctrlEndCalendar.GetCurSel(&endDate);

	CString csStartDate = m_timeManipulation.TimeToCString(startDate);
	CString csEndDate = m_timeManipulation.TimeToCString(endDate);

	DisplayRecData(csStartDate, csEndDate);
	*pResult = 0;
}

void CDialogReportRecruitsDlg::OnMcnViewChangeMonthcalendarStart(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMVIEWCHANGE pSelChange = reinterpret_cast<LPNMVIEWCHANGE>(pNMHDR);

	*pResult = 0;
}

void CDialogReportRecruitsDlg::OnMcnSelchangeMonthcalendarStart(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMSELCHANGE pSelChange = reinterpret_cast<LPNMSELCHANGE>(pNMHDR);
	CString csString;

	m_ctrlComboRunningDays.GetWindowText(csString);
	m_timeManipulation.AddDays(&(pSelChange->stSelStart), _ttoi(csString));
	m_ctrlEndCalendar.SetCurSel(&(pSelChange->stSelStart));

	SYSTEMTIME startDate, endDate;

	m_ctrlStartCalendar.GetCurSel(&startDate);
	m_ctrlEndCalendar.GetCurSel(&endDate);

	CString csStartDate = m_timeManipulation.TimeToCString(startDate);
	CString csEndDate = m_timeManipulation.TimeToCString(endDate);

	DisplayRecData(csStartDate, csEndDate);
	*pResult = 0;
}

HBRUSH CDialogReportRecruitsDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CMFCPropertyPage::OnCtlColor(pDC, pWnd, nCtlColor);

	switch (nCtlColor)
	{
		case CTLCOLOR_STATIC:
		{
			if ((pWnd->GetDlgCtrlID() == IDC_EDIT_DISPLAY_COUNT))
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
		case CTLCOLOR_DLG:
		{
			pDC->SetTextColor(ENZO_COLOR_WHITE);
			pDC->SetBkColor(ENZO_COLOR_NAVYBLUE);
			pDC->SetBkMode(TRANSPARENT);
			return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
		}

		default:
			return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	}
	return hbr;
}

void CDialogReportRecruitsDlg::OnCbnSelchangeComboRunDays()
{
	// TODO: Add your control notification handler code here
	SYSTEMTIME todayTime;
	CString csString;

	m_ctrlComboRunningDays.GetWindowText(csString);
	m_ctrlStartCalendar.GetCurSel(&todayTime);
	m_timeManipulation.AddDays(&todayTime, _ttoi(csString));
	m_ctrlEndCalendar.SetCurSel(&todayTime);

	SYSTEMTIME startDate, endDate;

	m_ctrlStartCalendar.GetCurSel(&startDate);
	m_ctrlEndCalendar.GetCurSel(&endDate);

	CString csStartDate = m_timeManipulation.TimeToCString(startDate);
	CString csEndDate = m_timeManipulation.TimeToCString(endDate);

	DisplayRecData(csStartDate, csEndDate);
}

void CDialogReportRecruitsDlg::OnClose()
{
	// TODO: Add your message handler code here and/or call default

	CMFCPropertyPage::OnClose();
}


BOOL CDialogReportRecruitsDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default
	CRect rect;
	//COLORREF bkCol = ::GetSysColor(COLOR_MENU);
	pDC->GetClipBox(&rect);
	pDC->FillSolidRect(&rect, ENZO_COLOR_NAVYBLUE);
	return TRUE; // CMFCPropertyPage::OnEraseBkgnd(pDC);
}

void CDialogReportRecruitsDlg::GetDataToPrint(vector<vector<CString>> &data)
{
	INT nCount = m_ctrlListRecruit.GetItemCount();
	vector<CString> vItemRow;
	CString csItem;

	for (INT item = 0; item < nCount; item++)
	{
		csItem = m_ctrlListRecruit.GetItemText(item, REC_HEADER_NAME+1);
		vItemRow.push_back(csItem);
		csItem = m_ctrlListRecruit.GetItemText(item, REC_HEADER_AGENTCODE + 1);
		vItemRow.push_back(csItem);
		csItem = m_ctrlListRecruit.GetItemText(item, REC_HEADER_DATE+1);
		vItemRow.push_back(csItem);

		data.push_back(vItemRow);
		vItemRow.clear();
	}
}

void CDialogReportRecruitsDlg::OnBnClickedStaticTallyBoard()
{
	// TODO: Add your control notification handler code here
}
