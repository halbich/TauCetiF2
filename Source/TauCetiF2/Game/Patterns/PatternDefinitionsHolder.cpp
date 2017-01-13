

#include "TauCetiF2.h"
#include "PatternDefinitionsHolder.h"

#pragma optimize("", off)


TArray<UBlockInfo*> UPatternDefinitionsHolder::DEBUGSpawnPatterns(const FVector & startingPoint)
{
	TArray<UBlockInfo*> result;

	auto startDim = BlockHelpers::GetLocalCoordinate(startingPoint);

	for (auto definition : UsedDefinitions)
	{
		auto spawnBlock = definition->objectDimensions;

		auto currentStartDim = startDim - spawnBlock->MinWorldCoord;
		definition->DEBUGSpawnPattern(currentStartDim, result);

		startDim += FVector(0, FMath::Abs(spawnBlock->MaxWorldCoord.Y - spawnBlock->MinWorldCoord.Y) + 5, 0);

	}

	return result;
}

TArray<UBlockInfo*> UPatternDefinitionsHolder::DEBUGUsedPatternElements(const FVector & startingPoint)
{

	TArray<UBlockInfo*> result;

	auto startDim = BlockHelpers::GetLocalCoordinate(startingPoint);

	for (auto It = SearchPatterns.CreateConstIterator(); It; ++It)
	{
		auto currentStartDim = startDim;

		auto gr = It->Value;

		float currentXMax(0);
		for (auto elem : gr->Patterns)
		{
			auto NewBlock = NewObject<UBlockInfo>(this, NAME_None, RF_NoFlags, elem->BlockInfo);
			NewBlock->Location = FVector::ZeroVector;

			// TODO
			/*auto box = BlockHelpers::GetSpawnBox(gr->Definition, NewBlock);

			auto localStartDim = currentStartDim - box->MinWorldCoord ;

			NewBlock->Location += localStartDim;
			result.Add(NewBlock);

			currentStartDim +=  FVector(0, FMath::Abs(box->MaxWorldCoord.Y - box->MinWorldCoord.Y) +5, 0);
			currentXMax = FMath::Max(currentXMax, FMath::Abs(box->MaxWorldCoord.X - box->MinWorldCoord.X));*/
		}

		startDim += FVector(currentXMax + 5, 0, 0);
	}

	return result;
}

UPatternDefinitionsHolder::UPatternDefinitionsHolder(const FObjectInitializer& ObjectInitializer) :
	UObject(ObjectInitializer)
{

}

