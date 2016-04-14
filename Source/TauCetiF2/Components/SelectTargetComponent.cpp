

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



void USelectTargetComponent::Select()
{
	if (HasOutline && primitive)
	{
		primitive->SetRenderCustomDepth(true);
		return;
	}
}

void USelectTargetComponent::Deselect()
{
	if (HasOutline && primitive)
		primitive->SetRenderCustomDepth(false);
}

void USelectTargetComponent::RegisterTargetPrimitiveComponent(UPrimitiveComponent* comp) {
	primitive = comp;
	if (comp != nullptr && comp->IsValidLowLevel()) {
		HasOutline = true;
	}

}

void USelectTargetComponent::OnUse(AActor* selectingActor) {



	if (!selectingActor || !IsUsable)
		return;

	if (MaxDistance > 0 && FVector::Dist(GetOwner()->GetActorLocation(), selectingActor->GetActorLocation()) > MaxDistance)
		return;

	print(TEXT("Using!"));


}