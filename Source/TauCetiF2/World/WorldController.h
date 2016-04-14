

#pragma once

#include "GameFramework/Actor.h"
#include "Common/WorldObject.h"
#include "Blocks/CubeObject.h"
#include "Blocks/CubeSideObject.h"
#include "Blocks/CubeBodyObject.h"
#include "Blocks/TerminalObject.h"
#include "MinMaxBox.h"
#include "Blocks/FBlockDefinition.h"
#include "Blocks/FBlockDefinitionHolder.h"

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
