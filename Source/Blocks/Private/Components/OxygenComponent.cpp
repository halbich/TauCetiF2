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

UBlockWithOxygenInfo* UOxygenComponent::SetInfo(UBlockWithOxygenInfo* info)
{
	if (!info || !info->IsValidLowLevel())
		info = NewObject<UBlockWithOxygenInfo>();

	OxygenInfo = info;
	onComponentDataChanged();
	return OxygenInfo;
}

void UOxygenComponent::SetDefinition(FOxygenComponentDefinition def)
{
	OxygenComponentDef = def;
}

void UOxygenComponent::onComponentDataChanged()
{
	if (OxygenInfo)
		OnComponentDataChangedEvent.Broadcast(OxygenInfo);
}

bool UOxygenComponent::ObtainAmount(float requested, float& actuallyObtained, bool requireExact)
{
	if (FMath::IsNearlyZero(requested))
	{
		actuallyObtained = 0;
		return true;
	}

	check(OxygenInfo->CurrentFillingValue >= 0);

	if (FMath::IsNearlyZero(OxygenInfo->CurrentFillingValue))
	{
		actuallyObtained = 0;
		return false;
	}

	actuallyObtained = FMath::Min(requested, OxygenInfo->CurrentFillingValue);

	if (requireExact && !FMath::IsNearlyZero(requested - actuallyObtained))
	{
		actuallyObtained = 0;
		return false;
	}

	OxygenInfo->CurrentFillingValue -= actuallyObtained;
	onComponentDataChanged();
	return true;
}

bool UOxygenComponent::PutAmount(float aviable, float& actuallyPutted)
{
	if (FMath::IsNearlyZero(aviable))
	{
		actuallyPutted = 0;
		return true;
	}

	auto aviableToFill = OxygenComponentDef.TotalObjectVolume - OxygenInfo->CurrentFillingValue;
	check(aviableToFill >= 0);

	if (FMath::IsNearlyZero(aviableToFill))
	{
		actuallyPutted = 0;
		return false;
	}

	actuallyPutted = FMath::Min(aviable, aviableToFill);

	OxygenInfo->CurrentFillingValue += actuallyPutted;
	onComponentDataChanged();
	return true;
}


void UOxygenComponent::ToggleIsInCreative(bool newInCreative)
{
	IsInCreative = newInCreative;
	onComponentDataChanged();
}