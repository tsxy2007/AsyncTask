#pragma once

#include "CoreMinimal.h"
#include "Interface/ProxyInterface.h"
#include "Interface/ThreadManageInterface.h"

class ASYNCTASK_API FThreadProxyManagement : public IThreadProxyContainer
{
public:
	~FThreadProxyManagement();
	
	//同步
	bool Join(FThreadHandle Handle);

	//异步
	bool Detach(FThreadHandle Handle);

	EThreadState Joinable(FThreadHandle Handle);

	//绑定单步执行，通过Join detach 执行
public:
	
	template<class UserClass, typename... VarTypes>
	inline FThreadHandle BindRaw(UserClass* TargetClass, typename TMemFunPtrType<false, UserClass, void(VarTypes...)>::Type InMethod, VarTypes... Vars)
	{
		return *this << FSimpleDelegate::CreateRaw(TargetClass, InMethod, Vars...);
	}

	template<class UserClass, typename... VarTypes>
	inline FThreadHandle BindSP(const TSharedRef<UserClass>& TargetClass, typename TMemFunPtrType<false, UserClass, void(VarTypes...)>::Type InMethod, VarTypes... Vars)
	{
		return *this << FSimpleDelegate::CreateSP(TargetClass, InMethod, Vars...);
	}

	template <typename UObjectTemplate, typename... VarTypes>
	inline FThreadHandle BindUFunction(UObjectTemplate* InUserObject, const FName& InFunctionName, VarTypes... Vars)
	{
		return *this << FSimpleDelegate::CreateUFunction(InUserObject, InFunctionName, Vars...);
	}

	template<class UserClass, typename... VarTypes>
	inline FThreadHandle BindUObject(UserClass* TargetClass, typename TMemFunPtrType<false, UserClass, void(VarTypes...)>::Type InMethod, VarTypes... Vars)
	{
		return *this << FSimpleDelegate::CreateUFunction(InUserObject, InFunctionName, Vars...);
	}

	template < typename FuncType, typename... VarTypes>
	inline FThreadHandle BindStatic(FuncType InFunc, VarTypes... Vars)
	{
		return *this << FSimpleDelegate::CreateStatic(InFunc, Vars...);
	}

	template <typename FuncType, typename... VarTypes>
	inline FThreadHandle BindLambda(FuncType&& InFunc, VarTypes... Vars)
	{
		return *this << FSimpleDelegate::CreateLambda(InFunc, Vars...);
	}
	//创建之后无需等待直接执行，属于异步操作
public:
	template<class UserClass, typename... VarTypes>
	inline FThreadHandle CreateRaw(UserClass* TargetClass, typename TMemFunPtrType<false, UserClass, void(VarTypes...)>::Type InMethod, VarTypes... Vars)
	{
		return *this^ FSimpleDelegate::CreateRaw(TargetClass, InMethod, Vars...);
	}
	
	template<class UserClass, typename... VarTypes>
	inline FThreadHandle CreateSP(const TSharedRef<UserClass>& TargetClass, typename TMemFunPtrType<false, UserClass, void(VarTypes...)>::Type InMethod, VarTypes... Vars)
	{
		return *this ^ FSimpleDelegate::BindSP(TargetClass, InMethod, Vars...);
	}

	template <typename UObjectTemplate, typename... VarTypes>
	inline FThreadHandle CreateUFunction(UObjectTemplate* InUserObject, const FName& InFunctionName, VarTypes... Vars)
	{
		return *this ^ FSimpleDelegate::BindUFunction(InUserObject, InFunctionName, Vars...);
	}

	template<class UserClass, typename... VarTypes>
	inline FThreadHandle CreateUObject(UserClass* TargetClass, typename TMemFunPtrType<false, UserClass, void(VarTypes...)>::Type InMethod, VarTypes... Vars)
	{
		return *this ^ FSimpleDelegate::BindUFunction(InUserObject, InFunctionName, Vars...);
	}

	template < typename FuncType, typename... VarTypes>
	inline FThreadHandle CreateStatic(FuncType InFunc, VarTypes... Vars)
	{
		return *this ^ FSimpleDelegate::BindStatic(InFunc, Vars...);
	}

	template <typename FuncType, typename... VarTypes>
	inline FThreadHandle CreateLambda(FuncType&& InFunc, VarTypes... Vars)
	{
		return *this ^ FSimpleDelegate::BindLambda(InFunc, Vars...);
	}
};

FThreadProxyManagement::~FThreadProxyManagement()
{
	for (auto& Tmp : *this )
	{
		Tmp->WaitAndCompleted();
	}
	// 自己销毁不用自己调用Empty
}

bool FThreadProxyManagement::Join(FThreadHandle Handle)
{
	TSharedPtr<IThreadProxy> Proxy = *this >> Handle;
	if (Proxy.IsValid())
	{
		Proxy->BlockingAndCompletion();
		return true;
	}
	return false;
}

bool FThreadProxyManagement::Detach(FThreadHandle Handle)
{
	TSharedPtr<IThreadProxy> Proxy = *this >> Handle;
	if (Proxy.IsValid())
	{
		Proxy->WakeupThread();
		return true;
	}
	return false;
}

EThreadState FThreadProxyManagement::Joinable(FThreadHandle Handle)
{
	TSharedPtr<IThreadProxy> Proxy = *this >> Handle;
	if (Proxy.IsValid())
	{
		if (Proxy->IsSuspend())
		{
			return EThreadState::LEISURELY;
		}
		else
		{
			return EThreadState::WORKING;
		}
	}
	return EThreadState::ERROR;
}
