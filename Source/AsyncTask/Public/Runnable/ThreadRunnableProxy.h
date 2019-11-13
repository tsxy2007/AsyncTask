#pragma  once 
#include "CoreMinimal.h"
#include "HAL/Runnable.h"
#include "HAL/CriticalSection.h"
#include "Core/SimpleThreadType.h"
#include "Core/SimpleSemaphore.h"
#include "Interface/ProxyInterface.h"

class FRunnableThread;
class FEvent;

class FThreadRunnable : public FRunnable, public IThreadProxy
{
public:
	FThreadRunnable(bool IsSuspend = true);
	virtual ~FThreadRunnable();

	virtual void CreateSafeThread();
	virtual void WakeupThread();
	virtual bool IsSuspend() ;
	virtual void WaitAndCompleted();
	virtual void BlockingAndCompletion();
private:
	virtual uint32 Run();
	virtual bool Init();
	virtual void Stop();
	virtual void Exit();
private:
	bool bRun; //是否运行
	bool bSuspendAtFirst; // 第一次是否挂起
	FRunnableThread* Thread;
	FString RunnableName;
	FSimpleSemaphore  ThreadEvent; //负责挂起，唤醒线程;
	FSimpleSemaphore StartUpEvent; //负责挂起，唤醒线程;
	FSimpleSemaphore WaitExitEvent; //负责挂起，唤醒线程;

	FCriticalSection pMutex;//锁

	static int32 ThreadCount;

};