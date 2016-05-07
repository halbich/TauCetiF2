

#pragma once

#include "Object.h"

#include "Game/Inventory/TagGroup.h"
#include "InventoryTagGroup.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class TAUCETIF2_API UInventoryTagGroup : public UObject
{
	GENERATED_BODY()
public:

	

	UPROPERTY(BlueprintReadWrite, Category = InventoryTagGroup)
		TArray<UTagGroup*> GroupList;

	UPROPERTY(BlueprintReadOnly, Category = InventoryTagGroup)
		int32 Index;

	UPROPERTY(BlueprintReadWrite, Category = InventoryTagGroup)
		FString Name;

	UPROPERTY(BlueprintReadWrite, Category = InventoryTagGroup)
		bool IsEnabled;
	
};
