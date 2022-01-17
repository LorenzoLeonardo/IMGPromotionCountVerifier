#include "pch.h"
#include "CTimeManipulation.h"

void CTimeManipulation::AddDays(LPSYSTEMTIME lpTime, WORD wDays)
{
	lpTime;
	WORD wDaysRemainingThisMonth = 0x0000;

	//We add 1 since the first date the sale or recruit submitted is counted as day 1
	wDaysRemainingThisMonth = (GetNumDays(lpTime->wYear, lpTime->wMonth) - lpTime->wDay) + 1;

	for (int i = 0; i < 13; i++)
	{
		lpTime->wMonth++;
		if (lpTime->wMonth > 12)
		{
			lpTime->wMonth = JANUARY;
			lpTime->wYear++;
		}
		if (wDaysRemainingThisMonth >= wDays)
		{
			wDaysRemainingThisMonth = wDaysRemainingThisMonth - wDays;
			lpTime->wMonth--;
			if (lpTime->wMonth == 0x0000)
			{
				lpTime->wMonth = DECEMBER;
				lpTime->wYear--;
			}
			wDaysRemainingThisMonth = GetNumDays(lpTime->wYear, lpTime->wMonth) - wDaysRemainingThisMonth;
			lpTime->wDay = wDaysRemainingThisMonth;
			break;
		}
		wDaysRemainingThisMonth += GetNumDays(lpTime->wYear, lpTime->wMonth);
	}
}

WORD CTimeManipulation::GetNumDays(WORD wYear, WORD wMonth)
{
	WORD wDaysInAMonth = 0x0000;

	switch (wMonth)
	{
		case JANUARY:
		{
			wDaysInAMonth = DAY31;
			break;
		}
		case FEBRUARY:
		{
			if ((wYear % 4) == 0)
				wDaysInAMonth = DAY29;
			else
				wDaysInAMonth = DAY28;
			break;
		}
		case MARCH:
		{
			wDaysInAMonth = DAY31;
			break;
		}
		case APRIL:
		{
			wDaysInAMonth = DAY30;
			break;
		}
		case MAY:
		{
			wDaysInAMonth = DAY31;
			break;
		}
		case JUNE:
		{
			wDaysInAMonth = DAY30;
			break;
		}
		case JULY:
		{
			wDaysInAMonth = DAY31;
			break;
		}
		case AUGUST:
		{
			wDaysInAMonth = DAY31;
			break;
		}
		case SEPTEMBER:
		{
			wDaysInAMonth = DAY30;
			break;
		}
		case OCTOBER:
		{
			wDaysInAMonth = DAY31;
			break;
		}
		case NOVEMBER:
		{
			wDaysInAMonth = DAY30;
			break;
		}
		case DECEMBER:
		{
			wDaysInAMonth = DAY31;
			break;
		}
	}
	return wDaysInAMonth;
}

CString CTimeManipulation::TimeToCString(SYSTEMTIME sysTime)
{
	CString csTime = _T("");

	csTime.Format(_T("%02d/%02d/%04d"), sysTime.wMonth, sysTime.wDay, sysTime.wYear);

	return csTime;
}

void CTimeManipulation::CStringToTime(CString csTime, LPSYSTEMTIME sysTime)
{
	sysTime->wYear = _ttoi(csTime.Mid(csTime.ReverseFind(_T('/')) + 1));
	sysTime->wMonth = _ttoi(csTime.Left(csTime.Find(_T("/"))));

	csTime = csTime.Mid(csTime.Find(_T("/")) + 1);
	sysTime->wDay = _ttoi(csTime.Left(csTime.Find(_T("/"))));
}

ULONG CTimeManipulation::CalcDayNumFromDate(UINT y, UINT m, UINT d)
{
	m = (m + 9) % 12;
	y -= m / 10;
	ULONG dn = 365 * y + y / 4 - y / 100 + y / 400 + (m * 306 + 5) / 10 + (d - 1);

	return dn;
}

// ----------------------------------------------------------------------
// Given year, month, day, return the day of week (string).
// ----------------------------------------------------------------------
CString CTimeManipulation::CalcDayOfWeek(WORD y, WORD m, WORD d)
{
	CString day[] = {
	  _T("Wednesday"),
	   _T("Thursday"),
	   _T("Friday"),
	   _T("Saturday"),
	   _T("Sunday"),
	   _T("Monday"),
	   _T("Tuesday")
	};

	ULONG dn = CalcDayNumFromDate(y, m, d);

	return day[dn % 7];
}

CString CTimeManipulation::GetMonthName(WORD wMonth)
{
	CString day[] = {
		_T("Error"),
		_T("January"),
		_T("February"),
		_T("March"),
		_T("April"),
		_T("May"),
		_T("June"),
		_T("July"),
		_T("August"),
		_T("September"),
		_T("October"),
		_T("November"),
		_T("December")
	};

	return day[wMonth];
}