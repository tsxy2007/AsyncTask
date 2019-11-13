#pragma once
#include "CoreMinimal.h"

struct ASYNCTASK_API FSimpleThreadHandle : public TSharedFromThis<FSimpleThreadHandle>
{
public:
	FSimpleThreadHandle();

	bool operator==(const FSimpleThreadHandle& Other)
	{
		return 
			GUIDTypeA == Other.GUIDTypeA &&
			GUIDTypeB == Other.GUIDTypeB &&
			GUIDTypeC == Other.GUIDTypeC &&
			GUIDTypeD == Other.GUIDTypeD;
	}
protected:
	uint64 GUIDTypeA;
	uint64 GUIDTypeB;
	uint64 GUIDTypeC;
	uint64 GUIDTypeD;
};

enum class EThreadState
{
	LEISURELY,
	WORKING,
	ERROR,
};

typedef TWeakPtr<FSimpleThreadHandle> FThreadHandle;
typedef TFunction<void()> FThreadLambda;