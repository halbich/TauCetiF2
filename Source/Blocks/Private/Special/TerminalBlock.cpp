#include "Blocks.h"
#include "TerminalBlock.h"

ATerminalBlock::ATerminalBlock()
	: Super(), ListeningHandle()
{
	PrimaryActorTick.bCanEverTick = false;

	TerminalBlockMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TerminalBlockMesh"));

	TerminalBlockMesh->SetupAttachment(GetRootComponent());
}

UStaticMeshComponent* ATerminalBlock::GetMeshStructureComponent_Implementation(int32 BlockMeshStructureDefIndex)
{
	if (BlockMeshStructureDefIndex == 0)
		return TerminalBlockMesh;

	return Super::GetMeshStructureComponent_Implementation(BlockMeshStructureDefIndex);
}

void  ATerminalBlock::OnConstruction(const FTransform& Transform) {
	Super::OnConstruction(Transform);

	SelectTargetComponent->EnableUse(400);

	FUseDelegate Subscriber;
	Subscriber.BindUObject(this, &ATerminalBlock::ListeningOnUse);
	ListeningHandle = SelectTargetComponent->AddEventListener(Subscriber);
}

void ATerminalBlock::ListeningOnUse(AActor* actor, bool isSpecial)
{
	//TODO Localization!
	print(TEXT("using terminal!"));

	if (isSpecial)
		print(TEXT("Special"));
	else
		print(TEXT("not so Special"));

}

void ATerminalBlock::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (ListeningHandle.IsValid() && SelectTargetComponent)
		SelectTargetComponent->RemoveEventListener(ListeningHandle);

	Super::EndPlay(EndPlayReason);
}

UPrimitiveComponent* ATerminalBlock::GetComponentForObjectOutline_Implementation() {
	return TerminalBlockMesh;
}