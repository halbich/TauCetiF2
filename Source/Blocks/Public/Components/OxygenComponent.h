#pragma once

#include "Components/ActorComponent.h"
#include "Info/Components/BlockWithOxygenInfo.h"
#include "Definitions/OxygenComponentDefinition.h"
#include "OxygenComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class BLOCKS_API UOxygenComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UOxygenComponent();

private:
	UPROPERTY(Transient)
		UBlockWithOxygenInfo* OxygenInfo;

	UPROPERTY(Transient)
		FOxygenComponentDefinition OxygenComponentDef;

public:

	UBlockWithOxygenInfo* SetInfo(UBlockWithOxygenInfo* info);

	void SetDefinition(FOxygenComponentDefinition def);
};
