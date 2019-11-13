#pragma once
#include "CoreMinimal.h"
#include "Interface/ThreadManageInterface.h"
#include "Tickable.h"

class ASYNCTASK_API FThreadTaskManagement : public IThreadTaskContainer, public FTickableGameObject
{
public:
	~FThreadTaskManagement();
	void Init(int32 ThreadNum);
private:
	//tick
	virtual void Tick(float DeltaTime) override;
	virtual TStatId GetStatId() const override;
public:
	template<class UserClass, typename... VarTypes>
	inline void CreateRaw(UserClass* TargetClass, typename TMemFunPtrType<false, UserClass, void(VarTypes...)>::Type InMethod, VarTypes... Vars)
	{
		*this^FSimpleDelegate::CreateRaw(TargetClass, InMethod, Vars...);
	}

	template<class UserClass, typename... VarTypes>
	inline void CreateSP(const TSharedRef<UserClass>& TargetClass, typename TMemFunPtrType<false, UserClass, void(VarTypes...)>::Type InMethod, VarTypes... Vars)
	{
		*this ^ FSimpleDelegate::CreateSP(TargetClass, InMethod, Vars...);
	}

	template <typename UObjectTemplate, typename... VarTypes>
	inline void CreateUFunction(UObjectTemplate* InUserObject, const FName& InFunctionName, VarTypes... Vars)
	{
		 *this ^ FSimpleDelegate::CreateUFunction(InUserObject, InFunctionName, Vars...);
	}

	template<class UserClass, typename... VarTypes>
	inline void CreateUObject(UserClass* TargetClass, typename TMemFunPtrType<false, UserClass, void(VarTypes...)>::Type InMethod, VarTypes... Vars)
	{
		 *this ^ FSimpleDelegate::CreateUFunction(InUserObject, InFunctionName, Vars...);
	}

	template < typename FuncType, typename... VarTypes>
	inline void CreateStatic(FuncType InFunc, VarTypes... Vars)
	{
		 *this ^ FSimpleDelegate::CreateStatic(InFunc, Vars...);
	}

	template <typename FuncType, typename... VarTypes>
	inline void CreateLambda(FuncType&& InFunc, VarTypes... Vars)
	{
		 *this ^ FSimpleDelegate::CreateLambda(InFunc, Vars...);
	}
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
			if (*this >> (Delegate))
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
