

#pragma once

#include "Components/ActorComponent.h"
#include "Components/SelectorComponent.h"
#include "Game/Inventory/BuildableBlockInfo.h"
#include "World/WorldController.h"
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


	UPROPERTY()
		USelectorComponent* selector;

	UPROPERTY()
		AWorldController* worldController;

	UFUNCTION(BlueprintCallable, Category = BuilderComponent)
		void SetCurrentBuildingItem(UBuildableBlockInfo* blockInfo);

	UFUNCTION(BlueprintCallable, Category = BuilderComponent)
		void SetWorldController(AWorldController* controller);

	FORCEINLINE void DoAction() {

		if (!selector || !selector->IsValidLowLevel() || !worldController || !selector->IsValidLowLevel())
			return;

		print(TEXT("doing something"));
	}
};
