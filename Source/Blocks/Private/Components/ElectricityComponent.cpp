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
		info = NewObject<UBlockWithElectricityInfo>();

	ElectricityInfo = info;
	onComponentDataChanged();
	return ElectricityInfo;
}

void UElectricityComponent::SetDefinition(FElectricityComponentDefinition def)
{
	ElectricityComponentDef = def;
}


void UElectricityComponent::onComponentDataChanged()
{
	if (ElectricityInfo)
		OnComponentDataChangedEvent.Broadcast(ElectricityInfo);
}


bool UElectricityComponent::ObtainAmount(float requested, float& actuallyObtained, bool requireExact)
{
	if (FMath::IsNearlyZero(requested))
	{
		actuallyObtained = 0;
		return true;
	}

	check(ElectricityInfo->CurrentObjectEnergy >= 0);

	if (FMath::IsNearlyZero(ElectricityInfo->CurrentObjectEnergy))
	{
		actuallyObtained = 0;
		return false;
	}

	actuallyObtained = FMath::Min(requested, ElectricityInfo->CurrentObjectEnergy);

	if (requireExact && !FMath::IsNearlyZero(requested - actuallyObtained))
	{
		actuallyObtained = 0;
		return false;
	}

	ElectricityInfo->CurrentObjectEnergy -= actuallyObtained;
	onComponentDataChanged();
	return true;

}

bool UElectricityComponent::PutAmount(float aviable, float& actuallyPutted)
{
	if (FMath::IsNearlyZero(aviable))
	{
		actuallyPutted = 0;
		return true;
	}

	auto aviableToFill = ElectricityComponentDef.TotalObjectEnergy - ElectricityInfo->CurrentObjectEnergy;
	check(aviableToFill >= 0);

	if (FMath::IsNearlyZero(aviableToFill))
	{
		actuallyPutted = 0;
		return false;
	}

	actuallyPutted = FMath::Min(aviable, aviableToFill);

	ElectricityInfo->CurrentObjectEnergy += actuallyPutted;
	onComponentDataChanged();
	return true;

}
