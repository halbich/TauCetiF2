

#pragma once

#include "Common/SaveGameCarrier.h"
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

	static FORCEINLINE UBlueprint* GetBlueprintByShape(const EShapeType shape) {

		FName name;
		switch (shape)
		{
		case EShapeType::Cube: name = TEXT("Blueprint'/Game/BuildingObjects/Shapes/ShapeCube.ShapeCube'"); break;
		case EShapeType::CubeSide: name = TEXT("Blueprint'/Game/BuildingObjects/Shapes/ShapeCubeSide.ShapeCubeSide'"); break;
		case EShapeType::CubeBody: name = TEXT("Blueprint'/Game/BuildingObjects/Shapes/ShapeCubeBody.ShapeCubeBody'"); break;
		default:
			return nullptr;
			break;
		}

		return LoadObjFromPath<UBlueprint>(name);
	}

	static FORCEINLINE FVector GetWorldCoordinate(const FVector& vect)
	{
		return FVector(vect) * 20;
	}


	static FORCEINLINE FTransform GetSpawnTransform(const FVector& localPosition, const FVector& size)
	{
		FTransform trans;
		trans.SetScale3D(size);
		auto transMove = FVector( (int32)(size.X + 1)% 2, (int32)(size.Y + 1) % 2, (int32)(size.Z + 1) % 2) * 0.5;
		trans.SetLocation(GetWorldCoordinate(localPosition + transMove));
		return trans;
	}
};
