﻿#pragma once

#include "Components/ActorComponent.h"
#include "Tree/KDTree.h"
#include "Tree/MinMaxTree.h"
#include "Tree/WeatherTargetsKDTree.h"
#include "Components/ElectricityComponent.h"
#include "Info/PatternGroupInfo.h"
#include "WorldObjectComponent.generated.h"

class UPatternGroupInfo;

UCLASS(Blueprintable, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class BLOCKS_API UWorldObjectComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UWorldObjectComponent();

	// Elements taken in WorldTree. This collection could be changes as WorldTree Changes
	UPROPERTY(Transient)
		TArray<UKDTree*> TreeElements;

	UPROPERTY(Transient)
		UKDTree* RootBox;

	UPROPERTY(Transient)
		UKDTree* DefiningBox;

	// Elements taken in Weather. This collection could be changes as WeatherTree Changes
	UPROPERTY(Transient)
		TArray<UWeatherTargetsKDTree*> WeatherTreeElements;

	// this is temporary holder for binding electric blocks into grid
	UPROPERTY(Transient)
		TArray<UElectricityComponent*> SurroundingElectricityComponent;

	UPROPERTY(Transient)
		UPatternGroupInfo* PatternGroupInfo;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	void UpdateDefiningBox(UKDTree* definingBox);
};
