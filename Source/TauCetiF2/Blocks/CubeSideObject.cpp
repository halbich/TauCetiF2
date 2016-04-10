

#include "TauCetiF2.h"
#include "CubeSideObject.h"
#include "Engine/DestructibleMesh.h"


ACubeSideObject::ACubeSideObject(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	auto dc = GetDestructibleComponent();
	if (!dc)
		return;

	static ConstructorHelpers::FObjectFinder<UDestructibleMesh> destructible(TEXT("DestructibleMesh'/Game/BuildingObjects/DestructibleMeshes/box1_DM.box1_DM'"));

	if (destructible.Succeeded())
		dc->SetDestructibleMesh(destructible.Object);

}

void  ACubeSideObject::OnConstruction(const FTransform& Transform) {

	Super::OnConstruction(Transform);
}