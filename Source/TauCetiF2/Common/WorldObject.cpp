

#include "TauCetiF2.h"
#include "WorldObject.h"
#include "Blocks/FBlockDefinition.h"
#include "Blocks/FBlockDefinitionHolder.h"


AWorldObject::AWorldObject(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	SetMobility(EComponentMobility::Movable);

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

	bool hasTranslucent = false;
	for (size_t i = 0; i < definition->UsedMaterials.Num(); i++)
	{
		auto mat = definition->UsedMaterials[i];

		setMaterial(mat.MaterialInstance, i, mat.GetParams(scale));
		hasTranslucent = hasTranslucent || mat.IsTranslucent;
	}

	if (!TranslucentSelectMesh)
		return;

	if (hasTranslucent)
	{
		setTranslucentMaterials(definition->UsedMaterials.Num());

		TranslucentSelectMesh->SetWorldScale3D(Transform.GetScale3D());
		TranslucentSelectMesh->SetWorldLocationAndRotation(Transform.GetLocation(), Transform.GetRotation());
		TranslucentSelectMesh->Activate();
		SelectTargetComponent->RegisterTargetPrimitiveComponent(TranslucentSelectMesh);
	}
	else
		TranslucentSelectMesh->DestroyComponent();

}

void AWorldObject::BeginPlay() {
	AStaticMeshActor::BeginPlay();

}


void AWorldObject::GetBoundingBox(FVector& min, FVector& max) {
	auto location = GetActorLocation();
	auto scale = GetActorRotation().RotateVector(GetActorScale3D() * 10);

	min = location - scale;
	max = location + scale;

}
