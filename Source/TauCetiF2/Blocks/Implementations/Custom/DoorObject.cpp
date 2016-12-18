
#include "TauCetiF2.h"
#include "DoorObject.h"




ADoorObject::ADoorObject(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer), ListeningHandle(), doorState(EDoorState::Opened), doorOpening(EDoorOpening::Left)
{

	static ConstructorHelpers::FObjectFinder<UStaticMesh> mesh(TEXT("StaticMesh'/Game/BuildingObjects/Meshes/doorFrame.doorFrame'"));

	checkf(mesh.Succeeded(), TEXT("Failed to find mesh"));

	//constructorSetMeshes(mesh.Object);
	DoorFrameMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorFrameMesh"));
	DoorFrameMesh->SetStaticMesh(mesh.Object);
	DoorFrameMesh->SetupAttachment(GetStaticMeshComponent());
	DoorFrameMesh->AddLocalOffset(FVector(0, -3 * GameDefinitions::CubeMinSize, 0));


	static ConstructorHelpers::FObjectFinder<UStaticMesh> doorMesh(TEXT("StaticMesh'/Game/BuildingObjects/Meshes/door.door'"));

	if (!doorMesh.Succeeded())
		return;


	DoorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorMesh"));
	DoorMesh->SetStaticMesh(doorMesh.Object);
	DoorMesh->SetupAttachment(DoorFrameMesh);

	PrimaryActorTick.bCanEverTick = true;

}


void  ADoorObject::OnConstruction(const FTransform& Transform) {

	Super::OnConstruction(Transform);

	SelectTargetComponent->RegisterTargetObjectPrimitiveComponent(DoorFrameMesh);
	SelectTargetComponent->EnableUse(200, DoorMesh);
	FUseDelegate Subscriber;
	Subscriber.BindUObject(this, &ADoorObject::ListeningOnUse);
	ListeningHandle = SelectTargetComponent->AddEventListener(Subscriber);

	
}

void ADoorObject::ListeningOnUse(AActor* actor)
{
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
	auto openingConstant = (doorOpening == EDoorOpening::Left && doorState == EDoorState::Closing) ||
		(doorOpening == EDoorOpening::Right && doorState == EDoorState::Opening) ? -1 : 1;
	auto rotAdd = DeltaSeconds * openingConstant * 180;

	auto newRot = currentYaw + rotAdd;
	if (doorState == EDoorState::Opening)
	{
		if (openingConstant*newRot >= 90.0f)
		{
			rotAdd = 90.0f - openingConstant*currentYaw;
			doorState = EDoorState::Opened;
		}
	}
	else
	{
		if (openingConstant*newRot >= 0.0f)
		{
			rotAdd = -currentYaw;
			doorState = EDoorState::Closed;
		}
	}


	currentTrans.SetRotation(currentTrans.Rotator().Add(0, rotAdd, 0).Quaternion());


	

	updateDoorState(currentTrans, openingConstant);
}

void ADoorObject::SetBlockInfo(UBlockInfo* info, UBlockDefinition* definition)
{
	AWorldObject::SetBlockInfo(info, definition);


	if (WorldObjectComponent && WorldObjectComponent->BlockInfo)
	{
		auto key = GetNameForTag(ENamedTag::DoorOpening);
		auto valueptr = WorldObjectComponent->BlockInfo->AdditionalFlags.Find(key);
		if (valueptr)
			doorOpening = (EDoorOpening)(*valueptr);
	}


	auto openingConstant = doorOpening == EDoorOpening::Left ? 1 : -1;
	auto transf = DoorMesh->GetRelativeTransform();
	transf.SetRotation(transf.Rotator().Add(0, 90 * openingConstant, 0).Quaternion());
	updateDoorState(transf, openingConstant);

}

UStaticMeshComponent* ADoorObject::GetPrimitiveComponentByName(const FName& name)
{
	if (name == TEXT("doorFrame"))
		return DoorFrameMesh;
	else
		if (name == TEXT("door"))
			return DoorMesh;

	return AWorldObject::GetPrimitiveComponentByName(name);
}