void UPatternDefinitionsHolder::Init()
{
	//TODO
	/*auto blockDefinitions = FBlockDefinitionHolder::Instance().GetDefinition((int32)EBlockName::ConstructCubeSide);

	auto test1 = NewObject<UPatternDefinition>(this);

	test1->UsedBlocks.Add(make(EBlockName::ConstructCube, FVector(0, 0, 0), FVector(4, 4, 1), FRotator(0, 0, 0)));
	test1->UsedBlocks.Add(make(EBlockName::ConstructCube, FVector(1, 0, -1), FVector(4, 4, 1), FRotator(0, 0, 0)));
	test1->SetRotationsFull();
	test1->InitData();
	UsedDefinitions.Add(test1);


	auto test2 = NewObject<UPatternDefinition>(this);

	test2->UsedBlocks.Add(make(EBlockName::WindowCube, FVector(-11, 3, 0), FVector(4, 4, 1), FRotator(0, 0, 0)));
	test2->UsedBlocks.Add(make(EBlockName::ConstructCube, FVector(-11, 6, 0), FVector(4, 1, 1), FRotator(0, 0, 0)));
	test2->UsedBlocks.Add(make(EBlockName::ConstructCube, FVector(-10, 1, 0), FVector(1, 1, 4), FRotator(90, 0, 0)));
	test2->UsedBlocks.Add(make(EBlockName::ConstructCube, FVector(-13, 4, 0), FVector(1, 1, 4), FRotator(90, 90, 0)));
	test2->UsedBlocks.Add(make(EBlockName::ConstructCube, FVector(-8, 4, 0), FVector(1, 1, 4), FRotator(90, 90, 0)));
	test2->UsedBlocks.Add(make(EBlockName::BaseCube, FVector(-8, 1, 0), FVector(1, 1, 1), FRotator(0, 0, 0)));
	test2->UsedBlocks.Add(make(EBlockName::BaseCube, FVector(-13, 1, 0), FVector(1, 1, 1), FRotator(0, 0, 0)));
	test2->UsedBlocks.Add(make(EBlockName::BaseCube, FVector(-13, 6, 2), FVector(1, 1, 4), FRotator(0, 0, 0)));
	test2->UsedBlocks.Add(make(EBlockName::BaseCube, FVector(-13, 1, 2), FVector(1, 1, 4), FRotator(0, 0, 0)));
	test2->UsedBlocks.Add(make(EBlockName::BaseCube, FVector(-8, 1, 2), FVector(1, 1, 4), FRotator(0, 0, 0)));
	test2->UsedBlocks.Add(make(EBlockName::WindowCube, FVector(-10, 1, 2), FVector(4, 4, 1), FRotator(180, 0, 90)));
	test2->UsedBlocks.Add(make(EBlockName::WindowCube, FVector(-13, 4, 2), FVector(4, 4, 1), FRotator(180, 90, 90)));
	test2->UsedBlocks.Add(make(EBlockName::WindowCube, FVector(-11, 6, 2), FVector(4, 4, 1), FRotator(180, 180, 90)));
	test2->UsedBlocks.Add(make(EBlockName::BaseCube, FVector(-10, 6, 5), FVector(1, 1, 4), FRotator(90, 0, 0)));
	test2->UsedBlocks.Add(make(EBlockName::BaseCube, FVector(-13, 4, 5), FVector(1, 1, 4), FRotator(90, 90, 0)));
	test2->UsedBlocks.Add(make(EBlockName::ConstructCube, FVector(-8, 6, 2), FVector(1, 1, 4), FRotator(0, 0, 0)));
	test2->UsedBlocks.Add(make(EBlockName::WindowCube, FVector(-11, 3, 5), FVector(4, 4, 1), FRotator(0, 0, 0)));
	test2->UsedBlocks.Add(make(EBlockName::BaseCube, FVector(-8, 3, 5), FVector(1, 1, 4), FRotator(0, 180, 270)));
	test2->UsedBlocks.Add(make(EBlockName::BaseCube, FVector(-10, 1, 5), FVector(1, 1, 4), FRotator(0, 270, 270)));
	test2->UsedBlocks.Add(make(EBlockName::WindowCube, FVector(-8, 3, 3), FVector(4, 4, 1), FRotator(0, 90, 90)));
	test2->UsedBlocks.Add(make(EBlockName::BaseCube, FVector(-13, 6, 0), FVector(1, 1, 1), FRotator(0, 0, 0)));
	test2->UsedBlocks.Add(make(EBlockName::BaseCube, FVector(-8, 6, 0), FVector(1, 1, 1), FRotator(0, 0, 0)));

	test2->SetRotationsFullYaw();

	test2->InitData();
	UsedDefinitions.Add(test2);

	auto testSide = NewObject<UPatternDefinition>(this);

	testSide->UsedBlocks.Add(make(EBlockName::ConstructCubeSide, FVector(0, 0, 0), FVector(4, 4, 4), FRotator(0, 0, 0)));

	testSide->SetRotationsFullYaw();

	testSide->InitData();
	UsedDefinitions.Add(testSide);*/
}

UPatternDefinitionsHolder* UPatternDefinitionsHolder::Instance()
{

	static UPatternDefinitionsHolder* instance = nullptr;

	if (instance == nullptr || !instance->IsValidLowLevel())
	{
		instance = NewObject<UPatternDefinitionsHolder>();
		instance->Init();
		instance->checkAllInit();
	}

	return instance;

}


#pragma optimize("", on)