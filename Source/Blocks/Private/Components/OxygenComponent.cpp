﻿#include "Blocks.h"
#include "OxygenComponent.h"

#pragma optimize("", off)

UOxygenComponent::UOxygenComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
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

void UOxygenComponent::SetDefinition(FOxygenComponentDefinition def, FVector& blockScale, FRotator& blockRotation)
{
	SetDefinition(def);

	auto oxygen = OxygenComponentDef.TotalObjectVolume; // *blockScale.X * blockScale.Y * blockScale.Z;

	OxygenInfo->CurrentObjectMaximumOxygen = oxygen;

	if (OxygenInfo->CurrentObjectOxygen > oxygen)
	{
		print(TEXT("over!"));
	}
	if (OxygenInfo->CurrentObjectOxygen < 0.0f)
	{
		print(TEXT("under!"));
	}


	OxygenInfo->CurrentObjectOxygen = FMath::Clamp(OxygenInfo->CurrentObjectOxygen, 0.0f, oxygen);
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

	if (!OxygenInfo || !OxygenInfo->IsValidLowLevel() || OxygenInfo->IsPendingKill())
	{
		actuallyObtained = 0;
		return false;
	}

	auto aviable = OxygenInfo->CurrentObjectOxygen;

	check(aviable >= 0);

	if (FMath::IsNearlyZero(aviable))
	{
		actuallyObtained = 0;
		return false;
	}

	ensure(requested >= 0.0f);

	actuallyObtained = FMath::Min(requested, aviable);

	if (requireExact && !FMath::IsNearlyZero(requested - actuallyObtained))
	{
		actuallyObtained = 0;
		return false;
	}

	OxygenInfo->CurrentObjectOxygen -= actuallyObtained;

	ensure(OxygenInfo->CurrentObjectOxygen >= 0.0f);
	ensure(OxygenInfo->CurrentObjectOxygen <= OxygenInfo->CurrentObjectMaximumOxygen);

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

	auto aviableToFill = OxygenInfo->CurrentObjectMaximumOxygen - OxygenInfo->CurrentObjectOxygen;
	check(aviableToFill >= 0);

	if (FMath::IsNearlyZero(aviableToFill))
	{
		actuallyPutted = 0;
		return false;
	}

	actuallyPutted = FMath::Min(aviable, aviableToFill);

	OxygenInfo->CurrentObjectOxygen += actuallyPutted;

	ensure(OxygenInfo->CurrentObjectOxygen >= 0.0f);
	ensure(OxygenInfo->CurrentObjectOxygen <= OxygenInfo->CurrentObjectMaximumOxygen);

	onComponentDataChanged();
	return true;
}

void UOxygenComponent::ToggleIsInCreative(bool newInCreative)
{
	IsInCreative = newInCreative;
	onComponentDataChanged();
}

UBlockInfo* UOxygenComponent::GetBlockInfo()
{
	if (!BlockInfo || !BlockInfo->IsValidLowLevel())
		BlockInfo = GetBlockInfoFromParent(this);

	return BlockInfo;
}

#pragma optimize("", on)