#include "Runnable/ThreadProxy.h"
#include "HAL/RunnableThread.h"
#include "HAL/PlatformProcess.h"


int32 FThreadBoost::ThreadCount = 0;
FThreadBoost::FThreadBoost(bool IsSuspend /*= true*/)
	:bRun(false)
	,bSuspend(false)
{

}

FThreadBoost::~FThreadBoost()
{

}

void FThreadBoost::CreateSafeThread()
{
	if (Init())
	{
		std::thread TmpThread(&FThreadBoost::Run, this);
		RunnableThread = std::move(TmpThread);
	}
}

void FThreadBoost::WakeupThread()
{
	bSuspend = false;
	WeakupCV.notify_one();
}

bool FThreadBoost::IsSuspend()
{
	return bSuspend;
}

void FThreadBoost::WaitAndCompleted()
{

}

void FThreadBoost::BlockingAndCompletion()
{
	
}


uint32 FThreadBoost::Run()
{
	while (bRun)
	{
		if (bSuspend)
		{
			std::unique_lock<std::mutex> lk(mutex);
			WeakupLK = std::move(lk);
			WeakupCV.wait(WeakupLK);
		}

		if (ThreadDelegate.IsBound())
		{
			ThreadDelegate.Execute();
			ThreadDelegate.Unbind();
		}
		bSuspend = true;
	}
	return 0;
}

bool FThreadBoost::Init()
{
	if (!FPlatformProcess::SupportsMultithreading())
	{
		bRun = false;
	}
	else
	{
		bRun = true;
	}
	return bRun;
}

