﻿#include "Blocks.h"
#include "ElectricityComponent.h"


#pragma optimize("", off)

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

void UElectricityComponent::SetDefinition(FElectricityComponentDefinition def, FVector& blockScale, FRotator& blockRotation)
{
	SetDefinition(def);


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

	if (ConnectedComponents.Num() > 0) {
		print(TEXT("Valid connections:"));
		print(*FText::AsNumber(ConnectedComponents.Num()).ToString());
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

#pragma optimize("", on)