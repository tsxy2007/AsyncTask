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
	bool bRun; //�Ƿ�����
	bool bSuspendAtFirst; // ��һ���Ƿ����
	FRunnableThread* Thread;
	FString RunnableName;
	FSimpleSemaphore  ThreadEvent; //������𣬻����߳�;
	FSimpleSemaphore StartUpEvent; //������𣬻����߳�;
	FSimpleSemaphore WaitExitEvent; //������𣬻����߳�;

	FCriticalSection pMutex;//��

	static int32 ThreadCount;

};