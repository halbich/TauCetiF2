

#include "TauCetiF2.h"
#include "WorldObject.h"


AWorldObject::AWorldObject(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	WorldObjectComponent = CreateDefaultSubobject<UWorldObjectComponent>(TEXT("WorldObjectComponent"));


}


void  AWorldObject::OnConstruction(const FTransform& Transform) {

	if (WorldObjectComponent && WorldObjectComponent->BlockInfo)
	{
		// TODO naplnit vlastnosti komponent
	}
}

