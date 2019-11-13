#include "Core/SimpleThreadType.h"
#include "Interface/ProxyInterface.h"

uint64 SpawnUniqueID()
{
	return FMath::Abs(FMath::RandRange(100, 0xffffffff) + FDateTime::Now().GetTicks());
}

FSimpleThreadHandle::FSimpleThreadHandle()
	:GUIDTypeA(SpawnUniqueID())
	,GUIDTypeB(SpawnUniqueID())
	,GUIDTypeC(SpawnUniqueID())
	,GUIDTypeD(SpawnUniqueID())
{

}

IThreadProxy::IThreadProxy()
	:SimpleThreadHandle(MakeShareable(new FSimpleThreadHandle))
{

}