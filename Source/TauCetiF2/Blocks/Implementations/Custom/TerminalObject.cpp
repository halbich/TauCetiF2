

#include "TauCetiF2.h"
#include "TerminalObject.h"
#include "Blocks/Definitions/FBlockDefinition.h"
#include "Blocks/Definitions/FBlockDefinitionHolder.h"


ATerminalObject::ATerminalObject(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	//auto dc = GetDestructibleComponent();
	//if (!dc)
	//	return;

	//ConstructorHelpers::FObjectFinder<UDestructibleMesh> destructible(TEXT("DestructibleMesh'/Game/BuildingObjects/DestructibleMeshes/box_DM.box_DM'"));

	//if (destructible.Succeeded())
	//	dc->SetDestructibleMesh(destructible.Object);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> mesh(TEXT("StaticMesh'/Game/BuildingObjects/Meshes/terminal.terminal'"));

	checkf(mesh.Succeeded(), TEXT("Failed to find mesh"));

	constructorSetMeshes(mesh.Object);

}


void  ATerminalObject::OnConstruction(const FTransform& Transform) {

	Super::OnConstruction(Transform);

	SelectTargetComponent->EnableUse(200);

}

