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

	UFUNCTION(BlueprintCallable, Category = TCF2Helpers)
		static TArray<FString> GetAllSaveGameSlots();

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Change Localization"), Category = "Locale")
		static bool ChangeLocalization(FString target);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Get Current Localizations"), Category = "Locale")
		static void GetCurrentLocalizations(UPARAM(ref)TArray<FString>& DisplayNames, UPARAM(ref)TArray<FString>& IsoNames);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Get Current Culture"), Category = "Locale")
		static FString GetCurrentCultureIsoName();

	UFUNCTION(BlueprintCallable, Category = TCF2Helpers)
		static void FatalError(const FName text);

	// https://wiki.unrealengine.com/Dynamic_Load_Object
	template <typename ObjClass>
	static FORCEINLINE ObjClass* LoadObjFromPath(const FName& Path)
	{
		if (Path == NAME_None)
			return nullptr;

		return Cast<ObjClass>(StaticLoadObject(ObjClass::StaticClass(), NULL, *Path.ToString()));
	}

	static FORCEINLINE UMaterial* GetMaterialByName(const FName& name) {
		return LoadObjFromPath<UMaterial>(name);
	}

	static FORCEINLINE UImage* GetImageByName(const FName& name) {
		return LoadObjFromPath<UImage>(name);
	}

	static FORCEINLINE UTexture2D* GetTexture2DByName(const FName& name) {
		return LoadObjFromPath<UTexture2D>(name);
	}


	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "TCF2 | Game Definitions")
		static int32 GetCubeMinSize();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "TCF2 | Game Definitions")
		static int32 GetCubeMinSizeHalf();

	UFUNCTION(BlueprintCallable, Category = "TCF2 | Game Helpers")
		static void ObjectApplyLocalTrans(UStaticMeshComponent* comp, FVector loc, FRotator rot, FVector scal);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = BlockConstructorSelector)
		static TArray<UInventoryFlagItem*> GetBlockFlags(UBlockBaseInfo* blockBaseInfo);
};
