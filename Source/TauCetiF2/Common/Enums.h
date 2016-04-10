

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
enum class EMaterialType : uint8
{
	Empty	 		UMETA(DisplayName = "Empty"),
	Polycarbon		UMETA(DisplayName = "Polycarbon"),
	Steel			UMETA(DisplayName = "Steel"),
	Alloy			UMETA(DisplayName = "Alloy"),
	SuperAlloy		UMETA(DisplayName = "SuperAlloy"),
};


