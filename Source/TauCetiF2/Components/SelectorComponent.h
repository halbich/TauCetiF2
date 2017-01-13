

#pragma once

#include "Components/ActorComponent.h"
#include "Blocks/WorldObject.h"
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


	UPROPERTY(BlueprintAssignable, Category = TargetSelector)
		FUsableObjectTargetedChanged OnUsableObjectTargetedChanged;

	UPROPERTY(BlueprintReadOnly)
		ABlock* SelectedTarget;

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
		void SetOutlining(bool enableOutlining, int32 stencilValue);

	UPROPERTY(BlueprintReadOnly)
		bool usableObjectTargeted;

	UPROPERTY(BlueprintReadOnly)
		int32 StencilValue;


	void OwnerBecomeViewTarget();
private:


	FORCEINLINE void updateUsableObjectTarget()
	{

		auto oldVal = usableObjectTargeted;
		usableObjectTargeted = SelectedActor &&
			SelectedTarget &&
			SelectedTarget->IsValidLowLevel() /*&&
			SelectedTarget->SelectTargetComponent &&
			SelectedTarget->SelectTargetComponent->IsValidLowLevel() &&
			SelectedTarget->SelectTargetComponent->IsInUsableArea(owner)*/;
		// TODO

		if (oldVal != usableObjectTargeted)
		{
			if (usableObjectTargeted)
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

		if (SelectedTarget && SelectedTarget->IsValidLowLevelFast())
		{
			// TODO
			/*if (SelectedTarget->SelectTargetComponent && SelectedTarget->SelectTargetComponent->IsValidLowLevelFast())
			{
				SelectedTarget->SelectTargetComponent->SelectObject(StencilValue);
			}
			else {
				SelectedActor = nullptr;
				SelectedTarget = nullptr;
			}*/

		}
	}

	FORCEINLINE void showUsableObjectOutline()
	{
		if (!SelectedActor)
			return;

		if (SelectedTarget && SelectedTarget->IsValidLowLevelFast())
		{
			// TODO
			/*if (SelectedTarget->SelectTargetComponent && SelectedTarget->SelectTargetComponent->IsValidLowLevelFast())
			{
				SelectedTarget->SelectTargetComponent->SelectUsableObject();
			}
			else {
				SelectedActor = nullptr;
				SelectedTarget = nullptr;
			}*/

		}
	}


	// TODO
	//FORCEINLINE void selectComponent(AActor* actor, AWorldObject* worldObj)
	//{

	//	if (actor && actor == SelectedActor)
	//	{
	//		updateUsableObjectTarget();
	//		return;
	//	}

	//	deselectComponent();

	//	SelectedActor = actor;
	//	//SelectedTarget = worldObj;


	//	if (outliningEnabled )
	//		showObjectOutline();

	//	updateUsableObjectTarget();
	//}


	FORCEINLINE void hideObjectOutline()
	{
		if (!SelectedActor)
			return;

		// TODO 
		/*if (SelectedTarget && SelectedTarget->IsValidLowLevelFast() && SelectedTarget->SelectTargetComponent && SelectedTarget->SelectTargetComponent->IsValidLowLevelFast())
		{
			SelectedTarget->SelectTargetComponent->DeselectObject();
		}*/

	}

	FORCEINLINE void hideUsableObjectOutline()
	{
		if (!SelectedActor)
			return;

	/*	if (SelectedTarget && SelectedTarget->IsValidLowLevelFast() && SelectedTarget->SelectTargetComponent && SelectedTarget->SelectTargetComponent->IsValidLowLevelFast())
		{
			SelectedTarget->SelectTargetComponent->DeselectUsableObject();
		}*/

	}

	FORCEINLINE void deselectComponent()
	{
		hideObjectOutline();
		hideUsableObjectOutline();

		SelectedTarget = nullptr;
		SelectedActor = nullptr;
		updateUsableObjectTarget();

	}

	UPROPERTY()
		AActor* spawnedPlane;


	UPROPERTY()
		APlayerCameraManager* cameraManager;

	UPROPERTY()
		APawn* playerPawn;


};


