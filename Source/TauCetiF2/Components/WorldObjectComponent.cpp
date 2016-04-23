

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


// Called when the game starts
void UWorldObjectComponent::BeginPlay()
{
	Super::BeginPlay();

	ensure(BlockInfo);

	if (BlockInfo->UnderConstruction)
		return;

	ensure(BuildingTree);




	ensure(DefiningBox);
	ensure(TreeElements.Num() > 0);

	RootBox = TreeElements[0]->GetRootNode<UKDTree>(true);
	ensure(RootBox != nullptr);

	auto surroundings = NewObject<UKDTree>()->Init(DefiningBox, RootBox);

	TArray<AWorldObject*> items;
	TreeElements[0]->GetContainingObjectsFromBottom(surroundings, items);
	print(TEXT("surroundings:"));

	TArray<UMinMaxTree*> usedTrees;

	for (auto object : items)
	{
		if (!object || !object->IsValidLowLevelFast() || object == DefiningBox->ContainingObject)
			continue;

		auto woc = object->WorldObjectComponent;
		if (!woc || !woc->IsValidLowLevel() || woc->BlockInfo->ID != BlockInfo->ID)
			continue;

		ensure(woc->BuildingTree);
		usedTrees.AddUnique(woc->BuildingTree->GetRoot());
	}

	for (auto rootObj : usedTrees)
	{
		print(TEXT("rootOBje"));
		rootObj->Insert(BuildingTree);
	}


	BuildingTree->GetRoot()->DEBUGDrawBorder(GetWorld());
}


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

	//ensure(usedBoxes.Num() > 0);

	//UKDTree* highestElem = nullptr;
	//TreeElements = TArray<UKDTree*>(usedBoxes);
	//for (auto elem : TreeElements)
	//{
	//	auto currentItem = Cast<UKDTree>(elem->ParentNode);
	//	if (!highestElem)
	//	{
	//		highestElem = currentItem;
	//		continue;
	//	}

	//	if (highestElem->DividingIndex > currentItem->DividingIndex)
	//		highestElem = currentItem;
	//}



	//RootBox = highestElem->GetParentNode<UKDTree>(true);
	//ensure(RootBox != nullptr);


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

		auto par = Cast<UKDTree>(object->ParentNode);
		object->MarkPendingKill();

		if (par && par->IsValidLowLevelFast())
			par->UpdateAfterChildDestroyed();
	}



}