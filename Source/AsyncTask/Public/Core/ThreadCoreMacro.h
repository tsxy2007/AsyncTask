#pragma once

#include "Misc/ScopeLock.h"
#include "Abandonable/SimpleAbandonable.h"
#include "Async/TaskGraphInterfaces.h"


#define DDD(...) __VA_ARGS__

//一下是宏封装
#if PLATFORM_WINDOWS
#include <iostream>
#include <thread>
#define CPUThreadNumber std::thread::hardware_concurrency()
#else
#if PLATFORM_LINUX
#define CPUThreadNumber 12
#else
#if PLATFORM_MAC
#define CPUThreadNumber 12
#endif
#endif
#endif


#define CPUThreadNum std::Thread::hardware_concurrency()

#define MUTEX_LOCL FScopeLock Lock(&Mutex)

//异步
#define USE_UE4_POOL_ASYNCTASK(Delegate)\
(new FAutoDeleteAsyncTask<FSimpleAbandonable>(Delegate))->StartBackgroundTask();

//同步
#define USE_UE4_POOL_SYNCTASK(Delegate)\
{\
	FAsyncTask<FSimpleAbandonable> *SimpleAbandonable = new FAsyncTask<FSimpleAbandonable>(Delegate); \
	SimpleAbandonable->StartBackgroundTask(); \
	SimpleAbandonable->EnsureCompletion(); \
	delete SimpleAbandonable;}

// 异步
#define ASYNCTASK_UOBJECT(UserObject,...)\
USE_UE4_POOL_ASYNCTASK(FSimpleDelegate::CreateUObject(UserObject,__VA_ARGS__))

#define ASYNCTASK_UFUNCTION(UserObject,...)\
USE_UE4_POOL_ASYNCTASK(FSimpleDelegate::CreateUFunction(UserObject,__VA_ARGS__))

#define ASYNCTASK_RAW(UserObject,...)\
USE_UE4_POOL_ASYNCTASK(FSimpleDelegate::CreateRaw(UserObject,__VA_ARGS__))

#define ASYNCTASK_SP(UserObject,...)\
USE_UE4_POOL_ASYNCTASK(FSimpleDelegate::CreateSP(UserObject,__VA_ARGS__))

#define ASYNCTASK_LAMBDA(...)\
USE_UE4_POOL_ASYNCTASK(FSimpleDelegate::CreateLambda(__VA_ARGS__))

#define ASYNCTASK_STATIC(...)\
USE_UE4_POOL_ASYNCTASK(FSimpleDelegate::CreateStatic(__VA_ARGS__))

//同步
#define SYNCTASK_UOBJECT(UserObject,...)\
USE_UE4_POOL_SYNCTASK(FSimpleDelegate::CreateUObject(UserObject,__VA_ARGS__))

#define SYNCTASK_UFUNCTION(UserObject,...)\
USE_UE4_POOL_SYNCTASK(FSimpleDelegate::CreateUFunction(UserObject,__VA_ARGS__))

#define SYNCTASK_RAW(UserObject,...)\
USE_UE4_POOL_SYNCTASK(FSimpleDelegate::CreateRaw(UserObject,__VA_ARGS__))

#define SYNCTASK_SP(UserObject,...)\
USE_UE4_POOL_SYNCTASK(FSimpleDelegate::CreateSP(UserObject,__VA_ARGS__))

#define SYNCTASK_LAMBDA(...)\
USE_UE4_POOL_SYNCTASK(FSimpleDelegate::CreateLambda(__VA_ARGS__))

#define SYNCTASK_STATIC(...)\
USE_UE4_POOL_SYNCTASK(FSimpleDelegate::CreateStatic(__VA_ARGS__))


//呼叫线程 可以设置前置任务
#define CALL_THREAD(OtherTask,CallThreadName,InTaskDeletegate) \
FSimpleDelegateGraphTask::CreateAndDispatchWhenReady(InTaskDeletegate,TStatId(),OtherTask,CallThreadName);

#define CALL_THREAD_UOBJECT(OtherTask,CallThreadName,Object,...) \
CALL_THREAD(OtherTask,CallThreadName,FSimpleDelegate::CreateUObject(Object,__VA_ARGS__))

#define CALL_THREAD_Raw(OtherTask,CallThreadName,Object,...) \
CALL_THREAD(OtherTask,CallThreadName,FSimpleDelegate::CreateRaw(Object,__VA_ARGS__))

#define CALL_THREAD_SP(OtherTask,CallThreadName,Object,...) \
CALL_THREAD(OtherTask,CallThreadName,FSimpleDelegate::CreateSP(Object,__VA_ARGS__))

#define CALL_THREAD_Lambda(OtherTask,CallThreadName,...) \
CALL_THREAD(OtherTask,CallThreadName,FSimpleDelegate::CreateLambda(__VA_ARGS__))

#define CALL_THREAD_UFunction(OtherTask,CallThreadName,Object,...) \
CALL_THREAD(CallThreadName,FSimpleDelegate::CreateUFunction(Object,__VA_ARGS__))

#define WAITING_OTHER_THREADS_COMPLETED(EventRef) \
FTaskGraphInterface::Get().WaitUntilTaskCompletes(EventRef);

#define WAITING_OTHER_THREADS_COMPLETED_MULTI(EventRef) \
FTaskGraphInterface::Get().WaitUntilTasksComplete(EventRef);
