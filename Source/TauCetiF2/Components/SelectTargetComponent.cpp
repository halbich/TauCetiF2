

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

void USelectTargetComponent::BeginPlay()
{
	UActorComponent::BeginPlay();
	owner = GetOwner();
	if (!owner)
		print(TEXT("No Owner! "));
}


void USelectTargetComponent::Select()
{
	if (HasOutline && primitive)
	{
		primitive->SetRenderCustomDepth(true);
		primitive->CustomDepthStencilValue = STENCIL_ITEMHIGHLIGHT;
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

	if (!IsInUsableArea(selectingActor))
		return;

	MyUseEvent.Broadcast(selectingActor);

}

FDelegateHandle USelectTargetComponent::AddEventListener(FUseDelegate& UseDelegate)
{
	return MyUseEvent.Add(UseDelegate);
}

void USelectTargetComponent::RemoveEventListener(FDelegateHandle DelegateHandle)
{
	MyUseEvent.Remove(DelegateHandle);
}