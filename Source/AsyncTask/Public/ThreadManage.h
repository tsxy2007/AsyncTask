#pragma once
#include "CoreMinimal.h"
#include "Runnable/ThreadRunnableProxy.h"
#include "Containers/Queue.h"
#include <thread>
#include <iostream>
#include "Tickable.h"
#include "Core/Manage/ThreadTaskManage.h"
#include "Core/Manage/ThreadProxyManage.h"


class ASYNCTASK_API FThreadManage : public TSharedFromThis<FThreadManage>
{
public:
	static TSharedRef<FThreadManage> Get();
	static void Destory();

protected:

	FThreadTaskManagement ThreadTaskManagement;
	FThreadProxyManagement ProxyManagement;
	
private:
	static TSharedPtr<FThreadManage> Instance;
};