#pragma optimize("", off)

#include "TauCetiF2.h"
#include "WorldObject.h"

//
//AWorldObject::AWorldObject(const FObjectInitializer& ObjectInitializer)
//	: Super(ObjectInitializer)
//{
//	SetMobility(EComponentMobility::Movable);
//
//	WorldObjectComponent = CreateDefaultSubobject<UWorldObjectComponent>(TEXT("WorldObjectComponent"));
//	SelectTargetComponent = CreateDefaultSubobject<USelectTargetComponent>(TEXT("SelectTarget"));
//}
//
//
//void  AWorldObject::OnConstruction(const FTransform& Transform) {
//
//	Super::OnConstruction(Transform);
//
//	SelectTargetComponent->RegisterTargetObjectPrimitiveComponent(GetStaticMeshComponent());
//
//
//	//TODO
//	if (WorldObjectComponent == nullptr || WorldObjectComponent->BlockInfo == nullptr)
//		return;
//
//	check(WorldObjectComponent != nullptr && WorldObjectComponent->BlockInfo != nullptr);
//
//
//	auto definition = UBlockHolderComponent::GetInstance()->GetDefinitionFor(WorldObjectComponent->BlockInfo->ID);
//
//	auto scale = GetActorScale3D();
//
//	if (WorldObjectComponent->BlockInfo->UnderConstruction)
//	{
//		UPrimitiveComponent* currentMesh = nullptr;
//		int32 currentIndexOffset(0);
//		/*for (size_t i = 0; i < definition->UsedMaterials.Num(); i++)
//		{
//			auto mat = definition->UsedMaterials[i];
//			auto setToComponent = GetPrimitiveComponentByName(mat.MeshName);
//			check(setToComponent && "Failed to find StaticMeshComponent!");
//
//			if (i > 0)
//			{
//				if (currentMesh != setToComponent)
//					currentIndexOffset = i;
//			}
//			currentMesh = setToComponent;
//
//			setConstructionMaterial(currentMesh, mat.MaterialInstance, i - currentIndexOffset, mat.GetParams(scale));
//		}*/
//
//		if (TranslucentSelectMesh)
//			TranslucentSelectMesh->DestroyComponent();
//
//		return;
//	}
//
//
//	UPrimitiveComponent* currentMesh = nullptr;
//	int32 currentIndexOffset(0);
//	bool hasTranslucent = false;
//	/*for (size_t i = 0; i < definition->UsedMaterials.Num(); i++)
//	{
//		auto mat = definition->UsedMaterials[i];
//
//		auto setToComponent = GetPrimitiveComponentByName(mat.MeshName);
//		check(setToComponent && "Failed to find StaticMeshComponent!");
//
//		if (i > 0)
//		{
//			if (currentMesh != setToComponent)
//				currentIndexOffset = i;
//		}
//		currentMesh = setToComponent;
//
//		setMaterial(currentMesh, mat.MaterialInstance, i - currentIndexOffset, mat.GetParams(scale));
//		hasTranslucent = hasTranslucent || mat.IsTranslucent;
//	}*/
//
//	if (!TranslucentSelectMesh)
//		return;
//
//	if (hasTranslucent)
//	{
//		//setTranslucentMaterials(definition->UsedMaterials.Num());
//
//		/*TranslucentSelectMesh->SetWorldScale3D(Transform.GetScale3D());
//		TranslucentSelectMesh->SetWorldLocationAndRotation(Transform.GetLocation(), Transform.GetRotation());*/
//		TranslucentSelectMesh->Activate();
//		SelectTargetComponent->RegisterTargetObjectPrimitiveComponent(TranslucentSelectMesh);
//	}
//	else
//		TranslucentSelectMesh->DestroyComponent();
//
//}
//
//void AWorldObject::BeginPlay() {
//	AStaticMeshActor::BeginPlay();
//
//}
//
//

//
//UStaticMeshComponent* AWorldObject::GetPrimitiveComponentByName(const FName& name)
//{
//	return name == NAME_None ? GetStaticMeshComponent() : nullptr;
//}
//

#pragma optimize("", on)