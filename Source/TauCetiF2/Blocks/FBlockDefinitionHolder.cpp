

#include "TauCetiF2.h"
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

	definitions.Add(cubeBuilding.ID, cubeBuilding);


	FBlockDefinition cubeBase(EBlockName::BaseCube, EShapeType::Cube);
	cubeBase.AddMaterials(3,
		FBlockMaterialDefinition(EMaterialInstance::BaseFloor, EAxis::Type::X, EAxis::Type::Y),
		FBlockMaterialDefinition(EMaterialInstance::BaseSide, EAxis::Type::X, EAxis::Type::Z),
		FBlockMaterialDefinition(EMaterialInstance::BaseSide, EAxis::Type::Y, EAxis::Type::Z));

	definitions.Add(cubeBase.ID, cubeBase);


	FBlockDefinition cubeWindow(EBlockName::WindowCube, EShapeType::Cube);
	cubeWindow.AddMaterials(3,
		FBlockMaterialDefinition(EMaterialInstance::Polycarbonate, EAxis::Type::X, EAxis::Type::Y, true),
		FBlockMaterialDefinition(EMaterialInstance::Polycarbonate, EAxis::Type::X, EAxis::Type::Z, true),
		FBlockMaterialDefinition(EMaterialInstance::Polycarbonate, EAxis::Type::Y, EAxis::Type::Z, true));

	definitions.Add(cubeWindow.ID, cubeWindow);



	FBlockDefinition constructCubeSide(EBlockName::ConstructCubeSide, EShapeType::CubeSide);
	constructCubeSide.AddMaterials(5,
		FBlockMaterialDefinition(EMaterialInstance::ConstructRectangle, EAxis::Type::X, EAxis::Type::Z),
		FBlockMaterialDefinition(EMaterialInstance::ConstructRectangleBody, EAxis::Type::X, EAxis::Type::Y),
		FBlockMaterialDefinition(EMaterialInstance::ConstructRectangle, EAxis::Type::X, EAxis::Type::Y),
		FBlockMaterialDefinition(EMaterialInstance::ConstructTriangle, EAxis::Type::Z, EAxis::Type::Y),
		FBlockMaterialDefinition(EMaterialInstance::ConstructTriangle, EAxis::Type::Y, EAxis::Type::Z));

	definitions.Add(constructCubeSide.ID, constructCubeSide);

	FBlockDefinition baseRamp(EBlockName::BaseRamp, EShapeType::CubeSide);
	baseRamp.AddMaterials(5,
		FBlockMaterialDefinition(EMaterialInstance::BaseSide, EAxis::Type::X, EAxis::Type::Z),
		FBlockMaterialDefinition(EMaterialInstance::BaseFloor, EAxis::Type::X, EAxis::Type::Y),
		FBlockMaterialDefinition(EMaterialInstance::BaseSide, EAxis::Type::X, EAxis::Type::Y),
		FBlockMaterialDefinition(EMaterialInstance::BaseSide, EAxis::Type::Z, EAxis::Type::Y),
		FBlockMaterialDefinition(EMaterialInstance::BaseSide, EAxis::Type::Y, EAxis::Type::Z));

	definitions.Add(baseRamp.ID, baseRamp);




	FBlockDefinition constructCubeBody(EBlockName::ConstructCubeBody, EShapeType::CubeBody);
	constructCubeBody.AddMaterials(4,
		FBlockMaterialDefinition(EMaterialInstance::ConstructTriangle, EAxis::Type::Z, EAxis::Type::X),
		FBlockMaterialDefinition(EMaterialInstance::ConstructTriangle, EAxis::Type::Y, EAxis::Type::Z),
		FBlockMaterialDefinition(EMaterialInstance::ConstructTriangle, EAxis::Type::X, EAxis::Type::Y),
		FBlockMaterialDefinition(EMaterialInstance::ConstructTriangle, EAxis::Type::None, EAxis::Type::None));

	definitions.Add(constructCubeBody.ID, constructCubeBody);


	/*

	cube side

	rectangle x z
	rectangle body x y
	rectangle x y
	triangle z y
	triangle y z


	cube body
	triangle z x
	y z
	x y
	0 0

	*/



	FBlockDefinition terminal(EBlockName::Terminal, EShapeType::Custom);
	terminal.AddMaterials(2,
		FBlockMaterialDefinition(EMaterialInstance::TerminalBase, EAxis::Type::X, EAxis::Type::X),
		FBlockMaterialDefinition(EMaterialInstance::TerminalScreen, EAxis::Type::X, EAxis::Type::X, true));

	definitions.Add(terminal.ID, terminal);

}