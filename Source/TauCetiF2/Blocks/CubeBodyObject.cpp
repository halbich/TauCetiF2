

#include "TauCetiF2.h"
#include "CubeBodyObject.h"
#include "Engine/DestructibleMesh.h"


ACubeBodyObject::ACubeBodyObject(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	//auto dc = GetDestructibleComponent();
	//if (!dc)
	//	return;

	//ConstructorHelpers::FObjectFinder<UDestructibleMesh> destructible(TEXT("DestructibleMesh'/Game/BuildingObjects/DestructibleMeshes/box2_DM.box2_DM'"));

	//if (destructible.Succeeded())
	//	dc->SetDestructibleMesh(destructible.Object);

	auto mc = GetStaticMeshComponent();
	if (!mc)
		return;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> mesh(TEXT("StaticMesh'/Game/BuildingObjects/Meshes/box2.box2'"));

	if (mesh.Succeeded())
		mc->SetStaticMesh(mesh.Object);

}


void  ACubeBodyObject::OnConstruction(const FTransform& Transform) {

	Super::OnConstruction(Transform);
}

