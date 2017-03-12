

#include "Blocks.h"
#include "OxygenComponent.h"


// Sets default values for this component's properties
UOxygenComponent::UOxygenComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UOxygenComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...

}


// Called every frame
void UOxygenComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

UBlockWithOxygenInfo* UOxygenComponent::SetInfo(UBlockWithOxygenInfo* info)
{
	if (!info || !info->IsValidLowLevel())
	{
		info = NewObject<UBlockWithOxygenInfo>();
		info->CurrentFillingValue = FMath::FRandRange(0, 20.0f);
	}

	OxygenInfo = info;
	return OxygenInfo;
}

