

#include "TauCetiF2.h"
#include "WorldObjectComponent.h"


// Sets default values for this component's properties
UWorldObjectComponent::UWorldObjectComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

#pragma optimize("",off)

// Called when the game starts
void UWorldObjectComponent::BeginPlay()
{
	Super::BeginPlay();

	ensure(BlockInfo);

	if (BlockInfo->UnderConstruction)
		return;

	ensure(BuildingTree);


	FlushPersistentDebugLines(GetWorld());



	ensure(DefiningBox);
	ensure(TreeElements.Num() > 0);

	RootBox = TreeElements[0]->GetRootNode<UKDTree>(true);
	ensure(RootBox != nullptr);

	auto surroundings = NewObject<UKDTree>()->Init(DefiningBox, RootBox);

	//surroundings->DEBUGDrawSurrondings(GetWorld());

	TArray<AWorldObject*> items;
	//TreeElements[0]->DEBUGDrawSurrondings(GetWorld(), FColor::Black);
	TreeElements[0]->GetContainingObjectsFromBottom(surroundings, items, this);
	print(TEXT("surroundings:"));

	TArray<UMinMaxTree*> usedTrees;

	for (auto object : items)
	{
		if (!object || !object->IsValidLowLevelFast())
			continue;

		auto woc = object->WorldObjectComponent;
		if (!woc || !woc->IsValidLowLevel())
			continue;

		ensure(woc->BuildingTree);
		usedTrees.AddUnique(woc->BuildingTree->GetRoot());
		print(*woc->DefiningBox->ContainingObject->GetName());
	}

	for (auto rootObj : usedTrees)
	{
		rootObj->Insert(BuildingTree);
	}

	BuildingTree->GetRoot()->DEBUGDrawBorder(GetWorld());
}


#pragma optimize("",on)

// Called every frame
void UWorldObjectComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UWorldObjectComponent::UpdateDefiningBox(UKDTree* definingBox)
{
	DefiningBox = definingBox;
	ensure(DefiningBox != nullptr);

	BuildingTree = NewObject<UMinMaxTree>()->Init(DefiningBox);

}

void UWorldObjectComponent::OnTreeElementsChanged()
{

}

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