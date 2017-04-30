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

UENUM(BlueprintType)
enum class EInventoryGroupType : uint8
{
	Building	 	UMETA(DisplayName = "Building"),
	Inventory		UMETA(DisplayName = "Inventory")
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
	MainMenu,
	Inventory,
	Registered
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
	MiniLevel,
	EmptyLevel,
	HardcodedLevelsMax
};

UENUM(BlueprintType)
enum class EApplyScaleToMaterialParam : uint8
{
	None	 	UMETA(DisplayName = "None"),
	X			UMETA(DisplayName = "X"),
	Y			UMETA(DisplayName = "Y"),
	Z			UMETA(DisplayName = "Z")
};

UENUM(BlueprintType)
enum class EGameUserSettingsVariable : uint8
{
	None	 			UMETA(DisplayName = "None"),
	MasterVolume		UMETA(DisplayName = "MasterVolume"),
	MusicVolume			UMETA(DisplayName = "MusicVolume"),
	OtherVolume			UMETA(DisplayName = "OtherVolume"),
	GeneratorVisualizationEnabled		UMETA(DisplayeName = "GeneratorVisualizationEnabled")
};

UENUM(BlueprintType)
enum class EElectricNetworkState : uint8
{
	Invalid	 		UMETA(DisplayName = "Invalid"),
	InRecompute		UMETA(DisplayName = "InRecompute"),
	Valid			UMETA(DisplayName = "Valid")
};

UENUM(BlueprintType)
enum class EHealthSeverity : uint8
{
	OK	 			UMETA(DisplayName = "OK"),
	ToRepair		UMETA(DisplayName = "ToRepair"),
	Important		UMETA(DisplayName = "Important"),
	Critical		UMETA(DisplayName = "Critical"),
	Dead			UMETA(DisplayName = "Dead")
};

UENUM(BlueprintType)
enum class EControlRelationship : uint8
{
	IsControlledByTarget	 	UMETA(DisplayName = "IsControlledByTarget"),
	IsControllingTarget			UMETA(DisplayName = "IsControllingTarget")
};

UENUM(BlueprintType)
enum class EStormNotification : uint8
{
	StormIsComming	 	UMETA(DisplayName = "StormIsComming"),
	StormBegin			UMETA(DisplayName = "StormBegin"),
	StormEnd			UMETA(DisplayName = "StormEnd")
};