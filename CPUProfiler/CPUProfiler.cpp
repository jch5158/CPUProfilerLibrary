#include "stdafx.h"

int wmain()
{
	_wsetlocale(LC_ALL, L"");

	CCPUProfiler::GetInstance();

	for(;;)
	{
		static DWORD time = timeGetTime();

		if (timeGetTime() - time == 1000)
		{
			CCPUProfiler::GetInstance()->UpdateProcessProfile();

			wprintf_s(L"Total  : %0.1lf\n\n", CCPUProfiler::GetInstance()->GetProcessTotalPercentage());

			time = timeGetTime();
		}		
	}

	//wprintf_s(L"%lf", ((float)num2 / (float)num));
}
