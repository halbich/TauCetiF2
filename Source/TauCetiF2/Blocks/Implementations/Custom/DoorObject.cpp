

#include "TauCetiF2.h"
#include "DoorObject.h"




ADoorObject::ADoorObject(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer), ListeningHandle(), doorState(EDoorState::Closed), doorOpening(EDoorOpening::Left)
{
	//auto dc = GetDestructibleComponent();
	//if (!dc)
	//	return;

	//ConstructorHelpers::FObjectFinder<UDestructibleMesh> destructible(TEXT("DestructibleMesh'/Game/BuildingObjects/DestructibleMeshes/box_DM.box_DM'"));

	//if (destructible.Succeeded())
	//	dc->SetDestructibleMesh(destructible.Object);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> mesh(TEXT("StaticMesh'/Game/BuildingObjects/Meshes/doorFrame.doorFrame'"));

	checkf(mesh.Succeeded(), TEXT("Failed to find mesh"));

	constructorSetMeshes(mesh.Object);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> doorMesh(TEXT("StaticMesh'/Game/BuildingObjects/Meshes/door.door'"));

	if (!doorMesh.Succeeded())
		return;


	DoorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorMesh"));
	DoorMesh->SetStaticMesh(doorMesh.Object);
	DoorMesh->AttachTo(GetStaticMeshComponent());

	PrimaryActorTick.bCanEverTick = true;

}


void  ADoorObject::OnConstruction(const FTransform& Transform) {

	Super::OnConstruction(Transform);

	SelectTargetComponent->EnableUse(200);
	FUseDelegate Subscriber;
	Subscriber.BindUObject(this, &ADoorObject::ListeningOnUse);
	ListeningHandle = SelectTargetComponent->AddEventListener(Subscriber);

	if (WorldObjectComponent && WorldObjectComponent->BlockInfo)
	{
		auto key = TEXT("doorOpening");
		auto valueptr = WorldObjectComponent->BlockInfo->AdditionalFlags.Find(key);
		if (valueptr)
			doorOpening = (EDoorOpening)(*valueptr);
	}

}

void ADoorObject::ListeningOnUse(AActor* actor)
{
	print(TEXT("using door!"));

	if (doorState == EDoorState::Closed)
	{
		doorState = EDoorState::Opening;
		return;
	}
	else if (doorState == EDoorState::Opened)
		doorState = EDoorState::Closing;
}

void ADoorObject::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (ListeningHandle.IsValid() && SelectTargetComponent)
		SelectTargetComponent->RemoveEventListener(ListeningHandle);

	AWorldObject::EndPlay(EndPlayReason);
}

void ADoorObject::Tick(float DeltaSeconds)
{
	if (doorState == EDoorState::Closed || doorState == EDoorState::Opened)
		return;

	auto currentTrans = DoorMesh->GetRelativeTransform();
	auto currentYaw = DoorMesh->GetRelativeTransform().Rotator().Yaw;
	auto rotAdd = DeltaSeconds * (doorOpening == EDoorOpening::Left ? -1 : 1) * 90;

	if (currentYaw + rotAdd >= 90.0f)
	{
		doorState = EDoorState::Opened;
	}

	currentTrans.SetRotation(currentTrans.Rotator().Add(0, rotAdd, 0).Quaternion());

	DoorMesh->SetRelativeTransform(currentTrans);
	//print(TEXT("door tick"));
}

void ADoorObject::SetBlockInfo(UBlockInfo* info, FBlockDefinition* definition)
{
	AWorldObject::SetBlockInfo(info, definition);
}