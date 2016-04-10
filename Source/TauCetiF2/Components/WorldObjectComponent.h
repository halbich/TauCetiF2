

#pragma once

#include "Components/ActorComponent.h"
#include "Blocks/BlockInfo.h"
#include "WorldObjectComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TAUCETIF2_API UWorldObjectComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UWorldObjectComponent();


	UPROPERTY(BlueprintReadOnly, Category = WorldObjectComponent)
		UBlockInfo* BlockInfo;


	// Called when the game starts
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

		
	
};
