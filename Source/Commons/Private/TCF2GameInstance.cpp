

#include "Commons.h"
#include "TCF2GameInstance.h"




void UTCF2GameInstance::SetHolderInstance(UObject* holder)
{
	BlockHolder = holder;
	BlockHolder->AddToRoot();
}