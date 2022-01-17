// CDialogReportSalesDlg.cpp : implementation file
//

#include "pch.h"
#include "IMGPromotionCountVerifier.h"
#include "CDialogReportSalesDlg.h"
#include "CPlanHoldersInformationDlg.h"
#include "afxdialogex.h"


// CDialogReportSalesDlg dialog

IMPLEMENT_DYNAMIC(CDialogReportSalesDlg, CMFCPropertyPage)

CDialogReportSalesDlg::CDialogReportSalesDlg(CWnd* pParent /*=nullptr*/)
	: CMFCPropertyPage(IDD_DIALOG_REPORT_SALES)
{
	m_bIsVisited = FALSE;
	m_hBrushBackGround = CreateSolidBrush(ENZO_COLOR_NAVYBLUE);
	m_hBrushEditCtrl = CreateSolidBrush(ENZO_COLOR_DIRTYWHITE);
}

CDialogReportSalesDlg::~CDialogReportSalesDlg()
{
	DeleteObject(m_hBrushBackGround);
	DeleteObject(m_hBrushEditCtrl);
}

void CDialogReportSalesDlg::DoDataExchange(CDataExchange* pDX)
{
	CMFCPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MONTHCALENDAR_START, m_ctrlStartCalendar);
	DDX_Control(pDX, IDC_MONTHCALENDAR_END, m_ctrlEndCalendar);
	DDX_Control(pDX, IDC_COMBO_RUN_DAYS, m_ctrlComboRunningDays);
	DDX_Control(pDX, IDC_LIST_SALE, m_ctrlListSale);
	DDX_Control(pDX, IDC_EDIT_DISPLAY_COUNT, m_editCtrlDisplayCounter);
	DDX_Control(pDX, IDC_STATIC_ITEMS, m_staticItems);
}

BEGIN_MESSAGE_MAP(CDialogReportSalesDlg, CMFCPropertyPage)
	ON_WM_CTLCOLOR()
	ON_WM_CLOSE()
	ON_NOTIFY(MCN_SELCHANGE, IDC_MONTHCALENDAR_START, &CDialogReportSalesDlg::OnMcnSelchangeMonthcalendarStart)
	ON_NOTIFY(MCN_SELECT, IDC_MONTHCALENDAR_START, &CDialogReportSalesDlg::OnMcnSelectMonthcalendarStart)
	ON_CBN_EDITCHANGE(IDC_COMBO_RUN_DAYS, &CDialogReportSalesDlg::OnCbnEditchangeComboRunDays)
	ON_CBN_SELCHANGE(IDC_COMBO_RUN_DAYS, &CDialogReportSalesDlg::OnCbnSelchangeComboRunDays)
	ON_NOTIFY(LVN_HOTTRACK, IDC_LIST_SALE, &CDialogReportSalesDlg::OnNMHoverListSale)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_SALE, &CDialogReportSalesDlg::OnNMDblclkListSale)
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


