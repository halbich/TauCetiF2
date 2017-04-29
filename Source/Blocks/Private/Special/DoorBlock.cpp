#include "Blocks.h"
#include "DoorBlock.h"

ADoorBlock::ADoorBlock()
	: Super(), ListeningHandle(), doorState(EDoorState::Opened), doorOpening(EDoorOpening::Left)
{
	PrimaryActorTick.bCanEverTick = true;

	DoorBlockFrameMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorBlockFrameMesh"));
	DoorBlockFrameMesh->SetupAttachment(GetRootComponent());
	DoorBlockFrameMesh->AddLocalOffset(FVector(0, -3 * GameDefinitions::CubeMinSize, 0));

	DoorBlockMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorBlockMesh"));
	DoorBlockMesh->SetupAttachment(DoorBlockFrameMesh);

	ElectricityComponent = CreateDefaultSubobject<UElectricityComponent>(TEXT("ElectricityComponent"));
	AddOwnedComponent(ElectricityComponent);
}

UStaticMeshComponent* ADoorBlock::GetMeshStructureComponent_Implementation(int32 BlockMeshStructureDefIndex)
{
	switch (BlockMeshStructureDefIndex)
	{
	case 0:
		return DoorBlockFrameMesh;
	case 1:
		return DoorBlockMesh;
	default:
		return Super::GetMeshStructureComponent_Implementation(BlockMeshStructureDefIndex);
	}
}

void ADoorBlock::OnConstruction(const FTransform& Transform) {
	Super::OnConstruction(Transform);

	SelectTargetComponent->EnableSelect(DoorBlockFrameMesh);
	SelectTargetComponent->EnableUse(200, DoorBlockMesh);
	FUseDelegate Subscriber;
	Subscriber.BindUObject(this, &ADoorBlock::ListeningOnUse);
	ListeningHandle = SelectTargetComponent->AddEventListener(Subscriber);
}

void ADoorBlock::SetBlockInfo(UBlockInfo* info)
{
	Super::SetBlockInfo(info);

	auto valueptr = info->AdditionalFlags.Find(TEXT("DoorOpening"));
	if (valueptr)
		doorOpening = (EDoorOpening)(*valueptr);

	auto openingConstant = doorOpening == EDoorOpening::Left ? 1 : -1;

	auto state = info->BlockSpecificData.FindOrAdd(DoorBlockConstants::DoorState);
	if (state.IsEmpty())
	{
		doorState = EDoorState::Opened;
		BlockInfo->BlockSpecificData[DoorBlockConstants::DoorState] = FString::FromInt((uint8)doorState);
	}
	else {
		doorState = (EDoorState)FCString::Atoi(*state);
	}

	auto setYaw = 90 * openingConstant;

	auto yaw = BlockInfo->BlockSpecificData.FindOrAdd(DoorBlockConstants::DoorYaw);
	if (yaw.IsEmpty())
		BlockInfo->BlockSpecificData[DoorBlockConstants::DoorYaw] = FString::SanitizeFloat(setYaw);
	else
		setYaw = FCString::Atof(*yaw);

	auto transf = DoorBlockMesh->GetRelativeTransform();
	transf.SetRotation(transf.Rotator().Add(0, setYaw, 0).Quaternion());
	updateDoorState(transf, openingConstant);
}

void ADoorBlock::ListeningOnUse(AActor* actor, bool isSpecial)
{
	if (doorState == EDoorState::Closed)
	{
		doorState = EDoorState::Opening;
		return;
	}
	else if (doorState == EDoorState::Opened)
		doorState = EDoorState::Closing;
}

void ADoorBlock::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (ListeningHandle.IsValid() && SelectTargetComponent)
		SelectTargetComponent->RemoveEventListener(ListeningHandle);

	Super::EndPlay(EndPlayReason);
}

void ADoorBlock::Tick(float DeltaSeconds)
{
	if (doorState == EDoorState::Closed || doorState == EDoorState::Opened)
	{
		Super::Tick(DeltaSeconds);
		return;
	}

	auto currentTrans = DoorBlockMesh->GetRelativeTransform();
	auto currentYaw = DoorBlockMesh->GetRelativeTransform().Rotator().Yaw;
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

	Super::Tick(DeltaSeconds);
}


