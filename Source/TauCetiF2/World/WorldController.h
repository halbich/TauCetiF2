

#pragma once

#include "GameFramework/Actor.h"
#include "WorldController.generated.h"

UCLASS()
class TAUCETIF2_API AWorldController : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWorldController();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	UPROPERTY(BlueprintReadWrite, Category= WorldController)
	TArray<UBlockInfo*> UsedBlocks;

	UFUNCTION(BlueprintCallable, Category = WorldController)
		void LoadBlocksArray(UPARAM(ref)TArray<UBlockInfo*>& blocks);
	
};
