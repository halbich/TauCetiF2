

#include "TauCetiF2.h"
#include "Game/Localization/LocalizationHelpers.h"
#include "FBlockDefinitionHolder.h"



FBlockDefinitionHolder& FBlockDefinitionHolder::Instance()
{

	static	FBlockDefinitionHolder instance;
	return instance;
}



FBlockDefinitionHolder::FBlockDefinitionHolder()
{


	FBlockDefinition cubeBuilding(EBlockName::ConstructCube, EShapeType::Cube);
	cubeBuilding.AddMaterials(3,
		FBlockMaterialDefinition(EMaterialInstance::ConstructRectangle, EAxis::Type::X, EAxis::Type::Y),
		FBlockMaterialDefinition(EMaterialInstance::ConstructRectangle, EAxis::Type::X, EAxis::Type::Z),
		FBlockMaterialDefinition(EMaterialInstance::ConstructRectangle, EAxis::Type::Y, EAxis::Type::Z));
	cubeBuilding.BuilderSortIndex = 500;
	definitions.Add(cubeBuilding.ID, cubeBuilding);




	FBlockDefinition cubeBase(EBlockName::BaseCube, EShapeType::Cube);
	cubeBase.AddMaterials(3,
		FBlockMaterialDefinition(EMaterialInstance::BaseFloor, EAxis::Type::X, EAxis::Type::Y),
		FBlockMaterialDefinition(EMaterialInstance::BaseSide, EAxis::Type::X, EAxis::Type::Z),
		FBlockMaterialDefinition(EMaterialInstance::BaseSide, EAxis::Type::Y, EAxis::Type::Z));
	cubeBase.BuilderSortIndex = 600;
	cubeBase.MinBlockScale.Z = cubeBase.MaxBlockScale.Z = 4;
	cubeBase.AllowPitch = false;
	cubeBase.AllowRoll = false;
	definitions.Add(cubeBase.ID, cubeBase);




	FBlockDefinition cubeWindow(EBlockName::WindowCube, EShapeType::Cube);
	cubeWindow.AddMaterials(3,
		FBlockMaterialDefinition(EMaterialInstance::Polycarbonate, EAxis::Type::X, EAxis::Type::Y, NAME_None, true),
		FBlockMaterialDefinition(EMaterialInstance::Polycarbonate, EAxis::Type::X, EAxis::Type::Z, NAME_None, true),
		FBlockMaterialDefinition(EMaterialInstance::Polycarbonate, EAxis::Type::Y, EAxis::Type::Z, NAME_None, true));
	cubeWindow.BuilderSortIndex = 700;
	definitions.Add(cubeWindow.ID, cubeWindow);





	FBlockDefinition constructCubeSide(EBlockName::ConstructCubeSide, EShapeType::CubeSide);
	constructCubeSide.AddMaterials(5,
		FBlockMaterialDefinition(EMaterialInstance::ConstructRectangle, EAxis::Type::X, EAxis::Type::Z),
		FBlockMaterialDefinition(EMaterialInstance::ConstructRectangleBody, EAxis::Type::X, EAxis::Type::Y),
		FBlockMaterialDefinition(EMaterialInstance::ConstructRectangle, EAxis::Type::X, EAxis::Type::Y),
		FBlockMaterialDefinition(EMaterialInstance::ConstructTriangle, EAxis::Type::Z, EAxis::Type::Y),
		FBlockMaterialDefinition(EMaterialInstance::ConstructTriangle, EAxis::Type::Y, EAxis::Type::Z));
	constructCubeSide.BuilderSortIndex = 510;
	definitions.Add(constructCubeSide.ID, constructCubeSide);




	FBlockDefinition baseRamp(EBlockName::BaseRamp, EShapeType::CubeSide);
	baseRamp.AddMaterials(5,
		FBlockMaterialDefinition(EMaterialInstance::BaseSide, EAxis::Type::X, EAxis::Type::Z),
		FBlockMaterialDefinition(EMaterialInstance::BaseFloor, EAxis::Type::X, EAxis::Type::Y),
		FBlockMaterialDefinition(EMaterialInstance::BaseSide, EAxis::Type::X, EAxis::Type::Y),
		FBlockMaterialDefinition(EMaterialInstance::BaseSide, EAxis::Type::Z, EAxis::Type::Y),
		FBlockMaterialDefinition(EMaterialInstance::BaseSide, EAxis::Type::Y, EAxis::Type::Z));
	baseRamp.BuilderSortIndex = 610;
	baseRamp.MinBlockScale.Z = baseRamp.MaxBlockScale.Z = 4;
	baseRamp.AllowPitch = false;
	baseRamp.AllowRoll = false;
	definitions.Add(baseRamp.ID, baseRamp);




	FBlockDefinition constructCubeBody(EBlockName::ConstructCubeBody, EShapeType::CubeBody);
	constructCubeBody.AddMaterials(4,
		FBlockMaterialDefinition(EMaterialInstance::ConstructTriangle, EAxis::Type::Z, EAxis::Type::X),
		FBlockMaterialDefinition(EMaterialInstance::ConstructTriangle, EAxis::Type::Y, EAxis::Type::Z),
		FBlockMaterialDefinition(EMaterialInstance::ConstructTriangle, EAxis::Type::X, EAxis::Type::Y),
		FBlockMaterialDefinition(EMaterialInstance::ConstructTriangle, EAxis::Type::None, EAxis::Type::None));
	constructCubeBody.BuilderSortIndex = 520;
	definitions.Add(constructCubeBody.ID, constructCubeBody);



	FBlockDefinition terminal(EBlockName::Terminal, EShapeType::Custom);
	terminal.AddMaterials(2,
		FBlockMaterialDefinition(EMaterialInstance::TerminalBase, EAxis::Type::X, EAxis::Type::X),
		FBlockMaterialDefinition(EMaterialInstance::TerminalScreen, EAxis::Type::X, EAxis::Type::X, NAME_None, true));
	terminal.SetCustomScale(FVector(1, 1, 1), FVector(1, 8, 5));
	terminal.AllowPitch = false;
	terminal.AllowRoll = false;
	terminal.BuilderSortIndex = 900;
	definitions.Add(terminal.ID, terminal);


	FBlockDefinition door(EBlockName::Door, EShapeType::Custom);
	door.AddMaterials(4,
		FBlockMaterialDefinition(EMaterialInstance::DoorFrame, EAxis::Type::X, EAxis::Type::Y, FName(TEXT("doorFrame"))),
		FBlockMaterialDefinition(EMaterialInstance::Polycarbonate, EAxis::Type::X, EAxis::Type::X, FName(TEXT("door")), true),
		FBlockMaterialDefinition(EMaterialInstance::DoorCircle, EAxis::Type::X, EAxis::Type::Y, FName(TEXT("door"))),
		FBlockMaterialDefinition(EMaterialInstance::DoorBody, EAxis::Type::X, EAxis::Type::Y, FName(TEXT("door")))
		);
	door.SetCustomScale(FVector(1, 1, 1), FVector(7, 7, 11));
	door.AllowPitch = false;
	door.AllowRoll = false;
	door.BuilderSortIndex = 800;

	FBlockAdditionalFlagsDefinition doorAddFlags(ENamedTag::DoorOpening, LC_DoorOpening);
	doorAddFlags.SetValues(2,
		FBlockFlagValue((int32)EDoorOpening::Left, LC_DoorOpeningLeft),
		FBlockFlagValue((int32)EDoorOpening::Right, LC_DoorOpeningRight)
		);

	door.AdditionalFlags.Add(doorAddFlags);
	definitions.Add(door.ID, door);

	// TODO
	FBlockDefinition windowPowered(EBlockName::WindowPowered, EShapeType::Custom);
	/*windowPowered.AddMaterials(4,
		FBlockMaterialDefinition(EMaterialInstance::DoorFrame, EAxis::Type::X, EAxis::Type::Y, FName(TEXT("doorFrame"))),
		FBlockMaterialDefinition(EMaterialInstance::Polycarbonate, EAxis::Type::X, EAxis::Type::X, FName(TEXT("door")), true),
		FBlockMaterialDefinition(EMaterialInstance::DoorCircle, EAxis::Type::X, EAxis::Type::Y, FName(TEXT("door"))),
		FBlockMaterialDefinition(EMaterialInstance::DoorBody, EAxis::Type::X, EAxis::Type::Y, FName(TEXT("door")))
		);*/
	windowPowered.MinBlockScale = FVector(3, 1, 3);
	windowPowered.MaxBlockScale = FVector(20, 1, 20);
	windowPowered.BuilderSortIndex = 720;
	definitions.Add(windowPowered.ID, windowPowered);

}


void FBlockDefinitionHolder::GetAllDefinitions(TArray<int32>& outArray)
{
	definitions.ValueSort([](const FBlockDefinition& A, const FBlockDefinition& B) {
		return A.BuilderSortIndex < B.BuilderSortIndex;
	});
	for (auto& Elem : definitions)
	{
		outArray.Add(Elem.Key);
	}
}