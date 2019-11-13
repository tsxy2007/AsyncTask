#pragma once

#include "CoreMinimal.h"
#include <thread>
#include <mutex>
#include <condition_variable>
#include "Interface/ProxyInterface.h"
#include "Core/BoostSemaphore.h"

class FThreadBoost : public IThreadProxy
{
public:
	FThreadBoost(bool IsSuspend = false);
	virtual ~FThreadBoost();

	virtual void CreateSafeThread();
	virtual void WakeupThread();
	virtual bool IsSuspend();
	virtual void WaitAndCompleted();
	virtual void BlockingAndCompletion();
private:
	uint32 Run();
	bool Init();
	void Stop();
	void Exit();
private:
	bool bRun; //是否运行
	bool bSuspend; // 是否挂起
	std::thread RunnableThread;

	semaphore*					ThreadEvent;
	semaphore*					StartUpEvent;	 //信号量 阻塞启动线程


	std::mutex mutex;

};