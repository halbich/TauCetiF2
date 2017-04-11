#pragma once

#include "Block.h"
#include "Components/ActorComponent.h"
#include "BlockHolderComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class BLOCKS_API UBlockHolderComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UBlockHolderComponent();

	

	// Called when the game starts
	virtual void BeginPlay() override;



};
