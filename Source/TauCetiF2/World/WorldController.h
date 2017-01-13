

#pragma once

#include "GameFramework/Actor.h"
#include "Blocks/WorldObject.h"
#include "Blocks/Public/Components/BlockHolderComponent.h"
#include "Helpers/BlockHelpers.h"
#include "Helpers/WorldHelpers.h"
#include "MinMaxBox.h"
#include "KDTree.h"
#include "Blocks/Definitions/FBlockDefinition.h"
#include "Blocks/Definitions/FBlockDefinitionHolder.h"
#include "Game/Patterns/PatternDefinition.h"
#include "Game/Patterns/PatternDefinitionsHolder.h"
#include "Components/BaseControlComponent.h"

#include "WorldController.generated.h"

UCLASS()
class TAUCETIF2_API AWorldController : public AActor
{
	GENERATED_UCLASS_BODY()

public:


	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReasonType) override;

	UPROPERTY(BlueprintReadWrite, Transient, Category = WorldController)
		TArray<UBlockInfo*> UsedBlocks;

	UFUNCTION(BlueprintCallable, Category = WorldController)
		void LoadBlocksArray(UPARAM(ref)TArray<UBlockInfo*>& blocks);

	UFUNCTION(BlueprintCallable, Category = WorldController)
		void DEBUGShowMinMaxBoxes();

	UFUNCTION(BlueprintCallable, Category = WorldController)
		void DEBUGHideMinMaxBoxes();

	UFUNCTION(BlueprintCallable, Category = WorldController)
		void DEBUGSpawnPatterns(const FVector & startingPoint);

	UFUNCTION(BlueprintCallable, Category = WorldController)
		void DEBUGUsedPatternElements(const FVector & startingPoint);

	ABlock* SpawnWorldObject(UWorld* world, UBlockInfo* block, bool addToRoot);

	bool DestroyWorldObject(/*AWorldObject*/UObject* object);

	FORCEINLINE bool IsValidSpawnPoint(const UMinMaxBox* box)
	{
		return RootBox->IsPlaceEmpty(box);
	}

	UPROPERTY(Transient)
		UKDTree* RootBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = WorldController, meta = (AllowPrivateAccess = "true"))
		UBlockHolderComponent* BlockHolder;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = WorldController, meta = (AllowPrivateAccess = "true"))
		UBaseControlComponent* BaseControl;

private:

	UPROPERTY()
		bool debugBoxesShown;

	//TEMPLATE Load Obj From Path
	template <typename ObjClass>
	static FORCEINLINE ObjClass* LoadObjFromPath(const FName& Path)
	{
		if (Path == NAME_None) return NULL;
		//~

		return Cast<ObjClass>(StaticLoadObject(ObjClass::StaticClass(), NULL, *Path.ToString()));
	}


};
