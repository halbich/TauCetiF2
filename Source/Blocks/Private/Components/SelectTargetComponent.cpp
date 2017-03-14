#include "Blocks.h"
#include "Commons/Public/GameDefinitions.h"
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
	Super::BeginPlay();
	owner = GetOwner();
	if (!owner)
		print(TEXT("No Owner! "));
}

void USelectTargetComponent::SelectObject(int32 stencilValue)
{
	if (HasObjectOutline && objectOutlinePrimitive && objectOutlinePrimitive->IsValidLowLevel())
	{
		objectOutlinePrimitive->CustomDepthStencilValue = stencilValue > STENCIL_MINIMUM_USABLE_EXCLUDING ? stencilValue : STENCIL_ITEM_HIGHLIGHT;
		objectOutlinePrimitive->SetRenderCustomDepth(true);
	}
}

void USelectTargetComponent::DeselectObject()
{
	if (HasObjectOutline && objectOutlinePrimitive && objectOutlinePrimitive->IsValidLowLevel())
		objectOutlinePrimitive->SetRenderCustomDepth(false);
}

void USelectTargetComponent::SelectUsableObject()
{
	if (HasUsableObjectOutline && usableObjectOutlinePrimitive && usableObjectOutlinePrimitive->IsValidLowLevel())
	{
		usableObjectOutlinePrimitive->CustomDepthStencilValue = STENCIL_USE_HIGHLIGHT;
		usableObjectOutlinePrimitive->SetRenderCustomDepth(true);
	}
}

void USelectTargetComponent::DeselectUsableObject()
{
	if (HasUsableObjectOutline && usableObjectOutlinePrimitive && usableObjectOutlinePrimitive->IsValidLowLevel())
		usableObjectOutlinePrimitive->SetRenderCustomDepth(false);
}

void USelectTargetComponent::OnUse(AActor* selectingActor, bool isSpecial) {
	if (!IsInUsableArea(selectingActor))
		return;

	MyUseEvent.Broadcast(selectingActor, isSpecial);
}

FDelegateHandle USelectTargetComponent::AddEventListener(FUseDelegate& UseDelegate)
{
	return MyUseEvent.Add(UseDelegate);
}

void USelectTargetComponent::RemoveEventListener(FDelegateHandle DelegateHandle)
{
	MyUseEvent.Remove(DelegateHandle);
}