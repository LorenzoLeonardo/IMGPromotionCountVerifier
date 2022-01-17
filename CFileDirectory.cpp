#include "pch.h"
#include "CFileDirectory.h"

CFileDirectory::CFileDirectory()
{

}
CFileDirectory::~CFileDirectory()
{

}

CString CFileDirectory::GetCurrentDirectory()
{
	DWORD dwSize = 0;
	LPTSTR lpStr = NULL;
	CString csString;

	dwSize = ::GetCurrentDirectory(dwSize, lpStr);
	lpStr = new TCHAR[dwSize + 1];
	dwSize = ::GetCurrentDirectory(dwSize, lpStr);

	csString.Format(_T("%s"), lpStr);
	delete[] lpStr;
	lpStr = NULL;
	return csString;
}

CString CFileDirectory::OpenFile()
{
	TCHAR szFilters[] = _T("Members/Productions Text Files (*.txt)|*.txt|");
	SetCurrentDirectory(GetCurrentDirectory());

	// Create an Open dialog; the default file name extension is ".my".
	CFileDialog dlg(TRUE, _T("txt"), _T("*.txt"),
		OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, szFilters);

	dlg.DoModal();
	return dlg.GetFolderPath() + _T("\\") + dlg.GetFileName();
}

CString CFileDirectory::OpenPVAFile()
{
	TCHAR szFilters[] = _T("IMG pva files (*.pva)|*.pva|");
	SetCurrentDirectory(GetCurrentDirectory());

	// Create an Open dialog; the default file name extension is ".my".
	CFileDialog dlg(TRUE, _T("pva"), _T("*.pva"),
		OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, szFilters);

	if (dlg.DoModal() == IDOK)
		return dlg.GetFolderPath() + _T("\\") + dlg.GetFileName();
	else
		return _T("");
}

CString CFileDirectory::SaveFile()
{
	TCHAR szFilters[] = _T("IMG pva files (*.pva)|*.pva|");
	SetCurrentDirectory(GetCurrentDirectory());

	// Create an Open dialog; the default file name extension is ".my".
	CFileDialog dlg(FALSE, _T("pva"), _T(""),
		OFN_OVERWRITEPROMPT | OFN_CREATEPROMPT, szFilters);

	dlg.DoModal();

	return dlg.GetFolderPath() + _T("\\") + dlg.GetFileName();
}

CString CFileDirectory::GetPDFFilePath()
{
	CString csPDFOutput;
	TCHAR szFilters[] = _T("PDF Files (*.pdf)|*.pdf|");
	CFileDialog csFile(TRUE, _T("pdf"), _T("*.pdf"),
		OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, szFilters);

	csPDFOutput = csFile.GetFolderPath();

	CFileFind finder;

	// build a string with wildcards
	CString strWildcard;
	strWildcard = csPDFOutput + _T("\\*.pdf");
	FILETIME curTime = { 0 }, prevTime = { 0 };
	// start working for files
	BOOL bWorking = finder.FindFile(strWildcard);

	while (bWorking)
	{
		bWorking = finder.FindNextFile();

		// skip . and .. files; otherwise, we'd
		// recur infinitely!

		if (finder.IsDots())
			continue;

		// if it's a directory, recursively search it

		if (!finder.IsDirectory())
		{
			finder.GetLastWriteTime(&curTime);
			
			if (CompareFileTime(&curTime, &prevTime) > 0)
			{
				csPDFOutput = finder.GetFilePath();
				prevTime = curTime;
			}
		}
	}

	finder.Close();

	return csPDFOutput;
}