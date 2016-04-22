

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

	// ...

}


// Called every frame
void UWorldObjectComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UWorldObjectComponent::UpdateTree(UMinMaxBox* definingBox, TArray<UKDTree*>& usedBoxes)
{
	DefiningBox = definingBox;
	ensure(DefiningBox != nullptr);

	ensure(usedBoxes.Num() > 0);

	UKDTree* highestElem = nullptr;
	TreeElements = TArray<UKDTree*>(usedBoxes);
	for (auto elem : TreeElements)
	{
		auto currentItem = Cast<UKDTree>(elem->ParentNode);
		if (!highestElem)
		{
			highestElem = currentItem;
			continue;
		}

		if (highestElem->DividingIndex > currentItem->DividingIndex)
			highestElem = currentItem;
	}



	RootBox = highestElem->GetParentNode<UKDTree>(true);
	ensure(RootBox != nullptr);

	auto surroundings = NewObject<UKDTree>()->Init(DefiningBox, RootBox);
	surroundings->DEBUGDrawSurrondings(GetWorld());

	TArray<AWorldObject*> items;
	highestElem->GetContainingObjectsFromBottom(surroundings, items);
	for (auto object : items)
	{
		if (object)
			print(*object->GetName());
	}
}