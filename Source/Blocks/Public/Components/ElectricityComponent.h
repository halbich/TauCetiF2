

#pragma once

#include "Components/ActorComponent.h"
#include "Info/Components/BlockWithElectricityInfo.h"
#include "ElectricityComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BLOCKS_API UElectricityComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UElectricityComponent();

	UPROPERTY(BlueprintReadOnly, Transient, Category = BlockInfo)
		UBlockWithElectricityInfo* ElectricityInfo;

	UBlockWithElectricityInfo* SetInfo(UBlockWithElectricityInfo* info);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
	
};
