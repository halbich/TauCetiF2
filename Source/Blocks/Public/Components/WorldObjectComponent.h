#pragma once

#include "Components/ActorComponent.h"
#include "Tree/KDTree.h"
#include "Tree/MinMaxTree.h"
#include "Tree/WeatherTargetsKDTree.h"
#include "Components/ElectricityComponent.h"
#include "WorldObjectComponent.generated.h"

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

	// Tree for shapes recognition
	UPROPERTY(Transient)
		UMinMaxTree* BuildingTree;

	// Elements taken in Weather. This collection could be changes as WeatherTree Changes
	UPROPERTY(Transient)
		TArray<UWeatherTargetsKDTree*> WeatherTreeElements;

	// this is temporary holder for binding electric blocks into grid
	UPROPERTY(Transient)
		TArray<UElectricityComponent*> SurroundingElectricityComponent;

	// Called when the game starts
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	void UpdateDefiningBox(UKDTree* definingBox);

	void OnTreeElementsChanged();

	void OnWeatherTreeElementsChanged();
};
