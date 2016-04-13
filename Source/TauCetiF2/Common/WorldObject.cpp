

#include "TauCetiF2.h"
#include "WorldObject.h"
#include "Blocks/FBlockDefinition.h"
#include "Blocks/FBlockDefinitionHolder.h"


AWorldObject::AWorldObject(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	SetMobility(EComponentMobility::Static);

	WorldObjectComponent = CreateDefaultSubobject<UWorldObjectComponent>(TEXT("WorldObjectComponent"));
	SelectTargetComponent = CreateDefaultSubobject<USelectTargetComponent>(TEXT("SelectTarget"));


}


void  AWorldObject::OnConstruction(const FTransform& Transform) {

	Super::OnConstruction(Transform);

	SelectTargetComponent->RegisterTargetPrimitiveComponent(GetStaticMeshComponent());

	if (WorldObjectComponent == nullptr || WorldObjectComponent->BlockInfo == nullptr)
		return;

	//TODO assert


	auto definition = FBlockDefinitionHolder::Instance().GetDefinition(WorldObjectComponent->BlockInfo->ID);

	auto scale = GetActorScale3D();

	bool hasPolycarbonate = false;
	for (size_t i = 0; i < definition->UsedMaterials.Num(); i++)
	{
		auto mat = definition->UsedMaterials[i];

		setMaterial(mat.MaterialInstance, i, mat.ApplyCoordinates * scale);
		hasPolycarbonate = hasPolycarbonate || mat.MaterialInstance == EMaterialInstance::Polycarbonate;
	}



	if (hasPolycarbonate)
	{
		TranslucentSelectMesh->SetWorldScale3D(Transform.GetScale3D());
		TranslucentSelectMesh->SetWorldLocationAndRotation(Transform.GetLocation(), Transform.GetRotation());
		TranslucentSelectMesh->Activate();
		SelectTargetComponent->RegisterTargetPrimitiveComponent(TranslucentSelectMesh);
	}
	else
		TranslucentSelectMesh->DestroyComponent();

}

void AWorldObject::BeginPlay() {


}

