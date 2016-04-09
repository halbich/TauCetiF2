

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


