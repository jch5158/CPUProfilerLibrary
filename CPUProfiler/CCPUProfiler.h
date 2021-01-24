#pragma once



class CCPUProfiler
{
public:

	static CCPUProfiler* GetInstance(HANDLE processHandle = INVALID_HANDLE_VALUE)
	{
		static CCPUProfiler cpuProfiler(processHandle);

		return &cpuProfiler;
	}


	FLOAT GetProcessorTotalPercentage(void)
	{
		return mProcessorTotalPercentage;
	}

	FLOAT GetProcessorKernelPercentage(void)
	{
		return mProcessorKernelPercentage;
	}

	FLOAT GetProcessorUserPercentage(void)
	{
		return mProcessorUserPercentage;
	}

	FLOAT GetProcessTotalPercentage(void)
	{
		return mProcessTotalPercentage;
	}

	FLOAT GetProcessKernelPercentage(void)
	{
		return mProcessKernelPercentage;
	}

	FLOAT GetProcessUserPercentage(void)
	{
		return mProcessUserPercentage;
	}


	void UpdateProcessorsProfile(void)
	{
		ULARGE_INTEGER idleTime   = { 0, };
		ULARGE_INTEGER kernelTime = { 0, };
		ULARGE_INTEGER userTime   = { 0, };


		if (GetSystemTimes((PFILETIME)&idleTime, (PFILETIME)&kernelTime, (PFILETIME)&userTime) == false)
		{
			return;
		}

		UINT64 idleDeltaTime   = idleTime.QuadPart - mProcessorLastIdleTime.QuadPart;
		UINT64 kernelDeltaTime = kernelTime.QuadPart - mProcessorLastKernelTime.QuadPart;
		UINT64 userDeltaTime   = userTime.QuadPart - mProcessorLastUserTime.QuadPart;

		UINT64 totalDeltaTime = kernelDeltaTime + userDeltaTime;

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

		mProcessorLastIdleTime   = idleTime;
		mProcessorLastKernelTime = kernelTime;
		mProcessorLastUserTime   = userTime;
	}


	void UpdateProcessProfile(void)
	{
		ULARGE_INTEGER noneUse = { 0, };
		ULARGE_INTEGER nowTime = { 0, };

		ULARGE_INTEGER kernelTime = { 0, };
		ULARGE_INTEGER userTime = { 0, };

		GetSystemTimeAsFileTime((LPFILETIME)&nowTime);

		GetProcessTimes(mProcessHandle, (LPFILETIME)&noneUse, (LPFILETIME)&noneUse, (LPFILETIME)&kernelTime, (LPFILETIME)&userTime);

		UINT64 deltaTime       = nowTime.QuadPart - mProcessLastTime.QuadPart;
		UINT64 kernelDeltaTime = kernelTime.QuadPart - mProcessLastKernelTime.QuadPart;
		UINT64 userDeltaTime   = userTime.QuadPart - mProcessLastUserTime.QuadPart;

		UINT64 totalDeltaTime = kernelDeltaTime + userDeltaTime;

		mProcessTotalPercentage = (FLOAT)((DOUBLE)totalDeltaTime / (DOUBLE)mNumberOfProcessors / (DOUBLE)deltaTime * 100.0f);
		mProcessKernelPercentage = (FLOAT)((DOUBLE)kernelDeltaTime / (DOUBLE)mNumberOfProcessors / (DOUBLE)deltaTime * 100.0f);
		mProcessUserPercentage = (FLOAT)((DOUBLE)userDeltaTime / (DOUBLE)mNumberOfProcessors / (DOUBLE)deltaTime * 100.0f);

		mProcessLastTime = nowTime;
		mProcessLastKernelTime = kernelTime;
		mProcessLastUserTime = userTime;
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

