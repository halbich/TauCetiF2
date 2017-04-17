#include "Blocks.h"
#include "ElectricityComponent.h"

#pragma optimize("", off)

UElectricityComponent::UElectricityComponent() : InOutCritical()
{
	PrimaryComponentTick.bCanEverTick = false;
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
	ElectricityInfo->CurrentObjectMaximumEnergy = def.TotalObjectEnergy;
}

void UElectricityComponent::SetDefinition(FElectricityComponentDefinition def, FVector& blockScale, FRotator& blockRotation)
{
	SetDefinition(def);

	auto energy = ElectricityComponentDef.TotalObjectEnergy * blockScale.X * blockScale.Y * blockScale.Z;

	ElectricityInfo->CurrentObjectMaximumEnergy = energy;

	if (ElectricityInfo->CurrentObjectEnergy > energy)
	{
		print(TEXT("over!"));
	}
	if (ElectricityInfo->CurrentObjectEnergy < 0.0f)
	{
		print(TEXT("under!"));
	}


	ElectricityInfo->CurrentObjectEnergy = FMath::Clamp(ElectricityInfo->CurrentObjectEnergy, 0.0f, energy);

	auto worldLocation = GetOwner()->GetActorLocation();

	for (auto areaDef : ElectricityComponentDef.BindableAreas.Planes)
	{
		auto bindArea = NewObject<UElectricityBindableAreaInfo>();
		bindArea->InitArea(ElectricityComponentDef.BindableAreas.UsedPoints, areaDef, blockScale, blockRotation, worldLocation);
		ElectricityBindableAreas.Add(bindArea);
		bindArea->DEBUG_DrawPoints(GetWorld());
	}

	auto surroundings = GetSurroundingComponents(this);

	for (auto surrounding : surroundings)
	{
		for (auto bindable : surrounding->ElectricityBindableAreas)
		{
			for (auto myBindable : ElectricityBindableAreas)
			{
				if (bindable->DominantPlane != myBindable->DominantPlane)
					continue;

				if (isValidConnection(myBindable, bindable))
				{
					ConnectedComponents.Add(surrounding);
					surrounding->ConnectedComponents.Add(this);
				}
			}
		}
	}

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

	if (!ElectricityInfo || !ElectricityInfo->IsValidLowLevel() || ElectricityInfo->IsPendingKill())
	{
		actuallyObtained = 0;
		return false;
	}

	InOutCritical.Lock();

	auto aviable = ElectricityInfo->CurrentObjectEnergy;
	check(aviable >= 0);

	if (FMath::IsNearlyZero(aviable))
	{
		actuallyObtained = 0;
		InOutCritical.Unlock();
		return false;
	}

	ensure(requested >= 0.0f);

	actuallyObtained = FMath::Min(requested,aviable);

	if (requireExact && !FMath::IsNearlyZero(requested - actuallyObtained))
	{
		actuallyObtained = 0;
		InOutCritical.Unlock();
		return false;
	}

	ElectricityInfo->CurrentObjectEnergy -= actuallyObtained;

	ensure(ElectricityInfo->CurrentObjectEnergy >= 0.0f);
	ensure(ElectricityInfo->CurrentObjectEnergy <= ElectricityInfo->CurrentObjectMaximumEnergy);

	InOutCritical.Unlock();
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

	InOutCritical.Lock();

	auto aviableToFill = ElectricityInfo->CurrentObjectMaximumEnergy - ElectricityInfo->CurrentObjectEnergy;
	check(aviableToFill >= 0);

	if (FMath::IsNearlyZero(aviableToFill))
	{
		actuallyPutted = 0;
		InOutCritical.Unlock();
		return false;
	}

	actuallyPutted = FMath::Min(aviable, aviableToFill);

	ElectricityInfo->CurrentObjectEnergy += actuallyPutted;

	ensure(ElectricityInfo->CurrentObjectEnergy >= 0.0f);
	ensure(ElectricityInfo->CurrentObjectEnergy <= ElectricityInfo->CurrentObjectMaximumEnergy);

	InOutCritical.Unlock();
	onComponentDataChanged();
	return true;
}

void UElectricityComponent::ToggleIsInCreative(bool newInCreative)
{
	IsInCreative = newInCreative;
	onComponentDataChanged();
}

bool UElectricityComponent::isValidConnection(UElectricityBindableAreaInfo* area, UElectricityBindableAreaInfo* otherArea)
{
	auto box1 = area->GetBox();
	auto box2 = otherArea->GetBox();

	box1 = box1.ExpandBy(area->DominantPlane.GetSafeNormal().GetAbs());
	box2 = box2.ExpandBy(otherArea->DominantPlane.GetSafeNormal().GetAbs());

	if (box1.Intersect(box2))
	{
		auto volume = IntersectVolume(box1, box2);
		if (volume == 0)
			return false;
	}

	// TODO
	return true;
}

void UElectricityComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (EndPlayReason == EEndPlayReason::Type::Quit)
		return;

	for (auto connected : ConnectedComponents)
	{
		connected->ConnectedComponents.Remove(this);
	}
	ConnectedComponents.Empty();
}

void UElectricityComponent::UpdateHealthSeverity()
{
	if (!Network || !Network->IsValidLowLevel())
		return;
}

UBlockInfo* UElectricityComponent::GetBlockInfo()
{
	if (!BlockInfo || !BlockInfo->IsValidLowLevel())
		BlockInfo = GetBlockInfoFromParent(this);

	return BlockInfo;
}

#pragma optimize("", on)