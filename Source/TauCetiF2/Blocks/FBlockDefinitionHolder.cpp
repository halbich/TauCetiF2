

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
		FBlockMaterialDefinition(EMaterialInstance::ConstructRectangle, 1, 1, 0),
		FBlockMaterialDefinition(EMaterialInstance::ConstructRectangle, 1, 0, 1),
		FBlockMaterialDefinition(EMaterialInstance::ConstructRectangle, 0, 1, 1));

	definitions.Add(cubeBuilding.ID, cubeBuilding);


	FBlockDefinition cubeBase(EBlockName::BaseCube, EShapeType::Cube);
	cubeBase.AddMaterials(3,
		FBlockMaterialDefinition(EMaterialInstance::BaseFloor, 1, 1, 0),
		FBlockMaterialDefinition(EMaterialInstance::BaseSide, 1, 0, 1),
		FBlockMaterialDefinition(EMaterialInstance::BaseSide, 0, 1, 1));

	definitions.Add(cubeBase.ID, cubeBase);


	FBlockDefinition cubeWindow(EBlockName::WindowCube, EShapeType::Cube);
	cubeWindow.AddMaterials(3,
		FBlockMaterialDefinition(EMaterialInstance::Polycarbonate, 1, 1, 0),
		FBlockMaterialDefinition(EMaterialInstance::Polycarbonate, 1, 0, 1),
		FBlockMaterialDefinition(EMaterialInstance::Polycarbonate, 0, 1, 1));

	definitions.Add(cubeWindow.ID, cubeWindow);


}