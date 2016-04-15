

#pragma once

#include "Components/ActorComponent.h"
#include "Components/SelectorComponent.h"
#include "Game/Inventory/BuildableBlockInfo.h"
#include "World/WorldController.h"
#include "Common/WorldObject.h"
#include "BuilderComponent.generated.h"


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class TAUCETIF2_API UBuilderComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UBuilderComponent();

	// Called when the game starts
	virtual void BeginPlay() override;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


	UPROPERTY()
		USelectorComponent* selector;

	UPROPERTY()
		AWorldController* worldController;


	UPROPERTY()
		UBuildableBlockInfo* currentBlockInfo;

	UPROPERTY()
		AWorldObject* currentSpawnedObject;

	UPROPERTY()
	TMap<UBuildableBlockInfo*, AWorldObject*> usedObjects;


	UFUNCTION(BlueprintCallable, Category = BuilderComponent)
		void SetCurrentBuildingItem(UBuildableBlockInfo* blockInfo);

	UFUNCTION(BlueprintCallable, Category = BuilderComponent)
		void SetWorldController(AWorldController* controller);

	void DoAction() {

		if (!selector || !selector->IsValidLowLevel() || !worldController || !selector->IsValidLowLevel() || !currentBlockInfo || !currentBlockInfo->IsValidLowLevel())
			return;

		if (currentBlockInfo->IsEmptyHand)
			return;

		print(TEXT("doing something"));

		auto used = usedObjects.Find(currentBlockInfo);
		check(used && "Failed to find currentBlockInfo object");
	}
};
