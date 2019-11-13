#include "Runnable/ThreadRunnableProxy.h"

#include "GenericPlatform/GenericPlatformAffinity.h"
#include "HAL/RunnableThread.h"

int32 FThreadRunnable::ThreadCount = 0;

FThreadRunnable::FThreadRunnable(bool IsSuspend)
	:IThreadProxy()
	, bRun(false)
	, bSuspendAtFirst(IsSuspend)
	, Thread(nullptr)
{

}

FThreadRunnable::~FThreadRunnable()
{
	//释放对象
	if (Thread != nullptr)
	{
		delete Thread;
		Thread = NULL;
	}
}

void FThreadRunnable::CreateSafeThread()
{
	RunnableName = FString::Printf(TEXT("SimpleThread-%d"),ThreadCount);
	Thread = FRunnableThread::Create(this, *RunnableName, 0, TPri_BelowNormal);
	ThreadCount++;
}

void FThreadRunnable::WakeupThread()
{
	ThreadEvent.Trigger();
}

bool FThreadRunnable::IsSuspend()
{
	return ThreadEvent.IsWait();
}

void FThreadRunnable::WaitAndCompleted()
{
	bRun = false;
	ThreadEvent.Trigger();
	StartUpEvent.Wait();
	FPlatformProcess::Sleep(0.03);
}

void FThreadRunnable::BlockingAndCompletion()
{
	ThreadEvent.Trigger();
	WaitExitEvent.Wait();
}

uint32 FThreadRunnable::Run()
{
	while (bRun)
	{
		if (bSuspendAtFirst)
		{
			ThreadEvent.Wait();
		}

		if (ThreadDelegate.IsBound())
		{
			ThreadDelegate.ExecuteIfBound();
			ThreadDelegate.Unbind();
		}

		// 同步唤醒主线程
		WaitExitEvent.Trigger();
		bSuspendAtFirst = false;
	}
	

	return 0;
}

bool FThreadRunnable::Init()
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

void FThreadRunnable::Stop()
{

}

void FThreadRunnable::Exit()
{
	//激活主线程
	StartUpEvent.Trigger();
}

