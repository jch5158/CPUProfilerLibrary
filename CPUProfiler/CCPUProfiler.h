#pragma once

#include <iostream>
#include <Windows.h>


class CCPUProfiler
{
public:

	static inline CCPUProfiler* GetInstance(HANDLE processHandle = INVALID_HANDLE_VALUE)
	{
		static CCPUProfiler cpuProfiler(processHandle);

		return &cpuProfiler;
	}


	inline FLOAT GetProcessorTotalPercentage(void)
	{
		return mProcessorTotalPercentage;
	}

	inline FLOAT GetProcessorKernelPercentage(void)
	{
		return mProcessorKernelPercentage;
	}

	inline FLOAT GetProcessorUserPercentage(void)
	{
		return mProcessorUserPercentage;
	}

	inline FLOAT GetProcessTotalPercentage(void)
	{
		return mProcessTotalPercentage;
	}

	inline FLOAT GetProcessKernelPercentage(void)
	{
		return mProcessKernelPercentage;
	}

	inline FLOAT GetProcessUserPercentage(void)
	{
		return mProcessUserPercentage;
	}


	inline void UpdateProcessorsProfile(void)
	{
		ULARGE_INTEGER idleTime   = { 0, };
		ULARGE_INTEGER kernelTime = { 0, };
		ULARGE_INTEGER userTime   = { 0, };


		if (GetSystemTimes((PFILETIME)&idleTime, (PFILETIME)&kernelTime, (PFILETIME)&userTime) == false)
		{
			return;
		}

		LONGLONG idleDeltaTime   = idleTime.QuadPart - mProcessorLastIdleTime.QuadPart;
		LONGLONG kernelDeltaTime = kernelTime.QuadPart - mProcessorLastKernelTime.QuadPart;
		LONGLONG userDeltaTime   = userTime.QuadPart - mProcessorLastUserTime.QuadPart;

		LONGLONG totalDeltaTime  = kernelDeltaTime + userDeltaTime;

		if (totalDeltaTime == 0)
		{
			mProcessorTotalPercentage  = 0.0f;
			mProcessorKernelPercentage = 0.0f;
			mProcessorUserPercentage   = 0.0f;
		}
		else
		{
			mProcessorTotalPercentage  = (FLOAT)((DOUBLE)(totalDeltaTime - idleDeltaTime) / (DOUBLE)totalDeltaTime * 100.0f);
			mProcessorKernelPercentage = (FLOAT)((DOUBLE)(kernelDeltaTime - idleDeltaTime) / (DOUBLE)totalDeltaTime * 100.0f);
			mProcessorUserPercentage   = (FLOAT)((DOUBLE)(userDeltaTime) / (DOUBLE)totalDeltaTime * 100.0f);
		}

		mProcessorLastIdleTime		   = idleTime;
		mProcessorLastKernelTime	   = kernelTime;
		mProcessorLastUserTime		   = userTime;
	}


	inline void UpdateProcessProfile(void)
	{
		ULARGE_INTEGER noneUse    = { 0, };
		ULARGE_INTEGER nowTime    = { 0, };

		ULARGE_INTEGER kernelTime = { 0, };
		ULARGE_INTEGER userTime   = { 0, };


		// 현재 시간을 100나노 세컨드 해상도를 가진
		GetSystemTimeAsFileTime((LPFILETIME)&nowTime);

		GetProcessTimes(mProcessHandle, (LPFILETIME)&noneUse, (LPFILETIME)&noneUse, (LPFILETIME)&kernelTime, (LPFILETIME)&userTime);
		
		// 지난시간에서 얼마만큼의 시간이 지났는지 확인한다.
		LONGLONG deltaTime		  = nowTime.QuadPart - mProcessLastTime.QuadPart;

		LONGLONG kernelDeltaTime	  = kernelTime.QuadPart - mProcessLastKernelTime.QuadPart;
		LONGLONG userDeltaTime	  = userTime.QuadPart - mProcessLastUserTime.QuadPart;
								  
		LONGLONG processDeltaTime   = kernelDeltaTime + userDeltaTime;
								  
								  
		mProcessTotalPercentage	  = (FLOAT)((DOUBLE)processDeltaTime / (DOUBLE)mNumberOfProcessors / (DOUBLE)deltaTime * 100.0f);
		mProcessKernelPercentage  = (FLOAT)((DOUBLE)kernelDeltaTime / (DOUBLE)mNumberOfProcessors / (DOUBLE)deltaTime * 100.0f);	
		mProcessUserPercentage	  = (FLOAT)((DOUBLE)userDeltaTime / (DOUBLE)mNumberOfProcessors / (DOUBLE)deltaTime * 100.0f);
								  
		mProcessLastTime		  = nowTime;
		mProcessLastKernelTime	  = kernelTime;
		mProcessLastUserTime	  = userTime;
	}


private:

	CCPUProfiler(HANDLE processHandle)
		: mProcessHandle(INVALID_HANDLE_VALUE)
		, mNumberOfProcessors(0)

		, mProcessorTotalPercentage(0.0f)
		, mProcessorKernelPercentage(0.0f)		
		, mProcessorUserPercentage(0.0f)
		
		, mProcessTotalPercentage(0.0f)
		, mProcessKernelPercentage(0.0f)
		, mProcessUserPercentage(0.0f)

		, mProcessorLastIdleTime{ 0, }
		, mProcessorLastKernelTime{ 0, }
		, mProcessorLastUserTime{ 0, }

		, mProcessLastTime{ 0, }
		, mProcessLastKernelTime{ 0, }
		, mProcessLastUserTime{ 0, }

	{
		if (processHandle == INVALID_HANDLE_VALUE)
		{
			mProcessHandle = GetCurrentProcess();
		}
		else
		{
			mProcessHandle = processHandle;
		}

		SYSTEM_INFO systemInfo = { 0, };

		GetSystemInfo(&systemInfo);
		
		// 현재 PC의 프로세서의 개수를 구한다.
		mNumberOfProcessors = systemInfo.dwNumberOfProcessors;


		// 생성자에서 한 번 호출해준다.
		UpdateProcessorsProfile();

		UpdateProcessProfile();
	}


	~CCPUProfiler()
	{
	
	}


	HANDLE mProcessHandle;

	DWORD mNumberOfProcessors;

	FLOAT mProcessorTotalPercentage;
	FLOAT mProcessorKernelPercentage;
	FLOAT mProcessorUserPercentage;
	
	FLOAT mProcessTotalPercentage;
	FLOAT mProcessKernelPercentage;
	FLOAT mProcessUserPercentage;
	
	ULARGE_INTEGER mProcessorLastIdleTime;
	ULARGE_INTEGER mProcessorLastKernelTime;
	ULARGE_INTEGER mProcessorLastUserTime;

	ULARGE_INTEGER mProcessLastTime;
	ULARGE_INTEGER mProcessLastKernelTime;
	ULARGE_INTEGER mProcessLastUserTime;
};

