#pragma once

#include "Components/ActorComponent.h"
#include "Info/Components/BlockWithOxygenInfo.h"
#include "Definitions/OxygenComponentDefinition.h"
#include "OxygenComponent.generated.h"


DECLARE_DELEGATE_OneParam(FOxygenComponentDataChangedDelegate, UBlockWithOxygenInfo*);
DECLARE_EVENT_OneParam(UOxygenComponent, FOxygenComponentDataChangedEvent, UBlockWithOxygenInfo*);

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

	void onComponentDataChanged();

protected:

	void BeginPlay();
		 

public:

	UBlockWithOxygenInfo* SetInfo(UBlockWithOxygenInfo* info);

	void SetDefinition(FOxygenComponentDefinition def);

public:
	FDelegateHandle AddEventListener(FOxygenComponentDataChangedDelegate& dataChangedDelegate);
	void RemoveEventListener(FDelegateHandle DelegateHandle);

	FORCEINLINE const FOxygenComponentDefinition* GetDefinition()
	{
		return &OxygenComponentDef;
	}

private:
	FOxygenComponentDataChangedEvent MyComponentDataChangedEvent;
};
