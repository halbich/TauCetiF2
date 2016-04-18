#pragma optimize("", off)

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

	//constructorSetMeshes(mesh.Object);
	DoorFrameMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorFrameMesh"));
	DoorFrameMesh->SetStaticMesh(mesh.Object);
	DoorFrameMesh->AttachTo(GetStaticMeshComponent());
	DoorFrameMesh->AddLocalOffset(FVector(0, -3 * 20, 0));


	//TranslucentSelectMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TranslucentMeshSelector"));
	//TranslucentSelectMesh->SetStaticMesh(mesh);
	//TranslucentSelectMesh->SetRenderInMainPass(false);
	//TranslucentSelectMesh->Deactivate();
	//TranslucentSelectMesh->AttachTo(mc);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> doorMesh(TEXT("StaticMesh'/Game/BuildingObjects/Meshes/door.door'"));

	if (!doorMesh.Succeeded())
		return;


	DoorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorMesh"));
	DoorMesh->SetStaticMesh(doorMesh.Object);
	DoorMesh->AttachTo(DoorFrameMesh);

	PrimaryActorTick.bCanEverTick = true;

}


void  ADoorObject::OnConstruction(const FTransform& Transform) {

	Super::OnConstruction(Transform);

	SelectTargetComponent->RegisterTargetPrimitiveComponent(DoorMesh);
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


	switch (doorState)
	{
	case EDoorState::Closed: 
		currentTrans.SetLocation(FVector::ZeroVector);
		break;
	case EDoorState::Opening:
		currentTrans.SetLocation(FMath::InterpSinIn(FVector::ZeroVector, FVector(-60 * openingConstant , 60, 0), FMath::Abs(currentTrans.Rotator().Yaw / 90.0f)));
		break;
	case EDoorState::Opened: break;
		currentTrans.SetLocation(FVector(-60 * openingConstant, 60, 0));
	case EDoorState::Closing:
		currentTrans.SetLocation(FMath::InterpSinIn( FVector::ZeroVector,FVector(60* openingConstant, 60, 0), FMath::Abs( currentTrans.Rotator().Yaw / 90.0f)));
		break;

	}

	DoorMesh->SetRelativeTransform(currentTrans);
	//print(TEXT("door tick"));
}

void ADoorObject::SetBlockInfo(UBlockInfo* info, FBlockDefinition* definition)
{
	AWorldObject::SetBlockInfo(info, definition);


}

#pragma optimize("", on)