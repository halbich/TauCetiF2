#include "Blocks.h"
#include "ElectricityComponent.h"

// Sets default values for this component's properties
UElectricityComponent::UElectricityComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	// ...
}

UBlockWithElectricityInfo* UElectricityComponent::SetInfo(UBlockWithElectricityInfo* info)
{
	if (!info || !info->IsValidLowLevel())
	{
		info = NewObject<UBlockWithElectricityInfo>();
		//info->CurrentFillingValue = FMath::FRandRange(0, OxygenComponentDef.MaxFilling); // TODO remove me!
	}

	ElectricityInfo = info;
	return ElectricityInfo;
}

void UElectricityComponent::SetDefinition(FElectricityComponentDefinition def)
{
	ElectricityComponentDef = def;
}


void UElectricityComponent::OnComponentDataChanged()
{
	MyComponentDataChangedEvent.Broadcast(ElectricityInfo);
}

FDelegateHandle UElectricityComponent::AddEventListener(FElectricityComponentDataChangedDelegate& dataChangedDelegate)
{
	return MyComponentDataChangedEvent.Add(dataChangedDelegate);
}

void UElectricityComponent::RemoveEventListener(FDelegateHandle DelegateHandle)
{
	MyComponentDataChangedEvent.Remove(DelegateHandle);
}