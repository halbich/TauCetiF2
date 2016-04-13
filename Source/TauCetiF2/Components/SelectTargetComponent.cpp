

#include "TauCetiF2.h"
#include "SelectTargetComponent.h"


// Sets default values for this component's properties
USelectTargetComponent::USelectTargetComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = false;



}


// Called when the game starts
void USelectTargetComponent::BeginPlay()
{
	Super::BeginPlay();

	auto owner = GetOwner();
	if (!owner)
		return;

	primitive = Cast<UPrimitiveComponent>(owner->FindComponentByClass<UStaticMeshComponent>());
	if (!primitive)
	{
		UE_LOG(LogTemp, Warning, TEXT("Actor %s failed to get component!"), *(owner->GetName()));
		return;
	}

	HasOutline = true;


}


void USelectTargetComponent::Select()
{
	if (HasOutline && primitive)
	{
		primitive->SetRenderCustomDepth(true);
		return;
	}

	auto owner = GetOwner();
	if (!owner)
		return;

	primitive = Cast<UPrimitiveComponent>(owner->FindComponentByClass<UStaticMeshComponent>());
	if (!primitive)
	{
		UE_LOG(LogTemp, Warning, TEXT("Actor %s failed to get component!"), *(owner->GetName()));
		return;
	}

	HasOutline = true;

}

void USelectTargetComponent::Deselect()
{
	if (HasOutline && primitive)
		primitive->SetRenderCustomDepth(false);
}