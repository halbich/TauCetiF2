

#pragma once

#include "Components/ActorComponent.h"
#include "Common/WorldObject.h"
#include "SelectorComponent.generated.h"


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class TAUCETIF2_API USelectorComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	USelectorComponent();


	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void BeginPlay() override;


	UPROPERTY(BlueprintReadOnly)
		AWorldObject* SelectedTarget;

	UPROPERTY(BlueprintReadOnly)
		AActor* SelectedActor;


	UPROPERTY(BlueprintReadOnly)
		FVector ImpactPoint;

	UPROPERTY(BlueprintReadOnly)
		FVector ImpactPointWithSnap;

	UPROPERTY(BlueprintReadOnly)
		FVector ImpactNormal;

	UPROPERTY(BlueprintReadWrite)
		AActor* traceIgnoreActor;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		UBlueprint* LockingPlane;

	UFUNCTION(BlueprintCallable, Category = TargetSelector)
		void ShowPlane();

	UFUNCTION(BlueprintCallable, Category = TargetSelector)
		void HidePlane();

private:

	FORCEINLINE void selectComponent(AActor* actor, AWorldObject* worldObj)
	{
		if (actor == SelectedActor)
			return;

		deselectComponent();

		SelectedActor = actor;
		SelectedTarget = worldObj;

		if (SelectedTarget && SelectedTarget->IsValidLowLevel() && SelectedTarget->SelectTargetComponent && SelectedTarget->SelectTargetComponent->IsValidLowLevel())
		{
			SelectedTarget->SelectTargetComponent->Select();

		}

	}

	FORCEINLINE void deselectComponent()
	{
		if (!SelectedActor)
			return;

		if (SelectedTarget && SelectedTarget->IsValidLowLevel() && SelectedTarget->SelectTargetComponent && SelectedTarget->SelectTargetComponent->IsValidLowLevel())
		{
			SelectedTarget->SelectTargetComponent->Deselect();
		}

		SelectedTarget = nullptr;
	}

	UPROPERTY()
		AActor* spawnedPlane;


	UPROPERTY()
		APlayerCameraManager* cameraManager;

	UPROPERTY()
		UWorld* world;

	UPROPERTY()
		APawn* playerPawn;


};


