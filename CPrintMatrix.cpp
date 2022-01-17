#include "pch.h"
#include "CPrintMatrix.h"

CPrintMatrix::CPrintMatrix()
{

}
CPrintMatrix::~CPrintMatrix()
{

}
CString CPrintMatrix::GetFormTypeString(ENUMPROMOTIONTYPE enumFormType)
{
	CString csType;

	switch (enumFormType)
	{
		case MD:
			csType = _T("MD");
			break;
		case SMD:
			csType = _T("SMD");
			break;
		case EMD:
			csType = _T("EMD");
			break;
		case GJ:
			csType = _T("GJ");
			break;
		case CEO:
			csType = _T("CEO");
			break;
		case EVC:
			csType = _T("EVC");
			break;
		case SEVC:
			csType = _T("SEVC");
			break;
		case OFFICE:
			csType = _T("OFFICE");
			break;
		case HMO:
			csType = _T("HMO");
			break;
		case LIMITEDPIN:
			csType = _T("LIMITEDPIN");
			break;
	}
	return csType;
}

CString CPrintMatrix::GetCurrentDirectory()
{
	DWORD dwSize = 0;
	TCHAR lpStr[MAX_PATH];
	CString csString;

	ZeroMemory(lpStr, sizeof(lpStr));

	dwSize = GetModuleFileName(NULL, lpStr, MAX_PATH);
	PathRemoveFileSpec(lpStr);

	csString.Format(_T("%s"), lpStr);

	return csString;
}

BOOL CPrintMatrix::LoadINIFile(ENUMPROMOTIONTYPE enumFormType)
{
	CString csSection = GetFormTypeString(enumFormType);
	CString csSubSectionFormat = csSection + _T("_%d");
	CString csSubSection;
	CString csIniFileName = GetCurrentDirectory() + _T("\\Forms.ini");
	PRINTPAGEMATRIX tPrintMatrix;
	FILE* fStream;

	ZeroMemory(&tPrintMatrix, sizeof(PRINTPAGEMATRIX));

	errno_t errCode = _tfopen_s(&fStream, csIniFileName, _T("r, ccs=UTF-8"));
	if (0 != errCode)
		return FALSE;
	fclose(fStream);

	m_documentMatrix.nNamePos.x			= GetPrivateProfileInt(csSection, _T("nNamePos.x"), -1, csIniFileName);
	m_documentMatrix.nNamePos.y			= GetPrivateProfileInt(csSection, _T("nNamePos.y"), -1, csIniFileName);
	m_documentMatrix.nCodePos.x			= GetPrivateProfileInt(csSection, _T("nCodePos.x"), -1, csIniFileName);
	m_documentMatrix.nCodePos.y			= GetPrivateProfileInt(csSection, _T("nCodePos.y"), -1, csIniFileName);
	m_documentMatrix.nDatePos.x			= GetPrivateProfileInt(csSection, _T("nDatePos.x"), -1, csIniFileName);
	m_documentMatrix.nDatePos.y			= GetPrivateProfileInt(csSection, _T("nDatePos.y"), -1, csIniFileName);
	m_documentMatrix.nRankPos.x			= GetPrivateProfileInt(csSection, _T("nRankPos.x"), -1, csIniFileName);
	m_documentMatrix.nRankPos.y			= GetPrivateProfileInt(csSection, _T("nRankPos.y"), -1, csIniFileName);
	m_documentMatrix.nSignaturePos.x	= GetPrivateProfileInt(csSection, _T("nSignaturePos.x"), -1, csIniFileName);
	m_documentMatrix.nSignaturePos.y	= GetPrivateProfileInt(csSection, _T("nSignaturePos.y"), -1, csIniFileName);
	m_documentMatrix.nUplineNamePos.x	= GetPrivateProfileInt(csSection, _T("nUplineNamePos.x"), -1, csIniFileName);
	m_documentMatrix.nUplineNamePos.y	= GetPrivateProfileInt(csSection, _T("nUplineNamePos.y"), -1, csIniFileName);
	m_documentMatrix.nPageCount			= GetPrivateProfileInt(csSection, _T("nPageCount"), -1, csIniFileName);

	for(int i = 0 ; i < m_documentMatrix.nPageCount ; i++)
	{ 
		csSubSection.Format(csSubSectionFormat, i);
		tPrintMatrix.nStartPos.x		= GetPrivateProfileInt(csSubSection, _T("nStartPos.x"), -1, csIniFileName);
		tPrintMatrix.nStartPos.y		= GetPrivateProfileInt(csSubSection, _T("nStartPos.y"), -1, csIniFileName);
		tPrintMatrix.nItemsPerPage		= GetPrivateProfileInt(csSubSection, _T("nItemsPerPage"), -1, csIniFileName);
		tPrintMatrix.nRowSpacing		= GetPrivateProfileInt(csSubSection, _T("nRowSpacing"), -1, csIniFileName);
		tPrintMatrix.nColSpacing[0]		= GetPrivateProfileInt(csSubSection, _T("nColSpacing[0]"), -1, csIniFileName);
		tPrintMatrix.nColSpacing[1]		= GetPrivateProfileInt(csSubSection, _T("nColSpacing[1]"), -1, csIniFileName);
		tPrintMatrix.nColSpacing[2]		= GetPrivateProfileInt(csSubSection, _T("nColSpacing[2]"), -1, csIniFileName);
		tPrintMatrix.nColSpacing[3]		= GetPrivateProfileInt(csSubSection, _T("nColSpacing[3]"), -1, csIniFileName);
		tPrintMatrix.nColSpacing[4]		= GetPrivateProfileInt(csSubSection, _T("nColSpacing[4]"), -1, csIniFileName);
		tPrintMatrix.nColSpacing[5]		= GetPrivateProfileInt(csSubSection, _T("nColSpacing[5]"), -1, csIniFileName);
		m_documentMatrix.vPrintPageMatrix.push_back(tPrintMatrix);
	}
	return TRUE;
}