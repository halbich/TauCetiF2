#pragma once

#include "Components/ActorComponent.h"
#include "WorldObjectComponent.generated.h"

class UMinMaxTree;
class UKDTree;

UCLASS(Blueprintable, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class BLOCKS_API UWorldObjectComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UWorldObjectComponent();

	
	// Elements taken in WorldTree. This collection could be changes as WorldTree Changes
	UPROPERTY(Transient)
		TArray<UKDTree*> TreeElements;

	UPROPERTY(Transient)
		UKDTree* RootBox;

	UPROPERTY(Transient)
		UKDTree* DefiningBox;

	UPROPERTY(Transient)
		UMinMaxTree* BuildingTree;

	// Called when the game starts
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	void UpdateDefiningBox(UKDTree* definingBox);

	void OnTreeElementsChanged();
};
