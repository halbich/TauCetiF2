#include "GameSave.h"
#include "SaveWorldActor.h"

// Sets default values
ASaveWorldActor::ASaveWorldActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the Save starts or when spawned
void ASaveWorldActor::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ASaveWorldActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	print(TEXT("SaveWorldActor tick"));
}