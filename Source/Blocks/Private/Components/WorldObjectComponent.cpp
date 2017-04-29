#include "Blocks.h"
#include "WorldObjectComponent.h"

UWorldObjectComponent::UWorldObjectComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}


void UWorldObjectComponent::UpdateDefiningBox(UKDTree* definingBox)
{
	DefiningBox = definingBox;
	ensure(DefiningBox != nullptr);
}



void UWorldObjectComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	auto isFinalEndGame = EndPlayReason == EEndPlayReason::Type::Quit || EndPlayReason == EEndPlayReason::Type::EndPlayInEditor;

	if (isFinalEndGame)
		RootBox->watchedGroups.Empty();

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
