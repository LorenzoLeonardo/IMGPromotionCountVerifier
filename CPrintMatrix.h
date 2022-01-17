#pragma once
#include "CustomHeader.h"
#include <vector>

using namespace std;



typedef struct tagCoordinates
{
	INT x;
	INT y;
}COORDINATES;

typedef struct tagPrintPageMatrix
{
	COORDINATES nStartPos;
	INT nItemsPerPage;
	INT nColSpacing[MAX_COLUMN];
	INT nRowSpacing;
}PRINTPAGEMATRIX;

typedef struct tagDocumentMatrix
{
	COORDINATES nNamePos;
	COORDINATES nCodePos;
	COORDINATES nDatePos;
	COORDINATES nSignaturePos;
	COORDINATES nUplineNamePos;
	COORDINATES nRankPos;
	INT nPageCount;
	vector<PRINTPAGEMATRIX> vPrintPageMatrix;
}DOCUMENTMATRIX;


class CPrintMatrix
{
public:
	CPrintMatrix();
	~CPrintMatrix();

	BOOL LoadINIFile(ENUMPROMOTIONTYPE enumFormType);
	DOCUMENTMATRIX GetDocumentMatrix()
	{
		return m_documentMatrix;
	}
	CString GetCurrentDirectory();

private:
	CString GetFormTypeString(ENUMPROMOTIONTYPE enumFormType);
private:
	DOCUMENTMATRIX m_documentMatrix;
};

