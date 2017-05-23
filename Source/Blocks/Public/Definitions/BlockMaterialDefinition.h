#pragma once

#include "Commons/Public/Enums.h"
#include "BlockMaterialDefinition.generated.h"

/**
 *
 */
USTRUCT(BlueprintType)
struct FBlockMaterialDefinition
{
	GENERATED_BODY();

	UPROPERTY(EditDefaultsOnly, Category = "TCF2 | Block")
		UMaterial* DefaultMat;

	UPROPERTY(EditDefaultsOnly, Category = "TCF2 | Block")
		UMaterial* TranslucentMat;

	UPROPERTY(EditDefaultsOnly, Category = "TCF2 | Block")
		EApplyScaleToMaterialParam KX;

	UPROPERTY(EditDefaultsOnly, Category = "TCF2 | Block")
		EApplyScaleToMaterialParam KY;

	UPROPERTY(EditDefaultsOnly, Category = "TCF2 | Block")
		bool OverrideScaleApplyCustom;

	FBlockMaterialDefinition()
	{
		DefaultMat = NULL;
		TranslucentMat = NULL;
		KX = EApplyScaleToMaterialParam::None;
		KY = EApplyScaleToMaterialParam::None;
	}

	FVector2D GetParams(const FVector& scale) {
		FVector2D res;

		switch (KX) {
		case EApplyScaleToMaterialParam::X: res.X = scale.X; break;
		case EApplyScaleToMaterialParam::Y: res.X = scale.Y; break;
		case EApplyScaleToMaterialParam::Z: res.X = scale.Z; break;
		case EApplyScaleToMaterialParam::None: res.X = 0; break;
		default:
			checkNoEntry();
			break;
		}

		switch (KY) {
		case EApplyScaleToMaterialParam::X: res.Y = scale.X; break;
		case EApplyScaleToMaterialParam::Y: res.Y = scale.Y; break;
		case EApplyScaleToMaterialParam::Z: res.Y = scale.Z; break;
		case EApplyScaleToMaterialParam::None: res.Y = 0; break;
		default:
			checkNoEntry();
			break;
		}

		return res;
	}
};
