

#pragma once

#include "Components/ActorComponent.h"
#include "Info/Components/BlockWithOxygenInfo.h"
#include "OxygenComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BLOCKS_API UOxygenComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UOxygenComponent();

	UPROPERTY(BlueprintReadOnly, Transient, Category = BlockInfo)
		UBlockWithOxygenInfo* OxygenInfo;

	UBlockWithOxygenInfo* SetInfo(UBlockWithOxygenInfo* info);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
	
};
