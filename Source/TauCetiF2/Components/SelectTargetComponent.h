

#pragma once

#include "Components/ActorComponent.h"
#include "SelectTargetComponent.generated.h"


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class TAUCETIF2_API USelectTargetComponent : public UActorComponent
{
	GENERATED_BODY()

		UPrimitiveComponent* primitive;

public:
	// Sets default values for this component's properties
	USelectTargetComponent();


	UPROPERTY(BlueprintReadOnly, Category = SelectTargetComponent)
		bool HasOutline;

	UPROPERTY(BlueprintReadOnly, Category = SelectTargetComponent)
		bool IsUsable;

	UPROPERTY(BlueprintReadOnly, Category = SelectTargetComponent)
		float MaxDistance;

	virtual void Select();

	virtual void Deselect();

	void RegisterTargetPrimitiveComponent(UPrimitiveComponent* comp);

	FORCEINLINE void EnableUse(float maxDistance = -1.0f)
	{
		IsUsable = true;
		MaxDistance = maxDistance;
	}

	void OnUse(AActor* selectingActor);
	
};
