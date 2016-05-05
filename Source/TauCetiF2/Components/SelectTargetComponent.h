

#pragma once

#include "Components/ActorComponent.h"
#include "SelectTargetComponent.generated.h"


DECLARE_DELEGATE_OneParam(FUseDelegate, AActor*);
DECLARE_EVENT_OneParam(USelectTargetComponent, FUseEvent, AActor*);


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class TAUCETIF2_API USelectTargetComponent : public UActorComponent
{
	GENERATED_BODY()

private:

	UPROPERTY()
		UPrimitiveComponent* objectOutlinePrimitive;

	UPROPERTY()
		UPrimitiveComponent* usableObjectOutlinePrimitive;

	UPROPERTY()
		AActor* owner;

public:
	// Sets default values for this component's properties
	USelectTargetComponent();

	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadOnly, Category = SelectTargetComponent)
		bool HasObjectOutline;

	UPROPERTY(BlueprintReadOnly, Category = SelectTargetComponent)
		bool HasUsableObjectOutline;

	UPROPERTY(BlueprintReadOnly, Category = SelectTargetComponent)
		bool IsUsable;

	UPROPERTY(BlueprintReadOnly, Category = SelectTargetComponent)
		float MaxDistance;

	UPROPERTY(BlueprintReadOnly, Category = SelectTargetComponent)
		FText CustomUsingMessage;

	virtual void SelectObject(int32 stencilValue);

	virtual void DeselectObject();

	virtual void SelectUsableObject();

	virtual void DeselectUsableObject();

	void RegisterTargetObjectPrimitiveComponent(UPrimitiveComponent* comp);


	FORCEINLINE void EnableUse(float maxDistance = -1.0f, UPrimitiveComponent* comp = nullptr)
	{
		IsUsable = true;
		MaxDistance = maxDistance;

		usableObjectOutlinePrimitive = comp == nullptr ? objectOutlinePrimitive : comp;

		if (usableObjectOutlinePrimitive != nullptr && usableObjectOutlinePrimitive->IsValidLowLevel()) {
			HasUsableObjectOutline = true;
		}
	}

	void OnUse(AActor* selectingActor);


	FORCEINLINE bool IsInUsableArea(AActor* selectingActor)
	{
		return selectingActor && IsUsable && (MaxDistance < 0 || FVector::Dist(owner->GetActorLocation(), selectingActor->GetActorLocation()) <= MaxDistance);
	}

public:
	FDelegateHandle AddEventListener(FUseDelegate& UseDelegate);
	void RemoveEventListener(FDelegateHandle DelegateHandle);
private:
	FUseEvent MyUseEvent;

};
