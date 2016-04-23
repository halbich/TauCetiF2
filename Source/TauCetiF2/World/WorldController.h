

#pragma once

#include "GameFramework/Actor.h"
#include "Blocks/WorldObject.h"
#include "Helpers/BlockHelpers.h"
#include "Helpers/WorldHelpers.h"
#include "MinMaxBox.h"
#include "KDTree.h"
#include "Blocks/Definitions/FBlockDefinition.h"
#include "Blocks/Definitions/FBlockDefinitionHolder.h"
#include "Game/Patterns/PatternDefinition.h"
#include "Game/Patterns/PatternDefinitionsHolder.h"

#include "WorldController.generated.h"

UCLASS()
class TAUCETIF2_API AWorldController : public AActor
{
	GENERATED_UCLASS_BODY()

public:

	UPROPERTY(BlueprintReadWrite, Category = WorldController)
		TArray<UBlockInfo*> UsedBlocks;

	UFUNCTION(BlueprintCallable, Category = WorldController)
		void LoadBlocksArray(UPARAM(ref)TArray<UBlockInfo*>& blocks);


	UFUNCTION(BlueprintCallable, Category = WorldController)
		void DEBUGShowMinMaxBoxes();

	UFUNCTION(BlueprintCallable, Category = WorldController)
		void DEBUGHideMinMaxBoxes();

	AWorldObject* SpawnWorldObject(UWorld* world, UBlockInfo* block, bool addToRoot);

	bool DestroyWorldObject(AWorldObject* object);

	FORCEINLINE bool IsValidSpawnPoint(const UMinMaxBox* box)
	{
		return RootBox->IsPlaceEmpty(box);
	}

	UPROPERTY()
		UKDTree* RootBox;


private:

	UPROPERTY()
		bool debugBoxesShown;


	

};
