

#pragma once

#include "Common/SaveGameCarrier.h"
#include "SlateBasics.h"
#include "Runtime/UMG/Public/UMG.h"
#include "Runtime/UMG/Public/Blueprint/UserWidget.h"
#include "Runtime/UMG/Public/Components/Widget.h"
#include "Runtime/UMG/Public/Components/Image.h"
#include "Blocks/Definitions/FBlockDefinition.h"

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

	UFUNCTION(BlueprintCallable, Category = TCF2Helpers)
		static UImage* GetImageForBlock(UBuildableBlockInfo* blockInfo);

	UFUNCTION(BlueprintCallable, Category = TCF2Helpers)
		static UTexture2D* GetTexture2DForBlock(UBuildableBlockInfo* blockInfo);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Change Localization"), Category = "Locale")
		static bool ChangeLocalization(FString target);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Get Current Localizations"), Category = "Locale")
		static void GetCurrentLocalizations(UPARAM(ref)TArray<FString>& DisplayNames, UPARAM(ref)TArray<FString>& IsoNames);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Get Current Culture"), Category = "Locale")
		static FString GetCurrentCultureIsoName();

	UFUNCTION(BlueprintCallable, Category = TCF2Helpers)
		static void FatalError(const FName text);

	static FORCEINLINE FString UHelpers::GetCleanSaveFileName(const FString& worldName, const FDateTime& saveTime) {
		auto name = worldName.Replace(TEXT(" "), TEXT("_"));
		name.ToLowerInline();
		auto date = saveTime.ToString(TEXT("%d_%m_%Y_%H_%M_%S"));

		return FString::Printf(TEXT("%s_%s"), *name, *date);
	}

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



	/*static FORCEINLINE FString GetMaterialName(const EMaterialInstance& instance, bool isConstruct = false) {
		const FString baseFolder = isConstruct ? TEXT("Material'/Game/Materials/BuildingObjects/Construction/%s") : TEXT("Material'/Game/Materials/BuildingObjects/%s");

		switch (instance)
		{
		case EMaterialInstance::Empty: return TEXT("Material'/Engine/EngineMaterials/WorldGridMaterial.WorldGridMaterial'");
		case EMaterialInstance::InnerMaterial: return FString::Printf(*baseFolder, TEXT("InsideMat.InsideMat'"));
		case EMaterialInstance::BaseFloor: return FString::Printf(*baseFolder, TEXT("MaterialBaseFloor.MaterialBaseFloor'"));
		case EMaterialInstance::BaseSide: return FString::Printf(*baseFolder, TEXT("MaterialBaseSide.MaterialBaseSide'"));
		case EMaterialInstance::Polycarbonate: return FString::Printf(*baseFolder, TEXT("MaterialPolycarbonate.MaterialPolycarbonate'"));
		case EMaterialInstance::ConstructRectangle: return FString::Printf(*baseFolder, TEXT("MaterialRectangle.MaterialRectangle'"));
		case EMaterialInstance::ConstructRectangleBody: return FString::Printf(*baseFolder, TEXT("MaterialRectangleBody.MaterialRectangleBody'"));
		case EMaterialInstance::ConstructTriangle: return FString::Printf(*baseFolder, TEXT("MaterialTriangle.MaterialTriangle'"));
		case EMaterialInstance::TerminalScreen: return FString::Printf(*baseFolder, TEXT("TerminalScreen.TerminalScreen'"));
		case EMaterialInstance::TerminalBase: return FString::Printf(*baseFolder, TEXT("TerminalBase.TerminalBase'"));
		case EMaterialInstance::DoorFrame: return FString::Printf(*baseFolder, TEXT("DoorFrame.DoorFrame'"));
		case EMaterialInstance::DoorBody: return FString::Printf(*baseFolder, TEXT("DoorBody.DoorBody'"));
		case EMaterialInstance::DoorCircle: return FString::Printf(*baseFolder, TEXT("DoorCircle.DoorCircle'"));
		default:
			return TEXT("");
		}
	}*/


	//static FORCEINLINE UMaterial* GetMaterialByInstance(const EMaterialInstance& instance, bool isConstruct = false) {
	//	auto name = GetMaterialName(instance, isConstruct);
	//	return name.Len() > 0 ? GetMaterialByName(*name) : nullptr;
	//}





};
