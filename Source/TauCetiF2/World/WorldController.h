

#pragma once

#include "GameFramework/Actor.h"
#include "Common/WorldObject.h"
#include "Blocks/CubeObject.h"
#include "Blocks/CubeSideObject.h"
#include "Blocks/CubeBodyObject.h"
#include "MinMaxBox.h"
#include "WorldController.generated.h"

UCLASS()
class TAUCETIF2_API AWorldController : public AActor
{
	GENERATED_UCLASS_BODY()

public:
	~AWorldController();

	UPROPERTY(BlueprintReadWrite, Category = WorldController)
		TArray<UBlockInfo*> UsedBlocks;

	UFUNCTION(BlueprintCallable, Category = WorldController)
		void LoadBlocksArray(UPARAM(ref)TArray<UBlockInfo*>& blocks);
	

	UFUNCTION(BlueprintCallable, Category = WorldController)
		void DEBUGShowMinMaxBoxes();

private:
		FMinMaxBox* RootBox;


	static FORCEINLINE UClass* GetClassByShape(const EShapeType shape) {

		switch (shape)
		{
		case EShapeType::Cube: return ACubeObject::StaticClass();
		case EShapeType::CubeSide: return ACubeSideObject::StaticClass();
		case EShapeType::CubeBody: return ACubeBodyObject::StaticClass();
		default:
			return nullptr;
			break;
		}

	}

};
