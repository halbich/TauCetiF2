#pragma once

#include "Components/ActorComponent.h"
#include "Info/Components/BlockWithElectricityInfo.h"
#include "Definitions/ElectricityComponentDefinition.h"
#include "ElectricityComponent.generated.h"

DECLARE_DELEGATE_OneParam(FElectricityComponentDataChangedDelegate, UBlockWithElectricityInfo*);
DECLARE_EVENT_OneParam(UElectricityComponent, FElectricityComponentDataChangedEvent, UBlockWithElectricityInfo*);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class BLOCKS_API UElectricityComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UElectricityComponent();

private:
	UPROPERTY(Transient)
		UBlockWithElectricityInfo* ElectricityInfo;

	UPROPERTY(Transient)
		FElectricityComponentDefinition ElectricityComponentDef;

	void OnComponentDataChanged();

public:

	UBlockWithElectricityInfo* SetInfo(UBlockWithElectricityInfo* info);

	void SetDefinition(FElectricityComponentDefinition def);

public:
	FDelegateHandle AddEventListener(FElectricityComponentDataChangedDelegate& dataChangedDelegate);
	void RemoveEventListener(FDelegateHandle DelegateHandle);
private:
	FElectricityComponentDataChangedEvent MyComponentDataChangedEvent;
};
