

#include "TauCetiF2.h"
#include "WorldObject.h"


AWorldObject::AWorldObject(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	print(TEXT("constructor"));
	WorldObjectComponent = CreateDefaultSubobject<UWorldObjectComponent>(TEXT("WorldObjectComponent"));
	

}

void AWorldObject::PostInitProperties() {

	Super::PostInitProperties();
	print(TEXT("PostInitProperties"));

	auto size = GetActorScale3D();
	print(size.ToString());

}

void AWorldObject::PostActorCreated() {

	Super::PostActorCreated();
	print(TEXT("PostActorCreated"));
	auto size = GetActorScale3D();
	print(size.ToString());

}

void AWorldObject::PostLoad() {

	Super::PostLoad();
	print(TEXT("PostLoad"));
	auto size = GetActorScale3D();
		print(size.ToString());

}

void  AWorldObject::OnConstruction(const FTransform& Transform) {
	print(TEXT("OnConstruction"));
	print(Transform.GetScale3D().ToString());
}

