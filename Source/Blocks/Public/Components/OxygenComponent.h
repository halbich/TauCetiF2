#pragma once

#include "Components/ActorComponent.h"
#include "Info/Components/BlockWithOxygenInfo.h"
#include "Definitions/OxygenComponentDefinition.h"
#include "OxygenComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOxygenComponentDataChangedDelegate, UBlockWithOxygenInfo*, info);

UCLASS(BlueprintType, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class BLOCKS_API UOxygenComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UOxygenComponent();

	UPROPERTY(BlueprintReadOnly, Transient)
		UBlockWithOxygenInfo* OxygenInfo;

	UPROPERTY(BlueprintReadOnly, Transient, Category = "TCF2 | OxygenComponent")
		FOxygenComponentDefinition OxygenComponentDef;

private:
	void onComponentDataChanged();


public:

	UBlockWithOxygenInfo* SetInfo(UBlockWithOxygenInfo* info);

	void SetDefinition(FOxygenComponentDefinition def);

public:
	FORCEINLINE const FOxygenComponentDefinition* GetDefinition()
	{
		return &OxygenComponentDef;
	}

	UPROPERTY(BlueprintAssignable, Category = "TCF2 | OxygenComponent")
		FOxygenComponentDataChangedDelegate OnComponentDataChangedEvent;
};
