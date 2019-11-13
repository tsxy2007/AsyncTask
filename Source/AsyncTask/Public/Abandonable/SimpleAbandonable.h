#pragma once

#include "CoreMinimal.h"
#include "Async/AsyncWork.h"

//ͬ���첽�̣߳����߳�ʹ�����ɱ��
struct ASYNCTASK_API FSimpleAbandonable :FNonAbandonableTask
{
public:
	FSimpleAbandonable(const FSimpleDelegate& InDelegate);
	~FSimpleAbandonable();

	//�߳�����ִ���߼��ĵط�
	void DoWork();

	//ID
	FORCEINLINE TStatId GetStatId() const
	{
		RETURN_QUICK_DECLARE_CYCLE_STAT(FSimpleAbandonable, STATGROUP_ThreadPoolAsyncTasks);
	}
protected:
	FSimpleDelegate Delegate;
};