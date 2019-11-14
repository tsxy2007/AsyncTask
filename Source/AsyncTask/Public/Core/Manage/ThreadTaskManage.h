#pragma once
#include "CoreMinimal.h"
#include "Interface/ThreadManageInterface.h"
#include "Tickable.h"
#include "ManageBase/ThreadManageBase.h"

class ASYNCTASK_API FThreadTaskManagement : public FThreadTemplate<IThreadTaskContainer>, public FTickableGameObject
{
public:
	~FThreadTaskManagement();
	void Init(int32 ThreadNum);
private:
	//tick
	virtual void Tick(float DeltaTime) override;
	virtual TStatId GetStatId() const override;

};

FThreadTaskManagement::~FThreadTaskManagement()
{
	for (auto& Tmp : *this)
	{
		Tmp->WaitAndCompleted();
	}
	//×Ô¶¯Çå³ýÈÝÆ÷£»
}

void FThreadTaskManagement::Init(int32 ThreadNum)
{
	for (int32 i = 0; i < ThreadNum; i++)
	{
		*this << MakeShareable(new FThreadRunnable);
	}
}

void FThreadTaskManagement::Tick(float DeltaTime)
{

	TSharedPtr<IThreadProxy> ThreadProxy = nullptr;
	{
		MUTEX_LOCL; 
		for (auto& Proxy : *this)
		{
			if (Proxy->IsSuspend())
			{
				ThreadProxy = Proxy;
				break;
			}
		}
	}
	
	if (ThreadProxy.IsValid())
	{
		if (!((TTaskQueue*)this)->IsEmpty())
		{
			FSimpleDelegate Delegate;
			if (*this <<= (Delegate))
			{
				ThreadProxy->GetThreadDelegate() = Delegate;
				ThreadProxy->WakeupThread();
			};
		}
	}
}

TStatId FThreadTaskManagement::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(FThreadTaskManagement, STATGROUP_Tickables);
}
