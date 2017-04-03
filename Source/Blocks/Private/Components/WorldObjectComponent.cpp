#include "Blocks.h"
#include "WorldObjectComponent.h"

// Sets default values for this component's properties
UWorldObjectComponent::UWorldObjectComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

#pragma optimize("",off)

// Called when the game starts
void UWorldObjectComponent::BeginPlay()
{
	Super::BeginPlay();

	/*auto Element = Cast<ABlock>(GetOwner);

	ensure(Element);

	if (Element->BlockInfo->UnderConstruction)
		return;*/


}

#pragma optimize("",on)

void UWorldObjectComponent::UpdateDefiningBox(UKDTree* definingBox)
{
	DefiningBox = definingBox;
	ensure(DefiningBox != nullptr);

	BuildingTree = NewObject<UMinMaxTree>()->Init(DefiningBox);
}

void UWorldObjectComponent::OnTreeElementsChanged()
{
	// TODO impl?
}

void UWorldObjectComponent::OnWeatherTreeElementsChanged()
{
	// TODO impl?
}

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

	if (DefiningBox && DefiningBox->IsValidLowLevel())
		RootBox->NotifyRegionChanged(DefiningBox);

	for (auto object : WeatherTreeElements)
	{
		if (!object || !object->IsValidLowLevelFast() || object->IsPendingKill())
			continue;

		object->RemoveFromTree(GetOwner());

		/*	auto par = object->GetParent();
			object->MarkPendingKill();

			if (par && par->IsValidLowLevelFast())
				par->UpdateAfterChildDestroyed();*/
	}

	if (BuildingTree && BuildingTree->IsValidLowLevel() && !BuildingTree->IsPendingKill())
	{
		auto parent = BuildingTree->GetParent();
		BuildingTree->MarkPendingKill();
		if (parent)
		{
			parent->ChildrenDeleted();
			FlushPersistentDebugLines(GetWorld());
			parent->GetRoot()->DEBUGDrawBorder(GetWorld());
		}
	}
}
#pragma optimize("", on)