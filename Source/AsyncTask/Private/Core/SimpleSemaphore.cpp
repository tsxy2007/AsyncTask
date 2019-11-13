#include "Core/SimpleSemaphore.h"
#include "HAL/PlatformProcess.h"
#include "HAL/Event.h"

FSimpleSemaphore::FSimpleSemaphore()
	:Event(FPlatformProcess::GetSynchEventFromPool())
	,bWait(false)
{

}

FSimpleSemaphore::~FSimpleSemaphore()
{
	bWait = false;
	FPlatformProcess::ReturnSynchEventToPool(Event);
}

bool FSimpleSemaphore::Wait()
{
	bWait = true;
	return Event->Wait();
}

bool FSimpleSemaphore::Wait(const FTimespan& WaitTime, const bool bIgnoreThreadIdleStats /*= false*/)
{
	bWait = true;
	return Event->Wait(WaitTime, bIgnoreThreadIdleStats);
}

void FSimpleSemaphore::Trigger()
{
	Event->Trigger();
	bWait = false;
}

