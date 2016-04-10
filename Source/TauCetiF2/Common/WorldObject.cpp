

#include "TauCetiF2.h"
#include "WorldObject.h"


AWorldObject::AWorldObject(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	WorldObjectComponent = CreateDefaultSubobject<UWorldObjectComponent>(TEXT("WorldObjectComponent"));


}


void  AWorldObject::OnConstruction(const FTransform& Transform) {

	Super::OnConstruction(Transform);

	if (WorldObjectComponent && WorldObjectComponent->BlockInfo)
	{
		// TODO naplnit vlastnosti komponent

		auto bi = WorldObjectComponent->BlockInfo;

		if (bi->BlockType == EBlockType::Base) {

			auto matName = UHelpers::GetMaterialByName(TEXT("Material'/Game/Materials/BuildingObjects/MaterialBaseFloor.MaterialBaseFloor'"));
			if (matName) {

				auto matInst = UMaterialInstanceDynamic::Create(matName, this);
				if (matInst)
				{
					matInst->SetScalarParameterValue(TEXT("Kx"), 1);
					matInst->SetScalarParameterValue(TEXT("Ky"), 1);
				//	DestructibleComponent->SetMaterial(0, matInst);
				}
			}
		}
	}

}

void AWorldObject::BeginPlay() {

	FVector min;
	FVector max;
	GetBoundingBox(min, max);

	auto center = (max + min) * 0.5;

	auto extend = max - center;
	/*print(TEXT("---"));
	print(min.ToString());

	print(max.ToString());*/

	DrawDebugBox(GetWorld(), center, extend, FColor::Red, false, 60);
	//UHelpers::DrawDebugBox(this, min, max);
}

