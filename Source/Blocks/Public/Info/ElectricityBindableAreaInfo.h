#pragma once

#include "UObject/NoExportTypes.h"
#include "Definitions/ElectricityBindableAreas.h"
#include "Commons/Public/GameDefinitions.h"
#include "ElectricityBindableAreaInfo.generated.h"

/**
 *
 */
UCLASS()
class BLOCKS_API UElectricityBindableAreaInfo : public UObject
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadOnly, Transient, Category = "TCF2 | ElectricityBindableAreaInfo")
		TArray<FVector> AreaPoints;

	UPROPERTY(BlueprintReadOnly, Transient, Category = "TCF2 | ElectricityBindableAreaInfo")
		FVector DominantPlane;

	void InitArea(TArray<FVector> usedPoints, FElectricityBindableArea& definedPlane, FVector& blockScale, FRotator& blockRotation, FVector worldLocation);

	void DEBUG_DrawPoints(UWorld* world);

	FBox GetBox();
};
