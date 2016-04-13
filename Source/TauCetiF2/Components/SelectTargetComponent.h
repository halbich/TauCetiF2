

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

	// Called when the game starts
	virtual void BeginPlay() override;


	UPROPERTY(EditAnywhere, Category = MySettings)
		bool HasOutline;

	virtual void Select();

	virtual void Deselect();
};
