#include "Runnable/ThreadBoostProxy.h"
#include "HAL/RunnableThread.h"
#include "HAL/PlatformProcess.h"
#include <thread>
#include <chrono>

FThreadBoost::FThreadBoost(bool IsSuspend /*= true*/)
	:bRun(false)
	,bSuspend(IsSuspend)
	, ThreadEvent(new semaphore(1))
	,StartUpEvent(new semaphore(1))
{

}

FThreadBoost::~FThreadBoost()
{
	delete ThreadEvent;
	delete StartUpEvent;
}

void FThreadBoost::CreateSafeThread()
{
	if (Init())
	{
		std::thread TmpThread(&FThreadBoost::Run, this);
		RunnableThread = std::move(TmpThread);
		RunnableThread.detach();
	}
}

void FThreadBoost::WakeupThread()
{
	if (ThreadEvent->IsWait())
		ThreadEvent->notify();
}

bool FThreadBoost::IsSuspend()
{
	return ThreadEvent->IsWait();
}

void FThreadBoost::WaitAndCompleted()
{
	Stop();
	if (ThreadEvent->IsWait())
	{
		ThreadEvent->notify();
		RunnableThread.join();
	}
}

void FThreadBoost::BlockingAndCompletion()
{
	if (ThreadEvent->IsWait())
	{
		ThreadEvent->notify();
	}
	if (!StartUpEvent->IsWait())
	{
		StartUpEvent->wait();
	}
}


uint32 FThreadBoost::Run()
{
	while (bRun)
	{
		if (!bSuspend)
		{
			ThreadEvent->wait();
		}

		if (ThreadDelegate.IsBound())
		{
			ThreadDelegate.Execute();
			ThreadDelegate.Unbind();
		}
		bSuspend = false;
		if (StartUpEvent->IsWait())
		{
			StartUpEvent->notify();
		}
	}
	Exit();
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

void FThreadBoost::Stop()
{
	bRun = false;
}

void FThreadBoost::Exit()
{
	if (StartUpEvent->IsWait())
	{
		StartUpEvent->notify();
	}
}

