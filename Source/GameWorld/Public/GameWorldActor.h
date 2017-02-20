#pragma once

#include "GameFramework/Actor.h"
#include "GameWorldActor.generated.h"

UCLASS()
class GAMEWORLD_API AGameWorldActor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AGameWorldActor();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaSeconds) override;
};
