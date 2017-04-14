#include "Commons.h"
#include "TCF2GameInstance.h"

void UTCF2GameInstance::SetHolderInstance(UObject* holder)
{
	BlockHolder = holder;
	BlockHolder->AddToRoot();
}

void UTCF2GameInstance::DaytimeChanged(bool isNight)
{
	IsNightInGame = isNight;
	OnDaytimeChangedEvent.Broadcast(isNight);
}