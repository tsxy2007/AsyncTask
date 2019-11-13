#pragma once

#include "CoreMinimal.h"
#include <thread>
#include <mutex>
#include <condition_variable>
#include "Interface/ProxyInterface.h"

class FThreadBoost : public IThreadProxy
{
public:
	FThreadBoost(bool IsSuspend = true);
	virtual ~FThreadBoost();

	virtual void CreateSafeThread();
	virtual void WakeupThread();
	virtual bool IsSuspend();
	virtual void WaitAndCompleted();
	virtual void BlockingAndCompletion();
private:
	uint32 Run();
	bool Init();
private:
	bool bRun; //是否运行
	bool bSuspend; // 是否挂起


	std::thread RunnableThread;


	std::condition_variable WeakupCV;
	std::unique_lock<std::mutex> WeakupLK;
	std::mutex mutex;

	static int32 ThreadCount;

};