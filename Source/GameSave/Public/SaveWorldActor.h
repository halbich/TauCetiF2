#pragma once

#include "GameFramework/Actor.h"
#include "SaveWorldActor.generated.h"

UCLASS()
class GAMESAVE_API ASaveWorldActor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASaveWorldActor();

	// Called when the Save starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaSeconds) override;
};
