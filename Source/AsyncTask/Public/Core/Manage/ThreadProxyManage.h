#pragma once

#include "CoreMinimal.h"
#include "Interface/ProxyInterface.h"
#include "Interface/ThreadManageInterface.h"

class ASYNCTASK_API FThreadProxyManagement : public IThreadProxyContainer
{
public:
	~FThreadProxyManagement();
	
	//ͬ��
	bool Join(FThreadHandle Handle);

	//�첽
	bool Detach(FThreadHandle Handle);

	EThreadState Joinable(FThreadHandle Handle);

	//�󶨵���ִ�У�ͨ��Join detach ִ��
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
	//����֮������ȴ�ֱ��ִ�У������첽����
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
	// �Լ����ٲ����Լ�����Empty
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
