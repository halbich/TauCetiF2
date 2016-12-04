

#include "TauCetiF2.h"
#include "Game/GameDefinitions.h"
#include "SelectTargetComponent.h"


// Sets default values for this component's properties
USelectTargetComponent::USelectTargetComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;



}

void USelectTargetComponent::BeginPlay()
{
	UActorComponent::BeginPlay();
	owner = GetOwner();
	if (!owner)
		print(TEXT("No Owner! "));
}

void USelectTargetComponent::SelectObject(int32 stencilValue)
{
	if (HasObjectOutline && objectOutlinePrimitive && objectOutlinePrimitive->IsValidLowLevel())
	{
		objectOutlinePrimitive->SetRenderCustomDepth(true);
		objectOutlinePrimitive->CustomDepthStencilValue = stencilValue > STENCIL_MINIMUM_USABLE_EXCLUDING ? stencilValue : STENCIL_ITEM_HIGHLIGHT;
	}
}

void USelectTargetComponent::DeselectObject()
{
	if (HasObjectOutline && objectOutlinePrimitive && objectOutlinePrimitive->IsValidLowLevel())
	{
		objectOutlinePrimitive->SetRenderCustomDepth(false);
	}
}

void USelectTargetComponent::SelectUsableObject()
{
	if (HasUsableObjectOutline && usableObjectOutlinePrimitive && usableObjectOutlinePrimitive->IsValidLowLevel())
	{
		usableObjectOutlinePrimitive->SetRenderCustomDepth(true);
		usableObjectOutlinePrimitive->CustomDepthStencilValue = STENCIL_USE_HIGHLIGHT;
	}
}

void USelectTargetComponent::DeselectUsableObject()
{
	if (HasUsableObjectOutline && usableObjectOutlinePrimitive && usableObjectOutlinePrimitive->IsValidLowLevel())
	{
		usableObjectOutlinePrimitive->SetRenderCustomDepth(false);
	}
}

void USelectTargetComponent::RegisterTargetObjectPrimitiveComponent(UPrimitiveComponent* comp) {
	objectOutlinePrimitive = comp;
	if (comp != nullptr && comp->IsValidLowLevel()) {
		HasObjectOutline = true;
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