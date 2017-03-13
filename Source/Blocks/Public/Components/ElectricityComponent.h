#pragma once

#include "Components/ActorComponent.h"
#include "Info/Components/BlockWithElectricityInfo.h"
#include "Definitions/ElectricityComponentDefinition.h"
#include "ElectricityComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FElectricityComponentDataChangedDelegate, UBlockWithElectricityInfo*, info);

UCLASS(BlueprintType, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class BLOCKS_API UElectricityComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UElectricityComponent();

	UPROPERTY(BlueprintReadOnly, Transient)
		UBlockWithElectricityInfo* ElectricityInfo;

	UPROPERTY(BlueprintReadOnly, Transient, Category = "TCF2 | OxygenComponent")
		FElectricityComponentDefinition ElectricityComponentDef;

private:
	void onComponentDataChanged();

public:

	UBlockWithElectricityInfo* SetInfo(UBlockWithElectricityInfo* info);

	void SetDefinition(FElectricityComponentDefinition def);

public:
	FORCEINLINE const FElectricityComponentDefinition* GetDefinition()
	{
		return &ElectricityComponentDef;
	}

	UPROPERTY(BlueprintAssignable, Category = "TCF2 | ElectricityComponent")
		FElectricityComponentDataChangedDelegate OnComponentDataChangedEvent;
};
