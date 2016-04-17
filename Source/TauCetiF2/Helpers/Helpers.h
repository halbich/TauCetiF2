

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

	static const FVector WorldBorders;
	static const FVector WorldCenterMapping;
	static const int32 CubeMinSize;


	UFUNCTION(BlueprintCallable, Category = TCF2Helpers)
		static TArray<FString> GetAllSaveGameSlots();

	UFUNCTION(BlueprintCallable, Category = TCF2Helpers)
		static UImage* GetImageForBlock(UBuildableBlockInfo* blockInfo);

	UFUNCTION(BlueprintCallable, Category = TCF2Helpers)
		static UTexture2D* GetTexture2DForBlock(UBuildableBlockInfo* blockInfo);


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
		if (Path == NAME_None) return NULL;
		//~

		return Cast<ObjClass>(StaticLoadObject(ObjClass::StaticClass(), NULL, *Path.ToString()));
	}



	static FORCEINLINE UMaterial* GetMaterialByName(const FName& name) {

		return LoadObjFromPath<UMaterial>(name);
	}

	static FORCEINLINE UImage* GetImageByName(const FName& name) {
		UE_LOG(LogTemp, Log, TEXT("%s"), *(name.ToString()));
		return LoadObjFromPath<UImage>(name);
	}

	static FORCEINLINE UTexture2D* GetTexture2DByName(const FName& name) {
		UE_LOG(LogTemp, Log, TEXT("%s"), *(name.ToString()));
		return LoadObjFromPath<UTexture2D>(name);
	}



	static FORCEINLINE FString GetMaterialName(const EMaterialInstance& instance, bool isConstruct = false) {
		const FString baseFolder = isConstruct ? TEXT("Material'/Game/Materials/BuildingObjects/%s") : TEXT("Material'/Game/Materials/BuildingObjects/Construction/%s");

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
		default:
			return TEXT("");
		}
	}


	static FORCEINLINE UMaterial* GetMaterialByInstance(const EMaterialInstance& instance, bool isConstruct = false) {
		auto name = GetMaterialName(instance, isConstruct);
		return name.Len() > 0 ? GetMaterialByName(*name) : nullptr;
	}

	static FORCEINLINE FVector BorderToWorld(const FVector& border)
	{
		return border - WorldCenterMapping;

	}

public:
	static FORCEINLINE FVector GetWorldCoordinate(const FVector& vect)
	{
		return FVector(vect) * CubeMinSize;
	}

	static FORCEINLINE FVector GetSpawnOffset(const FRotator& rotator, const FVector& size)
	{
		auto transMove = FVector((int32)(size.X + 1) % 2, (int32)(size.Y + 1) % 2, (int32)(size.Z + 1) % 2) * 0.5;
		auto rotatedVect = rotator.RotateVector(transMove);
		return rotatedVect;
	}

	static FORCEINLINE FVector GetSpawnCoords(const FVector& localPosition, const FVector& size, const FRotator& rotator)
	{
		return GetWorldCoordinate(localPosition + GetSpawnOffset(rotator, size));
	}

public:
	static FORCEINLINE FTransform GetSpawnTransform(const FBlockDefinition* definition, const FVector& localPosition, const FVector& size, const FRotator& rotator)
	{
		FTransform trans;
		trans.SetScale3D(definition->HasCustomScaling ? definition->MeshScale : size);
		trans.SetLocation(GetSpawnCoords(localPosition, definition->HasCustomScaling ? definition->WorldObjectScale : size, rotator));
		trans.SetRotation(FQuat(rotator));
		return trans;
	}

};
