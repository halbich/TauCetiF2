

#include "TauCetiF2.h"
#include "TerminalObject.h"
#include "Blocks/Definitions/FBlockDefinition.h"
#include "Blocks/Definitions/FBlockDefinitionHolder.h"


ATerminalObject::ATerminalObject(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer), ListeningHandle()
{

	static ConstructorHelpers::FObjectFinder<UStaticMesh> mesh(TEXT("StaticMesh'/Game/BuildingObjects/Meshes/terminal.terminal'"));

	checkf(mesh.Succeeded(), TEXT("Failed to find mesh"));

	constructorSetMeshes(mesh.Object);
	

}


void  ATerminalObject::OnConstruction(const FTransform& Transform) {

	Super::OnConstruction(Transform);

	SelectTargetComponent->EnableUse(200);

	FUseDelegate Subscriber;
	Subscriber.BindUObject(this, &ATerminalObject::ListeningOnUse);
	ListeningHandle = SelectTargetComponent->AddEventListener(Subscriber);

}

void ATerminalObject::ListeningOnUse(AActor* actor)
{
	print(TEXT("using terminal!"));
}

void ATerminalObject::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (ListeningHandle.IsValid() && SelectTargetComponent)
		SelectTargetComponent->RemoveEventListener(ListeningHandle);

	AWorldObject::EndPlay(EndPlayReason);
}

