

#pragma once

#include "Components/ActorComponent.h"
#include "Blocks/Info/BlockInfo.h"
#include "World/MinMaxBox.h"
#include "World/KDTree.h"
#include "WorldObjectComponent.generated.h"


UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TAUCETIF2_API UWorldObjectComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UWorldObjectComponent();


	UPROPERTY(BlueprintReadOnly, Category = WorldObjectComponent)
		UBlockInfo* BlockInfo;


	UPROPERTY()
		UKDTree* TreeElement;

	// Called when the game starts
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

		
	
};