BOOL CDialogReportSalesDlg::OnInitDialog()
{
	CMFCPropertyPage::OnInitDialog();
	typedef struct
	{
		CString csHeaderName;
		int nFormat;
		INT nHeaderSize;
	} t_Header;
	m_bIsVisited = TRUE;
	t_Header lpcSaleHeader[] = {	
									{_T("No."),					LVCFMT_FIXED_WIDTH,	30},
									{_T("BAP"),					LVCFMT_FIXED_WIDTH,	35},
									{_T("Provider Code"),		LVCFMT_FIXED_WIDTH,		0},
									{_T("Application Number"),	LVCFMT_FIXED_WIDTH,		0},
									{_T("Product Code"),		LVCFMT_FIXED_WIDTH,		0},
									{_T("Policy Number"),		LVCFMT_FIXED_WIDTH,		0},
									{_T("Policy Status"),		LVCFMT_FIXED_WIDTH,		0},
									{_T("Plan Holder"), 		LVCFMT_LEFT,		200},
									{_T("Plan Name"),			LVCFMT_LEFT,		100},
									{_T("Plan Amount"),			LVCFMT_FIXED_WIDTH,		0},

									{_T("Installment Amount"),	LVCFMT_FIXED_WIDTH,		0},
									{_T("Business Date"),		LVCFMT_LEFT,		80},
									{_T("Agent Code 1"),		LVCFMT_LEFT,		80},
									{_T("Agent Name 1"),		LVCFMT_LEFT,		160},
									{_T("Agent Code 2"),		LVCFMT_LEFT,		80},
									{_T("Agent Name 2"),		LVCFMT_LEFT,		160},
									{_T("Baseshop"),			LVCFMT_FIXED_WIDTH,		0},
									{_T("Hierarchy"),			LVCFMT_FIXED_WIDTH,		0},
									{_T("SMD"),					LVCFMT_LEFT,		160}
							   };

	pm_obMenu = new CMenu;
	pm_obMenu->LoadMenu(IDR_LIST_POPUP);
	m_pPopupMenu = pm_obMenu->GetSubMenu(0);

	for(int nCol = 0; nCol <= SALE_HEADER_END; nCol++)
		m_ctrlListSale.InsertColumn(nCol, lpcSaleHeader[nCol].csHeaderName, lpcSaleHeader[nCol].nFormat, lpcSaleHeader[nCol].nHeaderSize);
	m_ctrlListSale.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_TRACKSELECT);

	m_ctrlComboRunningDays.AddString(_T("30"));
	m_ctrlComboRunningDays.AddString(_T("60"));
	m_ctrlComboRunningDays.AddString(_T("90"));
	m_ctrlComboRunningDays.AddString(_T("365"));
	m_ctrlComboRunningDays.SetCurSel(2);

	m_csRawData.TrimLeft();
	m_csRawData.TrimRight();
	m_vSalesDataStorage = ParseRawData(m_csRawData);

	m_qSort.SetDataToSort(&m_vSalesDataStorage, SALE_HEADER_PLANHOLDER);
	m_qSort.SortVector(0, (INT)(m_vSalesDataStorage.size() - 1));
	m_vSalesDataStorage.erase(std::unique(m_vSalesDataStorage.begin(), m_vSalesDataStorage.end()), m_vSalesDataStorage.end());
	m_qSort.SetDataToSort(&m_vSalesDataStorage, SALE_HEADER_BUSINESSDATE);
	m_qSort.SortVector(0, (INT)(m_vSalesDataStorage.size() - 1));

	SYSTEMTIME startDate, endDate;
	m_ctrlStartCalendar.GetCurSel(&startDate);
	m_timeManipulation.CStringToTime(m_vSalesDataStorage[0][SALE_HEADER_BUSINESSDATE], &startDate);
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

void CDialogReportSalesDlg::DisplayRecData(CString csStartDate, CString csEndDate)
{
	int nItem = 0;
	CString csItems;

	m_ctrlListSale.DeleteAllItems();

	for (size_t rowId = 0; rowId < (int)m_vSalesDataStorage.size(); rowId++)
	{
		if ((m_qSort.CompareDate(m_vSalesDataStorage[rowId][SALE_HEADER_BUSINESSDATE], csStartDate) >= 0) &&
			(m_qSort.CompareDate(m_vSalesDataStorage[rowId][SALE_HEADER_BUSINESSDATE], csEndDate) <= 0))
		{
			CString csNumber;

			csNumber.Format(_T("%d"), nItem + 1);

			m_ctrlListSale.InsertItem(LVIF_TEXT | LVIF_STATE, nItem,
				csNumber, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED, 0, 0);

			for (int col = 0; col < (SALE_HEADER_SMD+1); col++)
			{
				m_ctrlListSale.SetItemText(nItem, col + 1, m_vSalesDataStorage[rowId][col]);
			}

			nItem++;
		}
	}
	csItems.Format(_T("Items : %d"), nItem);
	m_staticItems.SetWindowText(csItems);
	TallyCount(csStartDate, csEndDate);
}

vector<vector<CString>> CDialogReportSalesDlg::ParseRawData(CString csRawData)
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

		if (vLine.size() != SALE_HEADER_END)//if no MD from source, then put empty
			vLine.push_back(_T("<INPUT SMD NAME>"));

		m_mappedData.push_back(vLine);
		vLine.clear();
	}
	if (!csRawData.IsEmpty())
	{
		csRawDataLine = csRawData;
		if (!m_bIsPVA)
			vLine.push_back(_T("NO")); // push NO for Bap Column
		while ((nTabIndex = csRawDataLine.Find(_T("\t"))) != -1)
		{
			csItem = csRawDataLine.Left(nTabIndex);
			csRawDataLine = csRawDataLine.Mid(nTabIndex + 1);
			vLine.push_back(csItem);
		}

		if (!csRawDataLine.IsEmpty())
			vLine.push_back(csRawDataLine);

		if (vLine.size() != SALE_HEADER_END)//if no MD from source, then put empty
			vLine.push_back(_T("<INPUT SMD NAME>"));

		m_mappedData.push_back(vLine);
		vLine.clear();
	}
	return m_mappedData;
}

