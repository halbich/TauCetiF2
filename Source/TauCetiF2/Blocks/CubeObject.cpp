

#include "TauCetiF2.h"

#include "Engine/DestructibleMesh.h"
#include "CubeObject.h"


ACubeObject::ACubeObject(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	//auto dc = GetDestructibleComponent();
	//if (!dc)
	//	return;

	//ConstructorHelpers::FObjectFinder<UDestructibleMesh> destructible(TEXT("DestructibleMesh'/Game/BuildingObjects/DestructibleMeshes/box_DM.box_DM'"));

	//if (destructible.Succeeded())
	//	dc->SetDestructibleMesh(destructible.Object);

	auto mc = GetStaticMeshComponent();
	if (!mc)
		return;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> mesh(TEXT("StaticMesh'/Game/BuildingObjects/Meshes/box.box'"));

	if (mesh.Succeeded())
	{
		mc->SetStaticMesh(mesh.Object);
		TranslucentSelectMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TranslucentMeshSelector"));
		TranslucentSelectMesh->SetStaticMesh(mesh.Object);
		TranslucentSelectMesh->SetRenderInMainPass(false);
		TranslucentSelectMesh->Deactivate();
		TranslucentSelectMesh->SetMobility(EComponentMobility::Static);
		
	}

}


void  ACubeObject::OnConstruction(const FTransform& Transform) {

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


	if (WorldObjectComponent->BlockInfo->MaterialType == EMaterialType::Polycarbon)
	{
		TranslucentSelectMesh->SetWorldScale3D(Transform.GetScale3D());
		TranslucentSelectMesh->SetWorldLocationAndRotation(Transform.GetLocation(), Transform.GetRotation());
		TranslucentSelectMesh->Activate();
		SelectTargetComponent->RegisterTargetPrimitiveComponent(TranslucentSelectMesh);
	}
	else
		TranslucentSelectMesh->DestroyComponent();


}

