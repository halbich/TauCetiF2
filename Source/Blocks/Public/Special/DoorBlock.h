#pragma once

#include "Block.h"
#include "Commons/Public/GameDefinitions.h"
#include "DoorBlock.generated.h"

enum class EDoorState : uint8
{
	Closed,
	Opening,
	Opened,
	Closing
};

enum class EDoorOpening : uint8
{
	Left,
	Right
};

/**
 *
 */
UCLASS()
class BLOCKS_API ADoorBlock : public ABlock
{
	GENERATED_BODY()

public:
	ADoorBlock();

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category = "TCF2 | DoorBlock")
		UStaticMeshComponent* DoorBlockFrameMesh;

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category = "TCF2 | DoorBlock")
		UStaticMeshComponent* DoorBlockMesh;

	virtual UStaticMeshComponent* GetMeshStructureComponent_Implementation(int32 BlockMeshStructureDefIndex) override;

	virtual void OnConstruction(const FTransform& Transform) override;

	virtual void Tick(float DeltaSeconds) override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	virtual void SetBlockInfo(UBlockInfo* info);

	FDelegateHandle ListeningHandle;

private:
	EDoorState doorState;
	EDoorOpening doorOpening;

	void ListeningOnUse(AActor* actor);

	void updateDoorState(FTransform& currentTrans, int32 openingConstant)
	{
		switch (doorState)
		{
		case EDoorState::Closed:
			currentTrans.SetLocation(FVector::ZeroVector);
			SelectTargetComponent->CustomUsingMessage = NSLOCTEXT("TCF2LocSpace", "LC.DoorOpen", "Otevøít dveøe");
			break;
		case EDoorState::Opening:
			currentTrans.SetLocation(FMath::InterpSinIn(FVector::ZeroVector, FVector(-60 * openingConstant, 60, 0), FMath::Abs(currentTrans.Rotator().Yaw / 90.0f)));
			break;
		case EDoorState::Opened:
			currentTrans.SetLocation(FVector(-60 * openingConstant, 60, 0));
			SelectTargetComponent->CustomUsingMessage = NSLOCTEXT("TCF2LocSpace", "LC.DoorClose", "Zavøít dveøe");
			break;
		case EDoorState::Closing:
			currentTrans.SetLocation(FMath::InterpSinIn(FVector::ZeroVector, FVector(60 * openingConstant, 60, 0), FMath::Abs(currentTrans.Rotator().Yaw / 90.0f)));
			break;
		}

		DoorBlockMesh->SetRelativeTransform(currentTrans);
	}
};
