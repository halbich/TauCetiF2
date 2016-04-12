

#include "TauCetiF2.h"
#include "CubeSideObject.h"
#include "Engine/DestructibleMesh.h"


ACubeSideObject::ACubeSideObject(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	//auto dc = GetDestructibleComponent();
	//if (!dc)
	//	return;

	//static ConstructorHelpers::FObjectFinder<UDestructibleMesh> destructible(TEXT("DestructibleMesh'/Game/BuildingObjects/DestructibleMeshes/box1_DM.box1_DM'"));

	//if (destructible.Succeeded())
	//	dc->SetDestructibleMesh(destructible.Object);

	auto mc = GetStaticMeshComponent();
	if (!mc)
		return;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> mesh(TEXT("StaticMesh'/Game/BuildingObjects/Meshes/box1.box1'"));

	if (mesh.Succeeded())
		mc->SetStaticMesh(mesh.Object);

}

void  ACubeSideObject::OnConstruction(const FTransform& Transform) {

	Super::OnConstruction(Transform);

	EMaterialInstance matI1 = EMaterialInstance::Empty;
	EMaterialInstance matI2 = EMaterialInstance::Empty;
	EMaterialInstance matI3 = EMaterialInstance::Empty;


	prepareMaterials(matI1, matI2, matI3);
	UMaterial* matTop = UHelpers::GetMaterialByInstance(matI1);
	UMaterial* matSide1 = UHelpers::GetMaterialByInstance(matI2);
	UMaterial* matSide2 = UHelpers::GetMaterialByInstance(matI3);


	auto scale = GetActorScale3D();

	setMaterial(matTop, 0, scale.X, scale.Y);
	setMaterial(matSide1, 1, scale.X, scale.Z);
	setMaterial(matSide2, 2, scale.Y, scale.Z);
}