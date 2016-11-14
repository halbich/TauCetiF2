

#include "GameWorld.h"
#include "GameWorldActor.h"


// Sets default values
AGameWorldActor::AGameWorldActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AGameWorldActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGameWorldActor::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
	print(TEXT("GameWorldActor tick"));
}

