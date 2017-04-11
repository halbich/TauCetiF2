#include "Blocks.h"
#include "ARFilter.h"
#include "AssetRegistryModule.h"
#include "BlockHolderComponent.h"

#pragma optimize("", off)


// Sets default values for this component's properties
UBlockHolderComponent::UBlockHolderComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	ComponentTags.Add(TEXT("UBlockHolderComponent"));
}

// Called when the game starts
void UBlockHolderComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
}






//UObject * UBlockHolderComponent::MyGetAsset(FAssetData & asset) {
//	if (!asset.IsValid())
//	{
//		return NULL;
//	}
//
//	UObject * Asset = FindObject<UObject>(asset.GetPackage(), *asset.ObjectPath.ToString());
//	if (Asset == NULL)
//	{
//		FString tmpstring = asset.ObjectPath.ToString();
//		UObject * InOuter = NULL;
//		ResolveName(InOuter, tmpstring, true, true);
//
//		GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Magenta, TEXT("Now string is ") + tmpstring);
//		if (InOuter) {
//			GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Magenta, TEXT("InOuter->GetName is ") + InOuter->GetName());
//			Asset = StaticFindObjectFast(UObject::StaticClass(), InOuter, *tmpstring);
//		}
//		if (Asset) {
//			GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Black, TEXT("WOW I Have Load Something with StaticFindObjectFast!") + Asset->GetName());
//			return Asset;
//		}
//
//		Asset = (UObject *)StaticLoadObject(UBlueprint::StaticClass(), NULL, *asset.ObjectPath.ToString());
//		if (!Asset)
//			GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, TEXT(" FAILED LOAD Object like BLUEPRINT By StaticLoadObject"));
//		else {
//			GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Blue, TEXT(" LOADED like BLUEPRINT By StaticLoadObject"));
//			return Asset;
//		}
//
//		Asset = (UObject *)StaticLoadObject(UObject::StaticClass(), NULL, *asset.ObjectPath.ToString());
//		if (!Asset)
//			GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, TEXT(" FAILED LOAD Object like UOBJECT By StaticLoadObject"));
//		else {
//			GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Blue, TEXT(" LOADED like UOBJECT By StaticLoadObject"));
//			return Asset;
//		}
//
//		Asset = (UObject *)LoadObject<UBlueprint>(NULL, *asset.ObjectPath.ToString());
//		if (!Asset)
//			GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, TEXT(" FAILED LOAD Object like BLUERPRINT "));
//		else {
//			GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Blue, TEXT(" LOADED like BLUEPRINT By LoadObject"));
//			return Asset;
//		}
//
//		Asset = LoadObject<UObject>(NULL, *asset.ObjectPath.ToString());
//		if (!Asset)
//			GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, TEXT(" EVEN FAILED LOAD Object like UOBJECT "));
//		else {
//			GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Blue, TEXT(" LOADED like UOBJECT By LoadObject"));
//			return Asset;
//		}
//	}
//
//	return (UObject*)Asset;
//}

#pragma optimize("", on)