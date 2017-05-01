#pragma once

#include "SlateBasics.h"
#include "Runtime/UMG/Public/UMG.h"
#include "Runtime/UMG/Public/Blueprint/UserWidget.h"
#include "Runtime/UMG/Public/Components/Widget.h"
#include "Runtime/UMG/Public/Components/Image.h"
#include "Blocks/Public/Info/BlockBaseInfo.h"
#include "Blocks/Public/Info/BuildableBlockInfo.h"
#include "Widgets/Common/InventoryFlagItem.h"
#include "Game/Weather/WeatherDefinition.h"
#include "Helpers.generated.h"

USTRUCT(BlueprintType)
struct TAUCETIF2_API FControlsHelperStruct
{
	GENERATED_BODY();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = ControlsHelperStruct)
		FText PrimaryControl;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = ControlsHelperStruct)
		FText SecondaryControl;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = ControlsHelperStruct)
		FText Description;
};


USTRUCT(BlueprintType)
struct TAUCETIF2_API FControlsHelperStructGroup
{

	GENERATED_BODY();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = ControlsHelperStruct)
		FText SectionName;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = ControlsHelperStruct)
		TArray<FControlsHelperStruct> Controls;
};

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

	UFUNCTION(BlueprintCallable, Category = "TCF2 | Game Helpers")
		static void FatalError(const FName text);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "TCF2 | Game Definitions")
		static int32 GetCubeMinSize();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "TCF2 | Game Definitions")
		static int32 GetCubeMinSizeHalf();

	UFUNCTION(BlueprintCallable, Category = "TCF2 | Game Helpers")
		static void ObjectApplyLocalTrans(UStaticMeshComponent* comp, FVector loc, FRotator rot, FVector scal);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "TCF2 | Game Helpers", meta = (WorldContext = "WorldContextObject"))
		static TArray<UInventoryFlagItem*> GetBlockFlags(UObject* WorldContextObject, UBlockBaseInfo* blockBaseInfo);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "TCF2 | Game Helpers")
		static FDateTime GetTimeFromSeconds(float seconds);

	UFUNCTION(BlueprintCallable, Category = "TCF2 | Game Helpers")
		static void GetSliderValuesForObject(UBuildableBlockInfo* buildableInfo, FVector Limit, bool IsSaveEnabled, FVector& Min, FVector& Max);

	UFUNCTION(BlueprintCallable, Category = "TCF2 | Game Helpers | Sound")
		static bool GetSoundClassVolume(USoundClass* TargetClass, float& resultValue);

	UFUNCTION(BlueprintCallable, Category = "TCF2 | Game Helpers | Sound")
		static bool SetSoundClassVolume(USoundClass* TargetClass, float targetValue);

	UFUNCTION(BlueprintCallable, Category = "TCF2 | Game Helpers | Settings")
		static FWeatherDefinition GetRandomDefinition(UPARAM(ref)TArray<FWeatherDefinition>& aviableDefinitions, int32& outIndex);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "TCF2 | Game Helpers")
		static float GetGameDayLength();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "TCF2 | Game Helpers")
		static float GetGameDayMultiplier();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "TCF2 | Game Helpers")
		static TArray<FControlsHelperStructGroup> GetControlsList();
};
