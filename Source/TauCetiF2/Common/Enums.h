

#pragma once

UENUM(BlueprintType)
enum class EShapeType : uint8
{
	Empty	 	UMETA(DisplayName = "Empty"),
	Cube 		UMETA(DisplayName = "Cube"),
	CubeSide	UMETA(DisplayName = "CubeSide"),
	CubeBody	UMETA(DisplayName = "CubeBody"),
	Custom		UMETA(DisplayName = "Custom"),
};

UENUM(BlueprintType)
enum class EBlockType : uint8
{
	Empty	 	UMETA(DisplayName = "Empty"),
	Base		UMETA(DisplayName = "Base"),
	Building	UMETA(DisplayName = "Building"),
	Custom		UMETA(DisplayName = "Custom"),
};


/*
	Enum for block definition
*/
UENUM(BlueprintType)
enum class EMaterialType : uint8
{
	Empty	 		UMETA(DisplayName = "Empty"),
	Polycarbonate	UMETA(DisplayName = "Polycarbonate"),
	Steel			UMETA(DisplayName = "Steel"),
	Alloy			UMETA(DisplayName = "Alloy"),
	SuperAlloy		UMETA(DisplayName = "SuperAlloy"),
};


/*
	Enum for asset references
*/
UENUM(BlueprintType)
enum class EMaterialInstance : uint8
{
	Empty	 				UMETA(DisplayName = "Empty"),
	InnerMaterial	 		UMETA(DisplayName = "InnerMaterial"),
	BaseFloor				UMETA(DisplayName = "BaseFloor"),
	BaseSide				UMETA(DisplayName = "BaseSide"),
	Polycarbonate			UMETA(DisplayName = "Polycarbonate"),
	ConstructRectangle		UMETA(DisplayName = "ConstructRectangle"),
	ConstructRectangleBody	UMETA(DisplayName = "ConstructRectangleBody"),
	ConstructTriangle		UMETA(DisplayName = "ConstructTriangle"),
	TerminalScreen			UMETA(DisplayName = "TerminalScreen"),
	TerminalBase			UMETA(DisplayName = "TerminalBase"),
};


UENUM(BlueprintType)
enum class EBlockName : uint8
{
	Empty						UMETA(DisplayName = "Empty"),
	ConstructCube 				UMETA(DisplayName = "ConstructCube"),
	BaseCube 					UMETA(DisplayName = "BaseCube"),
	WindowCube 					UMETA(DisplayName = "WindowCube"),
	ConstructCubeSide 			UMETA(DisplayName = "ConstructCubeSide"),
	BaseRamp 					UMETA(DisplayName = "BaseRamp"),
	ConstructCubeBody 			UMETA(DisplayName = "ConstructCubeBody"),
	Terminal		 			UMETA(DisplayName = "Terminal"),
};


static FString GetEBlockNameAsString(int32 EnumValue)
{
	const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("EBlockName"), true);
	if (!EnumPtr) return TEXT("Invalid");

	return *EnumPtr->GetDisplayNameText(EnumValue).ToString();
}

