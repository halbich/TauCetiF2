#pragma once

#include "Components/ActorComponent.h"
#include "Info/Components/BlockWithElectricityInfo.h"
#include "Definitions/ElectricityComponentDefinition.h"
#include "Info/ElectricityBindableAreaInfo.h"
#include "Commons/Public/Enums.h"
#include "ElectricityComponent.generated.h"

class UElectricNetwork;

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

	UPROPERTY(Transient)
		UElectricNetwork* Network;

	UPROPERTY(Transient)
		EElectricNetworkState ComponentNetworkState;

	UPROPERTY(Transient)
		EHealthSeverity HealthSeverity;

	UPROPERTY(Transient)
		float EnergyProduced;

	UPROPERTY(Transient)
		float EnergyConsumed;

	UPROPERTY(Transient)
		UBlockInfo* BlockInfo;

	void onComponentDataChanged();

	friend TArray<UElectricityComponent*> GetSurroundingComponents(UElectricityComponent* source);

	friend UBlockInfo* GetBlockInfoFromParent(UActorComponent* source);

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

	void UpdateHealthSeverity();

	UBlockInfo* GetBlockInfo();

public:
	FORCEINLINE const FElectricityComponentDefinition* GetDefinition()
	{
		return &ElectricityComponentDef;
	}

	FORCEINLINE const UBlockWithElectricityInfo* GetInfo()
	{
		return ElectricityInfo;
	}

	UPROPERTY(BlueprintAssignable, Category = "TCF2 | ElectricityComponent")
		FElectricityComponentDataChangedDelegate OnComponentDataChangedEvent;

private:

	static bool isValidConnection(UElectricityBindableAreaInfo* area, UElectricityBindableAreaInfo* otherArea);

	static float IntersectVolume(const FBox& box, const FBox& other)
	{
		auto tmin = FVector(FMath::Max(box.Min.X, other.Min.X), FMath::Max(box.Min.Y, other.Min.Y), FMath::Max(box.Min.Z, other.Min.Z));
		auto tmax = FVector(FMath::Min(box.Max.X, other.Max.X), FMath::Min(box.Max.Y, other.Max.Y), FMath::Min(box.Max.Z, other.Max.Z));

		FBox bx(tmin, tmax);

		if (bx.IsValid == 0)
			return 0;

		return bx.GetVolume();
	}

	FCriticalSection InOutCritical;
};
