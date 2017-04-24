#include "Blocks.h"
#include "WorldObjectComponent.h"

UWorldObjectComponent::UWorldObjectComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

#pragma optimize("",off)

void UWorldObjectComponent::BeginPlay()
{
	Super::BeginPlay();
}

#pragma optimize("",on)

void UWorldObjectComponent::UpdateDefiningBox(UKDTree* definingBox)
{
	DefiningBox = definingBox;
	ensure(DefiningBox != nullptr);
}

//void UWorldObjectComponent::OnTreeElementsChanged()
//{
//	// TODO impl?
//}

//void UWorldObjectComponent::OnWeatherTreeElementsChanged()
//{
//	// TODO impl?
//}

#pragma optimize("", off)

void UWorldObjectComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (EndPlayReason == EEndPlayReason::Type::Quit)
		return;

	for (auto object : TreeElements)
	{
		if (!object || !object->IsValidLowLevelFast() || object->IsPendingKill())
			continue;

		auto par = object->GetParent();
		object->MarkPendingKill();

		if (par && par->IsValidLowLevelFast())
			par->UpdateAfterChildDestroyed();
	}

	TreeElements.Empty();

	if (DefiningBox && DefiningBox->IsValidLowLevel())
		RootBox->NotifyRegionChanged(DefiningBox);

	for (auto object : WeatherTreeElements)
	{
		if (!object || !object->IsValidLowLevelFast() || object->IsPendingKill())
			continue;

		object->RemoveFromTree(GetOwner());
	}

	WeatherTreeElements.Empty();
}
#pragma optimize("", on)