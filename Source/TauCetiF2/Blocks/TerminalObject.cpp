

#include "TauCetiF2.h"
#include "TerminalObject.h"
#include "Blocks/FBlockDefinition.h"
#include "Blocks/FBlockDefinitionHolder.h"


ATerminalObject::ATerminalObject(const FObjectInitializer& ObjectInitializer)
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

	static ConstructorHelpers::FObjectFinder<UStaticMesh> mesh(TEXT("StaticMesh'/Game/BuildingObjects/Meshes/terminal.terminal'"));

	if (mesh.Succeeded())
	{
		mc->SetStaticMesh(mesh.Object);
		TranslucentSelectMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TranslucentMeshSelector"));
		TranslucentSelectMesh->SetStaticMesh(mesh.Object);
		TranslucentSelectMesh->SetRenderInMainPass(false);
		TranslucentSelectMesh->Deactivate();


	}

}


void  ATerminalObject::OnConstruction(const FTransform& Transform) {

	Super::OnConstruction(Transform);

	SelectTargetComponent->EnableUse(200);



}

void ATerminalObject::GetBoundingBox(FVector& min, FVector& max) {
	
	FTransform trans;
	trans.SetLocation(GetActorLocation());
	trans.SetScale3D(FVector(1, 8, 5));
	trans.SetRotation(FQuat(GetActorRotation()));
	AWorldObject::GetBoundingBox(trans, min, max);

}
