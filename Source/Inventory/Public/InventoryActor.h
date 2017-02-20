#pragma once

#include "GameFramework/Actor.h"
#include "InventoryActor.generated.h"

UCLASS()
class INVENTORY_API AInventoryActor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AInventoryActor();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaSeconds) override;
};
