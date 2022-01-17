#include "pch.h"
#include "resource.h"
#include "DropEdit.h"
#include <sys/types.h>
#include <sys/stat.h>


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDropEdit

CDropEdit::CDropEdit()
{
	m_bUseDir=FALSE;
}

CDropEdit::~CDropEdit()
{
}


BEGIN_MESSAGE_MAP(CDropEdit, CEdit)
	//{{AFX_MSG_MAP(CDropEdit)
	ON_WM_DROPFILES()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDropEdit message handlers

/////////////////////////////////////////////////////////////////////////////
//	handles WM_DROPFILES

void CDropEdit::OnDropFiles(HDROP dropInfo)
{
	// Get the number of pathnames that have been dropped
	WORD wNumFilesDropped = DragQueryFile(dropInfo, -1, NULL, 0);

	CString csFirstFile = _T("");

	// there may be many, but we'll only use the first
	if (wNumFilesDropped > 0)
	{
		// Get the number of bytes required by the file's full pathname
		WORD wPathnameSize = DragQueryFile(dropInfo, 0, NULL, 0);

		// Allocate memory to contain full pathname & zero byte
		wPathnameSize+=1;

		TCHAR * pFile = new TCHAR[wPathnameSize];
		if (pFile == NULL)
		{
			ASSERT(0);
			DragFinish(dropInfo);
			return;
		}

		// Copy the pathname into the buffer
		DragQueryFile(dropInfo, 0, pFile, wPathnameSize);

		csFirstFile = pFile;

		// clean up
		delete [] pFile; 
		pFile = NULL;
	}

	// Free the memory block containing the dropped-file information
	DragFinish(dropInfo);

	// if this was a shortcut, we need to expand it to the target path
	CString csExpandedFile = ExpandShortcut(csFirstFile);

	// if that worked, we should have a real file name
	if (!csExpandedFile.IsEmpty()) 
	{
		csFirstFile = csExpandedFile;
	}

	if (!csFirstFile.IsEmpty())
	{
		struct _stat buf;
	
		// get some info about that file
		int result = _tstat( csFirstFile, &buf );
		if( result == 0 ) 
		{
			// verify that we have a dir (if we want dirs)
			if ((buf.st_mode & _S_IFDIR) == _S_IFDIR) 
			{
				if (m_bUseDir)
				{
					::SendMessage(m_hWndParent, WM_SEND_PATH, (WPARAM) csFirstFile.GetString(), m_nID);
				}

			// verify that we have a file (if we want files)
			} 
			else if ((buf.st_mode & _S_IFREG) == _S_IFREG) 
			{
				if (!m_bUseDir)
				{
					::SendMessage(m_hWndParent, WM_SEND_PATH, (WPARAM)csFirstFile.GetString(), m_nID);
				}
			}
		}
	}
}

//////////////////////////////////////////////////////////////////
//	use IShellLink to expand the shortcut
//	returns the expanded file, or "" on error
//
//	original code was part of CShortcut 
//	1996 by Rob Warner
//	rhwarner@southeast.net
//	http://users.southeast.net/~rhwarner

CString CDropEdit::ExpandShortcut(CString &inFile)
{
	CString outFile;

    // Make sure we have a path
    ASSERT(inFile != _T(""));

    IShellLink* psl;
    HRESULT hres;
    LPTSTR lpsz = inFile.GetBuffer(MAX_PATH);

    // Create instance for shell link
    hres = ::CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, IID_IShellLink, (LPVOID*) &psl);

    if (SUCCEEDED(hres))
    {
        // Get a pointer to the persist file interface
        IPersistFile* ppf;

        hres = psl->QueryInterface(IID_IPersistFile, (LPVOID*) &ppf);
        if (SUCCEEDED(hres))
        {
            // Make sure it's ANSI
            //WORD wsz[MAX_PATH];
            //::MultiByteToWideChar(CP_ACP, 0, lpsz, -1, wsz, MAX_PATH);

            // Load shortcut
            hres = ppf->Load(lpsz, STGM_READ);

            if (SUCCEEDED(hres)) 
			{
				WIN32_FIND_DATA wfd;

				// find the path from that
				HRESULT hres = psl->GetPath(outFile.GetBuffer(MAX_PATH), 
								MAX_PATH,
								&wfd, 
								SLGP_UNCPRIORITY);

				outFile.ReleaseBuffer();
            }

            ppf->Release();
        }

        psl->Release();
    }

	inFile.ReleaseBuffer();

	// if this fails, outFile == ""
    return outFile;
}

/////////////////////////////////////////////////////////////////////////////

void CDropEdit::PreSubclassWindow() 
{
	DragAcceptFiles(TRUE);
	
	CEdit::PreSubclassWindow();
}
