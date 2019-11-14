#pragma once

#include "CoreMinimal.h"

// << 同步操作
// >> 异步操作

template<class Task,typename ReturnType = void>
class FThreadTemplate : public Task
{
public:

	template<class UserClass, typename... VarTypes>
	inline void BindRaw(UserClass* TargetClass, typename TMemFunPtrType<false, UserClass, void(VarTypes...)>::Type InMethod, VarTypes... Vars)
	{
		return *this << FSimpleDelegate::CreateRaw(TargetClass, InMethod, Vars...);
	}

	template<class UserClass, typename... VarTypes>
	inline void BindSP(const TSharedRef<UserClass>& TargetClass, typename TMemFunPtrType<false, UserClass, void(VarTypes...)>::Type InMethod, VarTypes... Vars)
	{
		return *this << FSimpleDelegate::CreateSP(TargetClass, InMethod, Vars...);
	}

	template <typename UObjectTemplate, typename... VarTypes>
	inline void BindUFunction(UObjectTemplate* InUserObject, const FName& InFunctionName, VarTypes... Vars)
	{
		return *this << FSimpleDelegate::CreateUFunction(InUserObject, InFunctionName, Vars...);
	}

	template<class UserClass, typename... VarTypes>
	inline void BindUObject(UserClass* TargetClass, typename TMemFunPtrType<false, UserClass, void(VarTypes...)>::Type InMethod, VarTypes... Vars)
	{
		return *this << FSimpleDelegate::CreateUFunction(InUserObject, InFunctionName, Vars...);
	}

	template < typename FuncType, typename... VarTypes>
	inline void BindStatic(FuncType InFunc, VarTypes... Vars)
	{
		return *this << FSimpleDelegate::CreateStatic(InFunc, Vars...);
	}

	template <typename FuncType, typename... VarTypes>
	inline void BindLambda(FuncType&& InFunc, VarTypes... Vars)
	{
		return *this << FSimpleDelegate::CreateLambda(InFunc, Vars...);
	}
	//创建之后无需等待直接执行，属于异步操作
public:
	template<class UserClass, typename... VarTypes>
	inline void CreateRaw(UserClass* TargetClass, typename TMemFunPtrType<false, UserClass, void(VarTypes...)>::Type InMethod, VarTypes... Vars)
	{
		return *this^ FSimpleDelegate::CreateRaw(TargetClass, InMethod, Vars...);
	}

	template<class UserClass, typename... VarTypes>
	inline void CreateSP(const TSharedRef<UserClass>& TargetClass, typename TMemFunPtrType<false, UserClass, void(VarTypes...)>::Type InMethod, VarTypes... Vars)
	{
		return *this ^ FSimpleDelegate::CreateSP(TargetClass, InMethod, Vars...);
	}

	template <typename UObjectTemplate, typename... VarTypes>
	inline void CreateUFunction(UObjectTemplate* InUserObject, const FName& InFunctionName, VarTypes... Vars)
	{
		return *this ^ FSimpleDelegate::CreateUFunction(InUserObject, InFunctionName, Vars...);
	}

	template<class UserClass, typename... VarTypes>
	inline void CreateUObject(UserClass* TargetClass, typename TMemFunPtrType<false, UserClass, void(VarTypes...)>::Type InMethod, VarTypes... Vars)
	{
		return *this ^ FSimpleDelegate::CreateUFunction(InUserObject, InFunctionName, Vars...);
	}

	template < typename FuncType, typename... VarTypes>
	inline void CreateStatic(FuncType InFunc, VarTypes... Vars)
	{
		return *this ^ FSimpleDelegate::BindStatic(InFunc, Vars...);
	}

	template <typename FuncType, typename... VarTypes>
	inline void CreateLambda(FuncType&& InFunc, VarTypes... Vars)
	{
		return *this ^ FSimpleDelegate::CreateLambda(InFunc, Vars...);
	}
};