HBRUSH CDialogReportSalesDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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
		return m_hBrushBackGround;
	}

	default:
		return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	}
	return hbr;
}


void CDialogReportSalesDlg::OnClose()
{
	// TODO: Add your message handler code here and/or call default

	CMFCPropertyPage::OnClose();
}


void CDialogReportSalesDlg::OnMcnSelchangeMonthcalendarStart(NMHDR* pNMHDR, LRESULT* pResult)
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


void CDialogReportSalesDlg::OnMcnSelectMonthcalendarStart(NMHDR* pNMHDR, LRESULT* pResult)
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


void CDialogReportSalesDlg::OnCbnEditchangeComboRunDays()
{
	// TODO: Add your control notification handler code here
}

void CDialogReportSalesDlg::OnCbnSelchangeComboRunDays()
{
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


void CDialogReportSalesDlg::OnNMHoverListSale(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	LPNMLVCUSTOMDRAW  lplvcd = (LPNMLVCUSTOMDRAW)pNMHDR;

	*pResult = 0;
	if (pNMLV->iItem >= 0)
	{
		m_ctrlListSale.SetItemState(pNMLV->iItem, (LVIS_SELECTED | LVIS_FOCUSED), (LVIS_SELECTED | LVIS_FOCUSED));
		*pResult = 1;
	}
}


void CDialogReportSalesDlg::OnNMDblclkListSale(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;

	if (pNMItemActivate->iItem >= 0)
	{
		POSITION SelectionPos = m_ctrlListSale.GetFirstSelectedItemPosition();
		CString csString = m_ctrlListSale.GetItemText((size_t)(SelectionPos - 1), SALE_HEADER_PLANHOLDER + 1);

		auto it = m_vSalesDataStorage[0].begin();
		size_t nItemFound = -1;
		for (nItemFound = 0; nItemFound < m_vSalesDataStorage.size(); ++nItemFound)
		{
			it = find(m_vSalesDataStorage[nItemFound].begin(), m_vSalesDataStorage[nItemFound].end(), csString);
			if (m_vSalesDataStorage[nItemFound].end() != it)
			{
				break;
			}
		}
		if (nItemFound >= 0)
		{
			CPlanHoldersInformationDlg dlg;

			dlg.EditProfile(&m_vSalesDataStorage[nItemFound]);
			if (dlg.DoModal() == IDOK)
			{
				for (int col = 1; col <= SALE_HEADER_END; col++)
				{
					m_ctrlListSale.SetItemText((size_t)SelectionPos - 1, col, m_vSalesDataStorage[nItemFound][col - 1]);
				}
			}
			POSITION pos = m_ctrlListSale.GetFirstSelectedItemPosition();
			INT index = m_ctrlListSale.GetNextSelectedItem(pos);
			m_ctrlListSale.SetItemState(index, 0, LVIS_FOCUSED | LVIS_SELECTED);
		}

		SYSTEMTIME startDate, endDate;
		m_ctrlStartCalendar.GetCurSel(&startDate);
		m_ctrlEndCalendar.GetCurSel(&endDate);

		CString csStartDate = m_timeManipulation.TimeToCString(startDate);
		CString csEndDate = m_timeManipulation.TimeToCString(endDate);

		TallyCount(csStartDate, csEndDate);

	}
}
int Compare(const void* a, const void* b)
{
	return ((*(SALESPOINTCOUNTER*)a).lfFirstGenCount - (*(SALESPOINTCOUNTER*)b).lfFirstGenCount);
}
void CDialogReportSalesDlg::TallyCount(CString csStartDate, CString csEndDate)
{
	SALESPOINTCOUNTER tSalesCounter;
	INT nBaseshop = 0, nHierarchy = 0;
	vector<SALESPOINTCOUNTER> vSalesCounter;
	INT nFoundIndex = 0;
	double lfFirstGenCount = 0.0, lfBaseshopCount = 0.0;
	CQuickSort qSort;

	vSalesCounter.clear();
	tSalesCounter.csSMDName.Empty();
	tSalesCounter.lfBaseshopCount = 0.0;
	tSalesCounter.lfFirstGenCount = 0.0;
	tSalesCounter.nBapCount = 0;
	m_editCtrlDisplayCounter.SetWindowText(_T(""));
	m_vSMDLegCounter.clear();

	for (size_t i = 0; i < (int)m_vSalesDataStorage.size(); i++)
	{
		if ((m_qSort.CompareDate(m_vSalesDataStorage[i][SALE_HEADER_BUSINESSDATE], csStartDate) >= 0) &&
			(m_qSort.CompareDate(m_vSalesDataStorage[i][SALE_HEADER_BUSINESSDATE], csEndDate) <= 0))
		{
			nBaseshop = _ttoi(m_vSalesDataStorage[i][SALE_HEADER_BASESHOP]);
			nHierarchy = _ttoi(m_vSalesDataStorage[i][SALE_HEADER_HIERARCHY]);

			if (IsSMDNameExist(m_vSMDLegCounter, m_vSalesDataStorage[i][SALE_HEADER_SMD], &nFoundIndex))
			{
				if ((nBaseshop == 0) && (nHierarchy == 1))
				{
					if (m_vSalesDataStorage[i][SALE_HEADER_BAP].Compare(_T("YES")) == 0)
					{
						m_vSMDLegCounter[nFoundIndex].lfFirstGenCount += 0.5;
						m_vSMDLegCounter[nFoundIndex].nBapCount++;
					}
					else
					{
						m_vSMDLegCounter[nFoundIndex].lfFirstGenCount++;
					}
				}
				else if ((nBaseshop > 0) && (nHierarchy == 0))
				{
					if (m_vSalesDataStorage[i][SALE_HEADER_BAP].Compare(_T("YES")) == 0)
					{
						m_vSMDLegCounter[nFoundIndex].lfBaseshopCount += 0.5;
						m_vSMDLegCounter[nFoundIndex].nBapCount++;
					}
					else
					{
						m_vSMDLegCounter[nFoundIndex].lfBaseshopCount++;
					}
				}
				else if ((nBaseshop > 0) && (nHierarchy > 8))
				{
					if (m_vSalesDataStorage[i][SALE_HEADER_BAP].Compare(_T("YES")) == 0)
					{
						m_vSMDLegCounter[nFoundIndex].lfBaseshopCount += 0.5;
						m_vSMDLegCounter[nFoundIndex].nBapCount++;
					}
					else
					{
						m_vSMDLegCounter[nFoundIndex].lfBaseshopCount++;
					}
				}
				else if ((nBaseshop > 0) && (nHierarchy == 1))
				{
					if (m_vSalesDataStorage[i][SALE_HEADER_BAP].Compare(_T("YES")) == 0)
					{
						m_vSMDLegCounter[nFoundIndex].lfBaseshopCount += 0.25;
						m_vSMDLegCounter[nFoundIndex].lfFirstGenCount += 0.25;
					}
					else
					{
						m_vSMDLegCounter[nFoundIndex].lfBaseshopCount += 0.5;
						m_vSMDLegCounter[nFoundIndex].lfFirstGenCount += 0.5;
					}
				}
				else if ((nBaseshop > 0) && ((1 < nHierarchy) && (nHierarchy <= 8)))
				{
					if (m_vSalesDataStorage[i][SALE_HEADER_BAP].Compare(_T("YES")) == 0)
					{
						m_vSMDLegCounter[nFoundIndex].lfBaseshopCount += 0.25;
					}
					else
					{
						m_vSMDLegCounter[nFoundIndex].lfBaseshopCount += 0.5;
					}
				}
			}
			else
			{
				tSalesCounter.csSMDName = m_vSalesDataStorage[i][SALE_HEADER_SMD];
				tSalesCounter.lfBaseshopCount = 0.0;
				tSalesCounter.lfFirstGenCount = 0.0;
				tSalesCounter.nBapCount = 0;

				if ((nBaseshop == 0) && (nHierarchy == 1))
				{
					if (m_vSalesDataStorage[i][SALE_HEADER_BAP].Compare(_T("YES")) == 0)
					{
						tSalesCounter.lfFirstGenCount += 0.5;
						tSalesCounter.nBapCount++;
					}
					else
					{
						tSalesCounter.lfFirstGenCount++;
					}
				}
				else if ((nBaseshop > 0) && (nHierarchy == 0))
				{
					if (m_vSalesDataStorage[i][SALE_HEADER_BAP].Compare(_T("YES")) == 0)
					{
						tSalesCounter.lfBaseshopCount += 0.5;
						tSalesCounter.nBapCount++;
					}
					else
					{
						tSalesCounter.lfBaseshopCount++;
					}
				}
				else if ((nBaseshop > 0) && (nHierarchy > 8))
				{
					if (m_vSalesDataStorage[i][SALE_HEADER_BAP].Compare(_T("YES")) == 0)
					{
						tSalesCounter.lfBaseshopCount += 0.5;
						tSalesCounter.nBapCount++;
					}
					else
					{
						tSalesCounter.lfBaseshopCount++;
					}
				}
				else if ((nBaseshop > 0) && (nHierarchy == 1))
				{
					if (m_vSalesDataStorage[i][SALE_HEADER_BAP].Compare(_T("YES")) == 0)
					{
						tSalesCounter.lfBaseshopCount += 0.25;
						tSalesCounter.lfFirstGenCount += 0.25;
					}
					else
					{
						tSalesCounter.lfBaseshopCount += 0.5;
						tSalesCounter.lfFirstGenCount += 0.5;
					}
				}
				else if ((nBaseshop > 0) && ((1 < nHierarchy) && (nHierarchy <= 8)))
				{
					if (m_vSalesDataStorage[i][SALE_HEADER_BAP].Compare(_T("YES")) == 0)
					{
						tSalesCounter.lfBaseshopCount += 0.25;
					}
					else
					{
						tSalesCounter.lfBaseshopCount += 0.5;
					}
				}
				m_vSMDLegCounter.push_back(tSalesCounter);
			}
		}
	}

	CString csDisplayReport = _T("");
	tSalesCounter.lfBaseshopCount = 0.0;
	tSalesCounter.lfFirstGenCount = 0.0;

	for (int i = 0; i < (int)m_vSMDLegCounter.size(); i++)
	{
		tSalesCounter.lfBaseshopCount += m_vSMDLegCounter[i].lfBaseshopCount;
		tSalesCounter.lfFirstGenCount += m_vSMDLegCounter[i].lfFirstGenCount;
	}
	int nSMDCounter = 0;
	for (int i = 0; i < (int)m_vSMDLegCounter.size(); i++)
	{
		if (m_vSMDLegCounter[i].lfFirstGenCount > 0)
			nSMDCounter++;
	}

	csDisplayReport.Format(_T("Your Total Sales Count : %.2lf Sales\r\n"), tSalesCounter.lfBaseshopCount + tSalesCounter.lfFirstGenCount);
	csDisplayReport.AppendFormat(_T("Your Total Baseshop Sales Count : %.2lf Sales\r\n"), tSalesCounter.lfBaseshopCount);
	csDisplayReport.AppendFormat(_T("Your Total First Gen Sales Count : %.2lf Sales\r\n\r\n"), tSalesCounter.lfFirstGenCount);
	csDisplayReport.AppendFormat(_T("Your Total SMD Leg(s) : %d\r\n"), nSMDCounter);

	vector<vector<CString>> vDisplaySMDLegs;
	vector<CString> vPerson;
	CString csPoints;

	for (int i = 0; i < (int)m_vSMDLegCounter.size(); i++)
	{
		vPerson.push_back(m_vSMDLegCounter[i].csSMDName);
		csPoints.Format(_T("%.2lf"), m_vSMDLegCounter[i].lfFirstGenCount);
		vPerson.push_back(csPoints);
		vDisplaySMDLegs.push_back(vPerson);
		vPerson.clear();
	}

	m_qSort.SetDataToSort(&vDisplaySMDLegs, 1);
	m_qSort.SortVector(0, vDisplaySMDLegs.size() - 1);

	for (int i = vDisplaySMDLegs.size()-1; i >= 0; i--)
	{
		if(_ttof(vDisplaySMDLegs[i][1]) > 0)
			csDisplayReport.AppendFormat(_T("   SMD %s's Leg Count : %s  Sales\r\n"), vDisplaySMDLegs[i][0], vDisplaySMDLegs[i][1]);
	}
	m_editCtrlDisplayCounter.SetWindowText(csDisplayReport);
}

BOOL CDialogReportSalesDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default
	CRect rect;
	//COLORREF bkCol = ::GetSysColor(COLOR_MENU);
	pDC->GetClipBox(&rect);
	pDC->FillSolidRect(&rect, ENZO_COLOR_NAVYBLUE);
	return TRUE; // CMFCPropertyPage::OnEraseBkgnd(pDC);
}
void CDialogReportSalesDlg::GetDataToPrint(vector<vector<CString>>& data)
{
	INT nCount = m_ctrlListSale.GetItemCount();
	vector<CString> vItemRow;
	CString csItem;

	for (INT item = 0; item < nCount; item++)
	{
		csItem = m_ctrlListSale.GetItemText(item,SALE_HEADER_PLANHOLDER+1);
		vItemRow.push_back(csItem);
		csItem = m_ctrlListSale.GetItemText(item, SALE_HEADER_PLANNAME+1);
		vItemRow.push_back(csItem);
		csItem = m_ctrlListSale.GetItemText(item, SALE_HEADER_BUSINESSDATE+1);
		vItemRow.push_back(csItem);
		data.push_back(vItemRow);
		vItemRow.clear();
	}
}