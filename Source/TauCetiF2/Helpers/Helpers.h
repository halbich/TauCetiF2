#pragma once

#include "GameSave/Public/SaveGameCarrier.h"
#include "SlateBasics.h"
#include "Runtime/UMG/Public/UMG.h"
#include "Runtime/UMG/Public/Blueprint/UserWidget.h"
#include "Runtime/UMG/Public/Components/Widget.h"
#include "Runtime/UMG/Public/Components/Image.h"
#include "Widgets/Common/InventoryFlagItem.h"
#include "Helpers.generated.h"

/**
 *
 */
UCLASS()
class TAUCETIF2_API UHelpers : public UObject
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Change Localization"), Category = "Locale")
		static bool ChangeLocalization(FString target);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Get Current Localizations"), Category = "Locale")
		static void GetCurrentLocalizations(UPARAM(ref)TArray<FString>& DisplayNames, UPARAM(ref)TArray<FString>& IsoNames);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Get Current Culture"), Category = "Locale")
		static FString GetCurrentCultureIsoName();

	UFUNCTION(BlueprintCallable, Category = TCF2Helpers)
		static void FatalError(const FName text);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "TCF2 | Game Definitions")
		static int32 GetCubeMinSize();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "TCF2 | Game Definitions")
		static int32 GetCubeMinSizeHalf();

	UFUNCTION(BlueprintCallable, Category = "TCF2 | Game Helpers")
		static void ObjectApplyLocalTrans(UStaticMeshComponent* comp, FVector loc, FRotator rot, FVector scal);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "TCF2 | Game Helpers")
		static TArray<UInventoryFlagItem*> GetBlockFlags(UBlockBaseInfo* blockBaseInfo);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "TCF2 | Game Helpers")
		static FDateTime GetTimeFromSeconds(float seconds);

	UFUNCTION(BlueprintCallable, Category = "TCF2 | Game Helpers")
		static void GetSliderValuesForObject(UBuildableBlockInfo* buildableInfo, bool IsSaveEnabled, FVector& Min, FVector& Max);
};
