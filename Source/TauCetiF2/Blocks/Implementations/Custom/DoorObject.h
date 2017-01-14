

#pragma once

#include "Game/Localization/LocalizationHelpers.h"
#include "WorldObject.h"
//#include "DoorObject.generated.h"

//enum class EDoorState : uint8
//{
//	Closed,
//	Opening,
//	Opened,
//	Closing
//
//};
//
//
//
///**
// *
// */
//UCLASS()
//class TAUCETIF2_API ADoorObject : public AWorldObject
//{
//	GENERATED_UCLASS_BODY()
//
//public:
//	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = WorldObject)
//		UStaticMeshComponent* DoorMesh;
//
//	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = WorldObject)
//		UStaticMeshComponent* DoorFrameMesh;
//
//	virtual void OnConstruction(const FTransform& Transform) override;
//
//	virtual void Tick(float DeltaSeconds) override;
//
//	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
//
//	FDelegateHandle ListeningHandle;
//
//	virtual void SetBlockInfo(UBlockInfo* info, UBlockDefinition* definition) override;
//
//protected:
//
//	virtual UStaticMeshComponent* GetPrimitiveComponentByName(const FName& name) override;
//
//private:
//	EDoorState doorState;
//	EDoorOpening doorOpening;
//
//	void ListeningOnUse(AActor* actor);
//
//
//	void updateDoorState(FTransform& currentTrans, int32 openingConstant)
//	{
//		switch (doorState)
//		{
//		case EDoorState::Closed:
//			currentTrans.SetLocation(FVector::ZeroVector);
//			SelectTargetComponent->CustomUsingMessage = LC_DoorOpen;
//			break;
//		case EDoorState::Opening:
//			currentTrans.SetLocation(FMath::InterpSinIn(FVector::ZeroVector, FVector(-60 * openingConstant, 60, 0), FMath::Abs(currentTrans.Rotator().Yaw / 90.0f)));
//			break;
//		case EDoorState::Opened:
//			currentTrans.SetLocation(FVector(-60 * openingConstant, 60, 0));
//			SelectTargetComponent->CustomUsingMessage = LC_DoorClose;
//			break;
//		case EDoorState::Closing:
//			currentTrans.SetLocation(FMath::InterpSinIn(FVector::ZeroVector, FVector(60 * openingConstant, 60, 0), FMath::Abs(currentTrans.Rotator().Yaw / 90.0f)));
//			break;
//
//		}
//
//		DoorMesh->SetRelativeTransform(currentTrans);
//	}
//};
