

#pragma once

#include "Blocks/WorldObject.h"
#include "DoorObject.generated.h"

enum class EDoorState : uint8
{
	Closed,
	Opening,
	Opened,
	Closing

};



/**
 *
 */
UCLASS()
class TAUCETIF2_API ADoorObject : public AWorldObject
{
	GENERATED_UCLASS_BODY()

		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = WorldObject)
		UStaticMeshComponent* DoorMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = WorldObject)
		UStaticMeshComponent* DoorFrameMesh;

public:
	virtual void OnConstruction(const FTransform& Transform) override;

	virtual void Tick(float DeltaSeconds) override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override ;

	FDelegateHandle ListeningHandle;

	virtual void SetBlockInfo(UBlockInfo* info, FBlockDefinition* definition) override;

protected:

	virtual UStaticMeshComponent* GetPrimitiveComponentByName(const FName& name) override;

private:
	EDoorState doorState;
	EDoorOpening doorOpening;

	void ListeningOnUse(AActor* actor);

};