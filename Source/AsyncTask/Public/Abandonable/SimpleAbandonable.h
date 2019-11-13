#pragma once

#include "CoreMinimal.h"
#include "Async/AsyncWork.h"

//同步异步线程，该线程使用轻巧便捷
struct ASYNCTASK_API FSimpleAbandonable :FNonAbandonableTask
{
public:
	FSimpleAbandonable(const FSimpleDelegate& InDelegate);
	~FSimpleAbandonable();

	//线程真正执行逻辑的地方
	void DoWork();

	//ID
	FORCEINLINE TStatId GetStatId() const
	{
		RETURN_QUICK_DECLARE_CYCLE_STAT(FSimpleAbandonable, STATGROUP_ThreadPoolAsyncTasks);
	}
protected:
	FSimpleDelegate Delegate;
};