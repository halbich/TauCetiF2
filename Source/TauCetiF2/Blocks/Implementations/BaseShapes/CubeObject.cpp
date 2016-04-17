

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


	}

}


void  ACubeObject::OnConstruction(const FTransform& Transform) {

	Super::OnConstruction(Transform);


	


}

