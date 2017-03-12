#pragma once

#include "Components/ActorComponent.h"
#include "SelectorComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FUsableObjectTargetedChanged, bool, IsUsableObjectTargeted);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class TAUCETIF2_API USelectorComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	// Sets default values for this component's properties
	USelectorComponent();

	UPROPERTY(Transient)
		AActor* owner;

	UPROPERTY(BlueprintAssignable, Transient, Category = TargetSelector)
		FUsableObjectTargetedChanged OnUsableObjectTargetedChanged;

	UPROPERTY(BlueprintReadOnly, Transient)
		ABlock* SelectedBlock;

	UPROPERTY(BlueprintReadOnly, Transient)
		AActor* SelectedActor;

	UPROPERTY(BlueprintReadOnly, Transient)
		FVector ImpactPoint;

	UPROPERTY(BlueprintReadOnly, Transient)
		FVector ImpactPointWithSnap;

	UPROPERTY(BlueprintReadOnly, Transient)
		FVector ImpactNormal;

	UPROPERTY(BlueprintReadWrite, Transient)
		AActor* traceIgnoreActor;

	UPROPERTY(BlueprintReadWrite, Transient, EditAnywhere)
		UBlueprint* LockingPlane;

	UPROPERTY(BlueprintReadOnly, Transient)
		bool outliningEnabled;

	UPROPERTY(BlueprintReadOnly, Transient)
		bool usableObjectTargeted;

	UPROPERTY(BlueprintReadOnly, Transient)
		int32 StencilValue;

	UPROPERTY(Transient)
		AActor* spawnedPlane;

	UPROPERTY(Transient)
		APlayerCameraManager* cameraManager;

	UPROPERTY(Transient)
		APawn* playerPawn;

	UPROPERTY(Transient)
		bool IsUsableAllowed;

	UFUNCTION(BlueprintCallable, Category = TargetSelector)
		void SetOutlining(bool enableOutlining, int32 outlineStencilValue);

	UFUNCTION(BlueprintCallable, Category = TargetSelector)
		void ShowPlane();

	UFUNCTION(BlueprintCallable, Category = TargetSelector)
		void HidePlane();

	UFUNCTION(BlueprintCallable, Category = TargetSelector)
		void TrySelect();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void OwnerBecomeViewTarget();
private:

	FORCEINLINE void updateUsableObjectTarget()
	{
		auto oldVal = usableObjectTargeted;
		usableObjectTargeted = IsUsableAllowed && SelectedActor &&
			SelectedBlock &&
			SelectedBlock->IsValidLowLevel() &&
			SelectedBlock->IsInUsableArea(owner);

		if (oldVal != usableObjectTargeted)
		{
			if ( usableObjectTargeted)
				showUsableObjectOutline();
			else
				hideUsableObjectOutline();

			OnUsableObjectTargetedChanged.Broadcast(usableObjectTargeted);
		}
	}

	FORCEINLINE void showObjectOutline()
	{
		if (!SelectedActor)
			return;

		if (SelectedBlock && SelectedBlock->IsValidLowLevelFast())
		{
			if (SelectedBlock->SelectTargetComponent && SelectedBlock->SelectTargetComponent->IsValidLowLevelFast())
			{
				SelectedBlock->SelectTargetComponent->SelectObject(StencilValue);
			}
			else {
				SelectedActor = nullptr;
				SelectedBlock = nullptr;
			}
		}
	}

	FORCEINLINE bool showUsableObjectOutline()
	{
		if (!SelectedActor || !IsUsableAllowed)
			return false;

		if (SelectedBlock && SelectedBlock->IsValidLowLevelFast())
		{
			if (SelectedBlock->SelectTargetComponent && SelectedBlock->SelectTargetComponent->IsValidLowLevelFast())
			{
				SelectedBlock->SelectTargetComponent->SelectUsableObject();
				return true;
			}
			else {
				SelectedActor = nullptr;
				SelectedBlock = nullptr;
			}
		}
		return false;
	}

	FORCEINLINE void selectComponent(AActor* actor, ABlock* worldObj)
	{
		if (actor && actor == SelectedActor)
		{
			updateUsableObjectTarget();
			return;
		}

		deselectComponent();

		SelectedActor = actor;
		SelectedBlock = worldObj;

		if (outliningEnabled)
			showObjectOutline();

		updateUsableObjectTarget();
	}

	FORCEINLINE void hideObjectOutline()
	{
		if (!SelectedActor)
			return;

		if (SelectedBlock && SelectedBlock->IsValidLowLevelFast() && SelectedBlock->SelectTargetComponent && SelectedBlock->SelectTargetComponent->IsValidLowLevelFast())
		{
			SelectedBlock->SelectTargetComponent->DeselectObject();
		}
	}

	FORCEINLINE void hideUsableObjectOutline()
	{
		if (!SelectedActor)
			return;

		if (SelectedBlock && SelectedBlock->IsValidLowLevelFast() && SelectedBlock->SelectTargetComponent && SelectedBlock->SelectTargetComponent->IsValidLowLevelFast())
		{
			SelectedBlock->SelectTargetComponent->DeselectUsableObject();
		}
	}

	FORCEINLINE void deselectComponent()
	{
		hideObjectOutline();
		hideUsableObjectOutline();

		SelectedBlock = nullptr;
		SelectedActor = nullptr;
		updateUsableObjectTarget();
	}
};
