#pragma once
#include "CoreMinimal.h"
#include "Interface/ProxyInterface.h"
#include "Interface/ThreadManageInterface.h"

#include "CoreMinimal.h"
#include "Interface/ProxyInterface.h"
#include "Interface/ThreadManageInterface.h"
#include "ManageBase/ThreadManageBase.h"

class ASYNCTASK_API FThreadAbandonableManagement : public FThreadTemplate<IThreadAbandonableContainer>
{

};