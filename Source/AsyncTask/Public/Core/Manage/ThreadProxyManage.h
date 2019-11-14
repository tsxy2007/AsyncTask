#pragma once

#include "CoreMinimal.h"
#include "ManageBase/ThreadManageBase.h"
#include "Interface/ProxyInterface.h"
#include "Interface/ThreadManageInterface.h"

class ASYNCTASK_API FThreadProxyManagement : public FThreadTemplate<IThreadProxyContainer, FThreadHandle>
{
public:
	~FThreadProxyManagement();
	
	//同步
	bool Join(FThreadHandle Handle);

	//异步
	bool Detach(FThreadHandle Handle);

	EThreadState Joinable(FThreadHandle Handle);
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
