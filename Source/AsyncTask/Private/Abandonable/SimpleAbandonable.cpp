#include "Abandonable/SimpleAbandonable.h"

FSimpleAbandonable::FSimpleAbandonable(const FSimpleDelegate& InDelegate)
	:Delegate(InDelegate)
{

}

FSimpleAbandonable::~FSimpleAbandonable()
{

}

void FSimpleAbandonable::DoWork()
{
	Delegate.ExecuteIfBound();
}

