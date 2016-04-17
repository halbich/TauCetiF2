

#pragma once

#include "GameFramework/Actor.h"
#include "Blocks/WorldObject.h"
#include "Blocks/Implementations/BaseShapes/CubeObject.h"
#include "Blocks/Implementations/BaseShapes/CubeSideObject.h"
#include "Blocks/Implementations/BaseShapes/CubeBodyObject.h"
#include "Blocks/Implementations/Custom/TerminalObject.h"
#include "MinMaxBox.h"
#include "KDTree.h"
#include "Blocks/Definitions/FBlockDefinition.h"
#include "Blocks/Definitions/FBlockDefinitionHolder.h"

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

	FORCEINLINE bool IsValidSpawnPoint(const FTransform& transform)
	{
		UMinMaxBox* box = NewObject<UMinMaxBox>(this)->InitBox(transform);
		return RootBox->IsPlaceEmpty(box);
	}

	UPROPERTY()
		UKDTree* RootBox;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:

	UPROPERTY()
		bool debugBoxesShown;


	static FORCEINLINE UClass* GetClassByShape(const FBlockDefinition& definition) {

		switch (definition.ShapeType)
		{
		case EShapeType::Cube: return ACubeObject::StaticClass();
		case EShapeType::CubeSide: return ACubeSideObject::StaticClass();
		case EShapeType::CubeBody: return ACubeBodyObject::StaticClass();
		case EShapeType::Custom: {

			if (definition.ID == (uint32)EBlockName::Terminal)
				return ATerminalObject::StaticClass();

			break;
		}

		default:

			return nullptr;
			break;
		}

		return nullptr;
	}

};
