#pragma once

#include <thread>
#include <iostream>
#include "Tickable.h"

#define CPUThreadNum std::Thread::hardware_concurrency()

#define MUTEX_LOCL FScopeLock Lock(&Mutex)

//�첽
#define USE_UE4_POOL_ASYNCTASK(Delegate)\
(new FAutoDeleteAsyncTask<FSimpleAbandonable>(Delegate))->StartBackgroundTask();

//ͬ��
#define USE_UE4_POOL_SYNCTASK(Delegate)\
{\
	FAsyncTask<FSimpleAbandonable> *SimpleAbandonable = new FAsyncTask<FSimpleAbandonable>(Delegate); \
	SimpleAbandonable->StartBackgroundTask(); \
	SimpleAbandonable->EnsureCompletion(); \
	delete SimpleAbandonable;}

// �첽
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

//ͬ��
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