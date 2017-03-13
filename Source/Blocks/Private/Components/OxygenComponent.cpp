#include "Blocks.h"
#include "OxygenComponent.h"

// Sets default values for this component's properties
UOxygenComponent::UOxygenComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	// ...
}

void UOxygenComponent::BeginPlay()
{
	onComponentDataChanged();
	Super::BeginPlay();
}

UBlockWithOxygenInfo* UOxygenComponent::SetInfo(UBlockWithOxygenInfo* info)
{
	if (!info || !info->IsValidLowLevel())
	{
		info = NewObject<UBlockWithOxygenInfo>();
	}

	OxygenInfo = info;
	return OxygenInfo;
}

void UOxygenComponent::SetDefinition(FOxygenComponentDefinition def)
{
	OxygenComponentDef = def;
}

void UOxygenComponent::onComponentDataChanged()
{
	if (OxygenInfo)
		MyComponentDataChangedEvent.Broadcast(OxygenInfo);
}

FDelegateHandle UOxygenComponent::AddEventListener(FOxygenComponentDataChangedDelegate& dataChangedDelegate)
{
	return MyComponentDataChangedEvent.Add(dataChangedDelegate);
}

void UOxygenComponent::RemoveEventListener(FDelegateHandle DelegateHandle)
{
	MyComponentDataChangedEvent.Remove(DelegateHandle);
}