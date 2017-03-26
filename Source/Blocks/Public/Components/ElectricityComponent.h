#pragma once

#include "Components/ActorComponent.h"
#include "Info/Components/BlockWithElectricityInfo.h"
#include "Definitions/ElectricityComponentDefinition.h"
#include "Info/ElectricityBindableAreaInfo.h"
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

	UPROPERTY(BlueprintReadOnly, Transient, Category = "TCF2 | ElectricityComponent")
		FElectricityComponentDefinition ElectricityComponentDef;

	UPROPERTY(BlueprintReadOnly, Transient, Category = "TCF2 | ElectricityComponent")
		bool IsInCreative;

	UPROPERTY(BlueprintReadOnly, Transient, Category = "TCF2 | ElectricityComponent")
		TArray<UElectricityBindableAreaInfo*> ElectricityBindableAreas;


	UPROPERTY(BlueprintReadOnly, Transient, Category = "TCF2 | ElectricityComponent")
		TArray<UElectricityComponent*> ConnectedComponents;

	void onComponentDataChanged();

	friend TArray<UElectricityComponent*> GetSurroundingComponents(UElectricityComponent* source);

public:

	UBlockWithElectricityInfo* SetInfo(UBlockWithElectricityInfo* info);

	void SetDefinition(FElectricityComponentDefinition def);

	void SetDefinition(FElectricityComponentDefinition def, FVector& blockScale, FRotator& blockRotation);

	UFUNCTION(BlueprintCallable, Category = "TCF2 | ElectricityComponent")
		bool ObtainAmount(float requested, float& actuallyObtained, bool requireExact = false);

	UFUNCTION(BlueprintCallable, Category = "TCF2 | ElectricityComponent")
		bool PutAmount(float aviable, float& actuallyPutted);

	void ToggleIsInCreative(bool newInCreative);

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	FORCEINLINE const FElectricityComponentDefinition* GetDefinition()
	{
		return &ElectricityComponentDef;
	}

	UPROPERTY(BlueprintAssignable, Category = "TCF2 | ElectricityComponent")
		FElectricityComponentDataChangedDelegate OnComponentDataChangedEvent;

private:

	static bool isValidConnection(UElectricityBindableAreaInfo* area, UElectricityBindableAreaInfo* otherArea);
};
