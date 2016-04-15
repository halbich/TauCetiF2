

#pragma once

#include "Components/ActorComponent.h"
#include "Common/WorldObject.h"
#include "SelectorComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FUsableObjectTargetedChanged, bool, IsUsableObjectTargeted);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class TAUCETIF2_API USelectorComponent : public UActorComponent
{
	GENERATED_BODY()


		UPROPERTY()
		AActor* owner;

public:
	// Sets default values for this component's properties
	USelectorComponent();


	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void BeginPlay() override;


	UPROPERTY(BlueprintAssignable, Category = TargetSelector)
		FUsableObjectTargetedChanged OnUsableObjectTargetedChanged;

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

	UFUNCTION(BlueprintCallable, Category = TargetSelector)
		void TrySelect();

	UPROPERTY(BlueprintReadOnly)
		bool outliningEnabled;

	UFUNCTION(BlueprintCallable, Category = TargetSelector)
		void SetOutlining(bool enableOutlining);

	UPROPERTY(BlueprintReadOnly)
		bool usableObjectTargeted;

private:


	FORCEINLINE void updateUsableObjectTarget()
	{
		auto oldVal = usableObjectTargeted;
		usableObjectTargeted = SelectedActor &&
			SelectedTarget &&
			SelectedTarget->IsValidLowLevel() &&
			SelectedTarget->SelectTargetComponent &&
			SelectedTarget->SelectTargetComponent->IsValidLowLevel() &&
			SelectedTarget->SelectTargetComponent->IsInUsableArea(owner);

		if (oldVal != usableObjectTargeted)
		{
			OnUsableObjectTargetedChanged.Broadcast(usableObjectTargeted);
			print(usableObjectTargeted ? TEXT("UsableObjectTargeted") : TEXT("NoTarget"));
		}
	}

	FORCEINLINE void showOutline()
	{
		if (!SelectedActor)
			return;

		if (SelectedTarget && SelectedTarget->IsValidLowLevel())
		{
			if (SelectedTarget->SelectTargetComponent && SelectedTarget->SelectTargetComponent->IsValidLowLevel())
			{
				SelectedTarget->SelectTargetComponent->Select();
			}
			else {
				SelectedActor = nullptr;
				SelectedTarget = nullptr;
			}

		}
	}


	FORCEINLINE void selectComponent(AActor* actor, AWorldObject* worldObj)
	{
		if (actor && actor == SelectedActor)
			return;

		deselectComponent();

		SelectedActor = actor;
		SelectedTarget = worldObj;


		if (outliningEnabled)
			showOutline();

		updateUsableObjectTarget();
	}


	FORCEINLINE void hideOutline()
	{
		if (!SelectedActor)
			return;

		if (SelectedTarget && SelectedTarget->IsValidLowLevel() && SelectedTarget->SelectTargetComponent && SelectedTarget->SelectTargetComponent->IsValidLowLevel())
		{
			SelectedTarget->SelectTargetComponent->Deselect();
		}

	}

	FORCEINLINE void deselectComponent()
	{
		hideOutline();

		SelectedTarget = nullptr;
		SelectedActor = nullptr;

		updateUsableObjectTarget();
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


