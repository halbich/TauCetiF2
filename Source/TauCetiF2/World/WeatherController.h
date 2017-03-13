#pragma once

#include "GameFramework/Actor.h"
#include "WeatherController.generated.h"

UCLASS()
class TAUCETIF2_API AWeatherController : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AWeatherController();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
