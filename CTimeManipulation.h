#pragma once

#define JANUARY		0x0001
#define FEBRUARY	0x0002
#define MARCH		0x0003
#define APRIL		0x0004
#define MAY			0x0005
#define JUNE		0x0006
#define JULY		0x0007
#define AUGUST		0x0008
#define SEPTEMBER	0x0009
#define OCTOBER		0x000A
#define NOVEMBER	0x000B
#define DECEMBER	0x000C

#define DAY30		0x001E
#define DAY31		0x001F
#define DAY28		0x001C
#define DAY29		0x001D


class CTimeManipulation
{
public:
	CTimeManipulation(){}
	~CTimeManipulation(){}
	ULONG CalcDayNumFromDate(UINT y, UINT m, UINT d);
	CString CalcDayOfWeek(WORD y, WORD m, WORD d);
	void AddDays(LPSYSTEMTIME lpTime, WORD wDays);
	WORD GetNumDays(WORD wYear, WORD wMonth);
	CString TimeToCString(SYSTEMTIME sysTime);
	CString GetMonthName(WORD wMonth);
	void CStringToTime(CString csTime, LPSYSTEMTIME sysTime);
};

