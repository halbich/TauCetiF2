

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
	DoorFrame				UMETA(DisplayName = "DoorFrame"),
	DoorCircle				UMETA(DisplayName = "DoorCircle"),
	DoorBody				UMETA(DisplayName = "DoorBody"),
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
	Door						UMETA(DisplayName = "Door"),
	WindowPowered				UMETA(DisplayName = "WindowPowered"),
};

#if WITH_EDITOR
static FString GetEBlockNameAsString(int32 EnumValue)
{
	const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("EBlockName"), true);
	if (!EnumPtr) return TEXT("Invalid");

	return *EnumPtr->GetDisplayNameText(EnumValue).ToString();
}
#endif

enum class EDoorOpening : uint8
{
	Left,
	Right

};



UENUM(BlueprintType)
enum class EBuildableObjectAction : uint8
{
	None						UMETA(DisplayName = "None"),
	ConstructObject				UMETA(DisplayName = "ConstructObject"),
	DeleteObject				UMETA(DisplayName = "DeleteObject"),
};

UENUM(BlueprintType)
enum class EShownWidget : uint8
{
	None,
	InGameMenu,
	BaseControl,
	MainMenu,
	Inventory

};


UENUM(BlueprintType)
enum class EModalDialogResult : uint8
{
	None,
	Yes,
	No,
	OK,
	Cancel

};

UENUM(BlueprintType)
enum class ENamedHardcodedLevel : uint8
{
	MainMenu,
	DefaultLevel,
	EmptyLevel,
	HardcodedLevelsMax

};


UENUM(BlueprintType)
enum class ENamedTag : uint8
{
	DoorOpening
};

static FString GetNameForTag(ENamedTag tag)
{
	switch (tag)
	{
	case ENamedTag::DoorOpening: return TEXT("doorOpening");
	default:
		checkNoEntry();
		return TEXT("");
	}
}

