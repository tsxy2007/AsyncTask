#pragma once
#include "CoreMinimal.h"
#include "Core/SimpleThreadType.h"

class ASYNCTASK_API IThreadProxy : public TSharedFromThis<IThreadProxy>
{
public:
	IThreadProxy();
	virtual ~IThreadProxy() {};

	virtual void CreateSafeThread() = 0;
	virtual void WakeupThread() = 0;
	virtual bool IsSuspend() = 0;
	virtual void WaitAndCompleted() = 0;
	virtual void BlockingAndCompletion() = 0;


	FORCEINLINE FSimpleDelegate& GetThreadDelegate() { return ThreadDelegate; }
	FORCEINLINE FThreadHandle GetThreadHandle() { return SimpleThreadHandle; }
protected:
	FSimpleDelegate ThreadDelegate;
private:
	TSharedPtr<FSimpleThreadHandle> SimpleThreadHandle;
};