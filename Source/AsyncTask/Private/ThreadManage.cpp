#include "ThreadManage.h"

TSharedPtr<FThreadManage> FThreadManage::Instance = nullptr;

TSharedRef<FThreadManage> FThreadManage::Get()
{
	if (Instance.IsValid())
	{
		Instance = MakeShareable(new FThreadManage);
	}
	return Instance.ToSharedRef();
}

void FThreadManage::Destory()
{
	if (Instance.IsValid())
	{
		//Instance->ClearAllThread();
		Instance = nullptr;
	}
}
//
//void FThreadManage::Init(int32 ThreadNum)
//{
//	for (int32 i = 0; i < ThreadNum; i++)
//	{
//		Pool.Add(MakeShareable(new FThreadRunnable));
//	}
//}
//
//void FThreadManage::Tick(float DeltaTime)
//{
//	FScopeLock Lock(&pMutex);
//	TSharedPtr<IThreadProxy> ThreadProxy = nullptr;
//	for (auto& Proxy : Pool)
//	{
//		if (Proxy->IsSuspend())
//		{
//			ThreadProxy = Proxy;
//			break;
//		}
//	}
//	if (ThreadProxy.IsValid())
//	{
//		if (!TaskQueue.IsEmpty())
//		{
//			FSimpleDelegate Delegate;
//			TaskQueue.Dequeue(Delegate);
//			ThreadProxy->GetThreadDelegate() = Delegate;
//			ThreadProxy->WakeupThread();
//		}
//	}
//}
//
//TStatId FThreadManage::GetStatId() const
//{
//	RETURN_QUICK_DECLARE_CYCLE_STAT(FThreadManage, STATGROUP_Tickables);
//}
//
//void FThreadManage::ClearAllThread()
//{
//	FScopeLock ScopeLock(&pMutex);
//	for (auto& ThreadProxy : Pool)
//	{
//		ThreadProxy->WaitAndCompleted();
//	}
//	Pool.Empty();
//}
//
//bool FThreadManage::ClearThread(FThreadHandle Handle)
//{
//	FScopeLock ScopeLock(&pMutex);
//	int32 ThreadIndex = INDEX_NONE;
//	for (int32 i = 0 ;i<Pool.Num();i++)
//	{
//		if (Pool[i]->GetThreadHandle()==Handle)
//		{
//			ThreadIndex = i;
//			Pool[i]->WaitAndCompleted();
//			break;
//		}
//	}
//	if (ThreadIndex != INDEX_NONE)
//	{
//		Pool.RemoveAt(ThreadIndex);
//		return true;
//	}
//	return false;
//}
//
//EThreadState FThreadManage::ProceduralProcess(FThreadHandle Handle)
//{
//	if (!Handle.IsValid())
//	{
//		return EThreadState::ERROR;
//	}
//	FScopeLock ScopeLock(&pMutex);
//	for (auto& ThreadProxy : Pool)
//	{
//		if (ThreadProxy->IsSuspend())
//		{
//			if (ThreadProxy->GetThreadHandle() == Handle)
//			{
//				return EThreadState::LEISURELY;
//			}
//		}
//	}
//	return EThreadState::WORKING;
//}
//
//bool FThreadManage::Do(FThreadHandle Handle)
//{
//	FScopeLock ScopeLock(&pMutex);
//	for (auto& ThreadProxy : Pool)
//	{
//		if (ThreadProxy->IsSuspend())
//		{
//			if (ThreadProxy->GetThreadHandle() == Handle)
//			{
//				ThreadProxy->WakeupThread();
//				return true;
//			}
//		}
//	}
//	return false;
//}
//
//bool FThreadManage::DoWait(FThreadHandle Handle)
//{
//	FScopeLock ScopeLock(&pMutex);
//	for (auto& ThreadProxy : Pool)
//	{
//		if (ThreadProxy->IsSuspend())
//		{
//			if (ThreadProxy->GetThreadHandle() == Handle)
//			{
//				ThreadProxy->BlockingAndCompletion();
//				return true;
//			}
//		}
//	}
//	return false;
//}
//
//FThreadHandle FThreadManage::UpdateThreadPool(TSharedPtr<IThreadProxy> Proxy)
//{
//	FScopeLock ScopeLock(&pMutex);
//	Proxy->CreateSafeThread();
//	Pool.Add(Proxy);
//	return Proxy->GetThreadHandle();
//}
