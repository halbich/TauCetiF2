

#pragma once

#include "Components/ActorComponent.h"
#include "Blocks/Public/Components/ElectricityComponent.h"
#include "Commons/Public/Enums.h"
#include "GameElectricityComponent.generated.h"


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class TAUCETIF2_API UGameElectricityComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UGameElectricityComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	TQueue<UElectricNetwork*> networksToUpdate;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


	void AddToWorldNetwork(UElectricityComponent* comp);

	void RemoveFromWorldNetwork(UElectricityComponent* comp);

private:

	void processNetwork(UElectricNetwork* network);

	static const double maxFloatingTime;

};
