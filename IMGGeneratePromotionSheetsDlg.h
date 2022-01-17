
// IMGGeneratePromotionSheetsDlg.h : header file
//

#pragma once
#include "Resource.h"
#include "CustomHeader.h"
#include "CPrintMatrix.h"
#include <vector>

using namespace std;


// CIMGGeneratePromotionSheetsDlg dialog

typedef struct tagPrintMatrix
{
	COORDINATES nStartPos;
	INT nItemsPerPage;
	INT nColSpacing[6];
	INT nRowSpacing;
}PRINTMATRIX;


typedef struct tagHeaderData
{
	COORDINATES nNamePos;
	COORDINATES nCodePos;
	COORDINATES nDatePos;
	COORDINATES nSignaturePos;
	COORDINATES nUplineNamePos;
	COORDINATES nRankPos;

}HEADERDATA;

class CIMGGeneratePromotionSheetsDlg : public CDialogEx
{
// Construction
public:
	CIMGGeneratePromotionSheetsDlg(CWnd* pParent = nullptr);	// standard constructor
	~CIMGGeneratePromotionSheetsDlg();
// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_IMGGENERATEPROMOTIONSHEETS_DIALOG };
#endif
	void SetVersion(CString csVersion)
	{
		m_csVersion = csVersion;
	}

	void SetHWND(HWND hWnd)
	{
		m_hWndCallingProcess = hWnd;
	}
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;
	HWND  m_hWndCallingProcess;
	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	CStatic m_ctrlPreview;
	CButton m_btnPrevious;
	HBRUSH m_hBrushPrevBackGround;
	HBRUSH m_hBrushBackGround;
	BOOL DisplayPreview();
	BOOL LoadBMPTemplates();
	void WriteDataToImage();
	void SetWindowSize();
	BOOL LoadIMGData();
	INT ParseRawData(CString csRawData);
	BOOL InitializePrintMatrix();
	CString GetCurrentDirectory();
	void GetRectRatio(int nItem, LPRECT lpRect);
	INT m_nPage;
	vector<CBitmap *> m_vCBitmaps;
	vector<vector<CString>> m_vIMGData;
	ENUMPROMOTIONTYPE m_enumPromotionType;
	CString m_csNamePromotee;
	CString m_csCodeNumber;
	CString m_csUplineName;
	CString m_csDateCompletion;
	CString m_csCurrentRank;
	CString m_csVersion;
	DOCUMENTMATRIX m_tagDocumentMatrix;
	vector<CString> m_vListForm;

public:
	CButton m_btnNext;
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBnClickedButtonPrev();
protected:
	CButton m_btnPrint;
public:
	afx_msg void OnBnClickedButtonPrint();
	afx_msg void OnBnClickedButtonNext();
protected:
	CStatic m_staticTextPage;
public:
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
protected:
	CButton m_btnRefresh;
	INT m_nPagesToPrint;
public:
	afx_msg void OnBnClickedButtonRefresh();
};
