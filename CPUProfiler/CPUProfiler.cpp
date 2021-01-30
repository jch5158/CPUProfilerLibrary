
#pragma comment(lib,"Winmm.lib");

#include <locale>
#include "CCPUProfiler.h"


int wmain()
{
	_wsetlocale(LC_ALL, L"");

	CCPUProfiler::GetInstance();

	for(;;)
	{
		static DWORD time = timeGetTime();

		if (timeGetTime() - time >= 1000)
		{
			CCPUProfiler::GetInstance()->UpdateProcessProfile();

			wprintf_s(L"Total  : %3.1lf\n\n", CCPUProfiler::GetInstance()->GetProcessTotalPercentage());


			wprintf_s(L"Kernel  : %3.1lf\n\n", CCPUProfiler::GetInstance()->GetProcessKernelPercentage());


			wprintf_s(L"User  : %3.1lf\n\n", CCPUProfiler::GetInstance()->GetProcessUserPercentage());

			//Sleep(1000);

			time = timeGetTime();
		}		
	}

	//wprintf_s(L"%lf", ((float)num2 / (float)num));
}
