#pragma once

#include "CoreMinimal.h"
#include "Interface/ProxyInterface.h"
#include "Containers/Queue.h"
#include "Core/ThreadCoreMacro.h"
#include "Runnable/ThreadRunnableProxy.h"
#include "Abandonable/SimpleAbandonable.h"

class IThreadContainer
{
public:
	IThreadContainer(){}
	~IThreadContainer(){}
protected:
	FCriticalSection Mutex;
};

class IThreadProxyContainer : public TArray<TSharedPtr<IThreadProxy>>, public IThreadContainer
{

protected:
	typedef typename TArray<TSharedPtr<IThreadProxy>> TProxyArray;
public:
	
	FThreadHandle operator<<(const TSharedPtr<IThreadProxy>& Proxy)
	{
		MUTEX_LOCL;

		Proxy->CreateSafeThread();
		this->Add(Proxy);
		return Proxy->GetThreadHandle();
	}

	FThreadHandle operator>>(const FSimpleDelegate& Delegate)
	{
		FThreadHandle Handle = nullptr;
		{
			MUTEX_LOCL;
			for (auto& Proxy : *this)
			{
				if (Proxy->IsSuspend() && !Proxy->GetThreadDelegate().IsBound())
				{
					Proxy->GetThreadDelegate() = Delegate;
					Handle = Proxy->GetThreadHandle();
					break;
				}
			}
		}
		if (!Handle.IsValid())
		{
			TSharedPtr<FThreadRunnable> Proxy = MakeShareable(new FThreadRunnable(true));
			Proxy->GetThreadDelegate() = Delegate;
			Handle = *this << Proxy;
		}
		return Handle;
	}

	FThreadHandle operator<<(const FSimpleDelegate& Delegate)
	{
		FThreadHandle Handle = nullptr;
		{
			MUTEX_LOCL;
			for (auto& Proxy : *this)
			{
				if (Proxy->IsSuspend() && !Proxy->GetThreadDelegate().IsBound())
				{
					Proxy->GetThreadDelegate() = Delegate;
					Proxy->WakeupThread();
					Handle = Proxy->GetThreadHandle();
					break;
				}
			}
		}
		if (!Handle.IsValid())
		{
			TSharedPtr<FThreadRunnable> Proxy = MakeShareable(new FThreadRunnable);
			Proxy->GetThreadDelegate() = Delegate;
			Handle = *this << Proxy;
		}
		return Handle;
	}

	TSharedPtr<IThreadProxy> operator>>(const FThreadHandle& Handle)
	{
		MUTEX_LOCL;
		for (auto& Tmp : *this)
		{
			if (Tmp->GetThreadHandle() == Handle)
			{
				return Tmp;
			}
		}
		return nullptr;
	}
};

class IThreadTaskContainer : public TQueue<FSimpleDelegate>, public TArray<TSharedPtr<IThreadProxy>>, public IThreadContainer
{

protected:
	typedef typename TArray<TSharedPtr<IThreadProxy>> TProxyArray;
	typedef typename TQueue<FSimpleDelegate> TTaskQueue;
public:

	bool operator<<(const FSimpleDelegate & Delegate)
	{
		MUTEX_LOCL;
		this->Enqueue(Delegate);
	}

	bool operator<<=(FSimpleDelegate& Delegate)
	{
		MUTEX_LOCL;
		return this->Dequeue(Delegate);
	}

	IThreadTaskContainer& operator<<(const TSharedPtr<IThreadProxy> & ThreadProxy)
	{
		MUTEX_LOCL;
		this->Add(ThreadProxy);
		return *this;
	}

	IThreadTaskContainer& operator>>(const FSimpleDelegate & Delegate)
	{
		bool bSuccessful = false;
		{
			MUTEX_LOCL;
			for (auto& Tmp : *this)
			{
				if (Tmp->IsSuspend() && !Tmp->GetThreadDelegate().IsBound())
				{
					Tmp->GetThreadDelegate() = Delegate;
					Tmp->WakeupThread();
					bSuccessful = true;
					break;
				}
			}
		}
		if (!bSuccessful)
		{
			*this << Delegate;
		}
		return *this;
	}
};

class IThreadAbandonableContainer :  public IThreadContainer
{
protected:
	//同步操作;
	void operator<<(const FSimpleDelegate& Proxy)
	{
		FAsyncTask<FSimpleAbandonable> *SimpleAbandonable = new FAsyncTask<FSimpleAbandonable>(Proxy);
		SimpleAbandonable->StartBackgroundTask();
		SimpleAbandonable->EnsureCompletion();
		delete SimpleAbandonable;
	}
	//异步任务;
	void operator>>(const FSimpleDelegate& Delegate)
	{
		(new FAutoDeleteAsyncTask<FSimpleAbandonable>(Delegate))->StartBackgroundTask();
	}
};