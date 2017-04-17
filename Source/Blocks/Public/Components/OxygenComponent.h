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

	UPROPERTY(BlueprintReadOnly, Transient, Category = "TCF2 | OxygenComponent")
		bool IsInCreative;


	UPROPERTY(Transient)
		float OxygenProduced;

	UPROPERTY(Transient)
		float OxygenConsumed;

	UPROPERTY(Transient)
		UBlockInfo* BlockInfo;

	void onComponentDataChanged();

public:

	UBlockWithOxygenInfo* SetInfo(UBlockWithOxygenInfo* info);

	void SetDefinition(FOxygenComponentDefinition def);

	void SetDefinition(FOxygenComponentDefinition def, FVector& blockScale, FRotator& blockRotation);

	UFUNCTION(BlueprintCallable, Category = "TCF2 | OxygenComponent")
		bool ObtainAmount(float requested, float& actuallyObtained, bool requireExact = false);

	UFUNCTION(BlueprintCallable, Category = "TCF2 | OxygenComponent")
		bool PutAmount(float aviable, float& actuallyPutted);

	void ToggleIsInCreative(bool newInCreative);


	friend UBlockInfo* GetBlockInfoFromParent(UActorComponent* source);

	UBlockInfo* GetBlockInfo();


public:
	FORCEINLINE const FOxygenComponentDefinition* GetDefinition()
	{
		return &OxygenComponentDef;
	}

	FORCEINLINE const UBlockWithOxygenInfo* GetInfo()
	{
		return OxygenInfo;
	}

	UPROPERTY(BlueprintAssignable, Category = "TCF2 | OxygenComponent")
		FOxygenComponentDataChangedDelegate OnComponentDataChangedEvent;

	FCriticalSection InOutCritical;
};
