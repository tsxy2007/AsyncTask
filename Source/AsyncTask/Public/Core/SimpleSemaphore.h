#pragma once
#include "CoreMinimal.h"

struct FSimpleSemaphore 
{
	FSimpleSemaphore();
	~FSimpleSemaphore();
public:
	bool Wait();
	bool Wait(const FTimespan& WaitTime, const bool bIgnoreThreadIdleStats = false);
	void Trigger();

	FORCEINLINE bool IsWait() { return bWait; }
private:
	class FEvent* Event;
	uint8 bWait : 1;
};