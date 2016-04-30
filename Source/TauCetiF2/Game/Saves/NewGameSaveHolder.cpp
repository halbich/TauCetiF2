

#include "TauCetiF2.h"
#include "NewGameSaveHolder.h"


UNewGameSaveHolder* UNewGameSaveHolder::Instance()
{

	static UNewGameSaveHolder* instance = nullptr;

	if (instance == nullptr || !instance->IsValidLowLevel())
	{
		instance = NewObject<UNewGameSaveHolder>();
		instance->init();
	}

	return instance;

}

void UNewGameSaveHolder::init()
{
	fillingFunctions[(uint8)ENamedHardcodedLevel::MainMenu] = &UNewGameSaveHolder::getMainMenuSave;
	fillingFunctions[(uint8)ENamedHardcodedLevel::DefaultLevel] = &UNewGameSaveHolder::getDefaultGameSave;
	fillingFunctions[(uint8)ENamedHardcodedLevel::EmptyLevel] = &UNewGameSaveHolder::getEmptyGameSave;


	mainMenuSave = (this->* (fillingFunctions[(uint8)ENamedHardcodedLevel::MainMenu]))();

	for (uint8 i = ((uint8)ENamedHardcodedLevel::MainMenu + 1); i < (uint8)ENamedHardcodedLevel::HardcodedLevelsMax; i++)
	{
		newGameSaves.Add((this->* (fillingFunctions[i]))());
	}

}


TArray<USaveGameCarrier*> UNewGameSaveHolder::GetNewSaveGamesList()
{
	return Instance()->newGameSaves;
}


USaveGameCarrier * UNewGameSaveHolder::DEBUG_GetTestSave()
{
	return Instance()->newGameSaves[0];
}

USaveGameCarrier* UNewGameSaveHolder::GetSaveForMainMenu()
{
	return Instance()->mainMenuSave;

}


USaveGameCarrier* UNewGameSaveHolder::getDefaultGameSave()
{
	auto c = USaveGameCarrier::GetEmptyCarrier();

	c->SaveName = TEXT("Bìžná hra");
	c->IsSystemSave = true;
	c->HardcodedLevelName = ENamedHardcodedLevel::DefaultLevel;


	c->PlayerPosition = FVector(-600, 0, 90);
	c->PartOfDay = 0.5f;
	c->PlayerUseFPSCamera = true;


	c->addSystemBuildableActions();

	auto BuildableBlocks = &c->BuildableBlocks;

	BuildableBlocks->Add(makeBuildable(EBlockName::BaseCube, FVector(1, 1, 1)));
	BuildableBlocks->Add(makeBuildable(EBlockName::BaseCube, FVector(1, 1, 4)));
	BuildableBlocks->Add(makeBuildable(EBlockName::WindowCube, FVector(4, 4, 1)));
	//BuildableBlocks.Add(makeBuildable(EBlockName::Terminal, FVector(1, 1, 1)));
	BuildableBlocks->Add(makeBuildable(EBlockName::ConstructCube, FVector(1, 1, 1)));
	BuildableBlocks->Add(makeBuildable(EBlockName::ConstructCubeSide, FVector(2, 2, 2)));
	BuildableBlocks->Add(makeBuildable(EBlockName::ConstructCubeBody, FVector(5, 5, 5)));

	auto build1 = makeBuildable(EBlockName::Door, FVector(1, 1, 1));
	build1->AdditionalFlags.Add(TEXT("doorOpening"), (int32)EDoorOpening::Left);
	build1->DefaultBuildingRotation = FRotator(0, 180, 0);
	//BuildableBlocks.Add(build1);

	auto build2 = makeBuildable(EBlockName::Door, FVector(1, 1, 1));
	build2->AdditionalFlags.Add(TEXT("doorOpening"), (int32)EDoorOpening::Right);
	build2->DefaultBuildingRotation = FRotator(0, 180, 0);
	//BuildableBlocks.Add(build2);

	auto door1 = make(EBlockName::Door, FVector(-20, -20, 5), FVector(1, 1, 1), FRotator(0, 0, 0));
	door1->AdditionalFlags.Add(TEXT("doorOpening"), (int32)EDoorOpening::Left);
	//UsedBlocks.Add(door1);

	auto door2 = make(EBlockName::Door, FVector(-20, 30, 5), FVector(1, 1, 1), FRotator(0, 180, 0));
	door2->AdditionalFlags.Add(TEXT("doorOpening"), (int32)EDoorOpening::Right);

	//UsedBlocks.Add(door2);

	//UsedBlocks.Add(make(EBlockName::Terminal, FVector(-20, -30, 5), FVector(1, 1, 1), FRotator(0, 0, 0)));

	auto UsedBlocks = &c->UsedBlocks;
	UsedBlocks->Add(make(EBlockName::ConstructCubeSide, FVector(0, 0, 5), FVector(1, 4, 2), FRotator(0, 0, 0)));

	UsedBlocks->Add(make(EBlockName::WindowPowered, FVector(-10, -10, 9), FVector(10, 1, 5), FRotator(0, 0, 0)));

	return c;

	UsedBlocks->Add(make(EBlockName::WindowCube, FVector(-11, 3, 0), FVector(4, 4, 1), FRotator(0, 0, 0)));
	UsedBlocks->Add(make(EBlockName::BaseCube, FVector(-10, 6, 0), FVector(1, 1, 4), FRotator(90, 0, 0)));
	UsedBlocks->Add(make(EBlockName::BaseCube, FVector(-10, 1, 0), FVector(1, 1, 4), FRotator(90, 0, 0)));
	UsedBlocks->Add(make(EBlockName::BaseCube, FVector(-13, 4, 0), FVector(1, 1, 4), FRotator(90, 90, 0)));
	UsedBlocks->Add(make(EBlockName::BaseCube, FVector(-8, 4, 0), FVector(1, 1, 4), FRotator(90, 90, 0)));
	UsedBlocks->Add(make(EBlockName::BaseCube, FVector(-8, 1, 0), FVector(1, 1, 1), FRotator(0, 0, 0)));
	UsedBlocks->Add(make(EBlockName::BaseCube, FVector(-13, 1, 0), FVector(1, 1, 1), FRotator(0, 0, 0)));
	UsedBlocks->Add(make(EBlockName::BaseCube, FVector(-13, 6, 2), FVector(1, 1, 4), FRotator(0, 0, 0)));
	UsedBlocks->Add(make(EBlockName::BaseCube, FVector(-13, 1, 2), FVector(1, 1, 4), FRotator(0, 0, 0)));
	UsedBlocks->Add(make(EBlockName::BaseCube, FVector(-8, 1, 2), FVector(1, 1, 4), FRotator(0, 0, 0)));
	UsedBlocks->Add(make(EBlockName::WindowCube, FVector(-10, 1, 2), FVector(4, 4, 1), FRotator(180, 0, 90)));
	UsedBlocks->Add(make(EBlockName::WindowCube, FVector(-13, 4, 2), FVector(4, 4, 1), FRotator(180, 90, 90)));
	UsedBlocks->Add(make(EBlockName::WindowCube, FVector(-11, 6, 2), FVector(4, 4, 1), FRotator(180, 180, 90)));
	UsedBlocks->Add(make(EBlockName::BaseCube, FVector(-10, 6, 5), FVector(1, 1, 4), FRotator(90, 0, 0)));
	UsedBlocks->Add(make(EBlockName::BaseCube, FVector(-13, 4, 5), FVector(1, 1, 4), FRotator(90, 90, 0)));
	UsedBlocks->Add(make(EBlockName::BaseCube, FVector(-8, 6, 2), FVector(1, 1, 4), FRotator(0, 0, 0)));
	UsedBlocks->Add(make(EBlockName::WindowCube, FVector(-11, 3, 5), FVector(4, 4, 1), FRotator(0, 0, 0)));
	UsedBlocks->Add(make(EBlockName::BaseCube, FVector(-8, 3, 5), FVector(1, 1, 4), FRotator(0, 180, 270)));
	UsedBlocks->Add(make(EBlockName::BaseCube, FVector(-10, 1, 5), FVector(1, 1, 4), FRotator(0, 270, 270)));
	UsedBlocks->Add(make(EBlockName::WindowCube, FVector(-8, 3, 3), FVector(4, 4, 1), FRotator(0, 90, 90)));
	UsedBlocks->Add(make(EBlockName::BaseCube, FVector(-13, 6, 0), FVector(1, 1, 1), FRotator(0, 0, 0)));
	UsedBlocks->Add(make(EBlockName::BaseCube, FVector(-8, 6, 0), FVector(1, 1, 1), FRotator(0, 0, 0)));


}

USaveGameCarrier* UNewGameSaveHolder::getEmptyGameSave()
{
	auto c = USaveGameCarrier::GetEmptyCarrier();

	c->SaveName = TEXT("Prázdná hra");
	c->IsSystemSave = true;
	c->HardcodedLevelName = ENamedHardcodedLevel::DefaultLevel;

	c->PlayerPosition = FVector(0, 0, 90);
	c->PartOfDay = 0.5f;
	c->PlayerUseFPSCamera = false;


	c->addSystemBuildableActions();

	return c;

}

USaveGameCarrier* UNewGameSaveHolder::getMainMenuSave()
{
	auto c = USaveGameCarrier::GetEmptyCarrier();

	c->SaveName = TEXT("Main Menu Save");
	c->IsSystemSave = true;
	c->HardcodedLevelName = ENamedHardcodedLevel::MainMenu;


	c->PlayerPosition = FVector(-600, 0, 90);
	c->PartOfDay = 0.5f;
	c->PlayerUseFPSCamera = true;




	auto UsedBlocks = &c->UsedBlocks;

	UsedBlocks->Add(make(EBlockName::WindowCube, FVector(-11, 3, 0), FVector(4, 4, 1), FRotator(0, 0, 0)));
	UsedBlocks->Add(make(EBlockName::BaseCube, FVector(-10, 6, 0), FVector(1, 1, 4), FRotator(90, 0, 0)));
	UsedBlocks->Add(make(EBlockName::BaseCube, FVector(-10, 1, 0), FVector(1, 1, 4), FRotator(90, 0, 0)));
	UsedBlocks->Add(make(EBlockName::BaseCube, FVector(-13, 4, 0), FVector(1, 1, 4), FRotator(90, 90, 0)));
	UsedBlocks->Add(make(EBlockName::BaseCube, FVector(-8, 4, 0), FVector(1, 1, 4), FRotator(90, 90, 0)));
	UsedBlocks->Add(make(EBlockName::BaseCube, FVector(-8, 1, 0), FVector(1, 1, 1), FRotator(0, 0, 0)));
	UsedBlocks->Add(make(EBlockName::BaseCube, FVector(-13, 1, 0), FVector(1, 1, 1), FRotator(0, 0, 0)));
	UsedBlocks->Add(make(EBlockName::BaseCube, FVector(-13, 6, 2), FVector(1, 1, 4), FRotator(0, 0, 0)));
	UsedBlocks->Add(make(EBlockName::BaseCube, FVector(-13, 1, 2), FVector(1, 1, 4), FRotator(0, 0, 0)));
	UsedBlocks->Add(make(EBlockName::BaseCube, FVector(-8, 1, 2), FVector(1, 1, 4), FRotator(0, 0, 0)));
	UsedBlocks->Add(make(EBlockName::WindowCube, FVector(-10, 1, 2), FVector(4, 4, 1), FRotator(180, 0, 90)));
	UsedBlocks->Add(make(EBlockName::WindowCube, FVector(-13, 4, 2), FVector(4, 4, 1), FRotator(180, 90, 90)));
	UsedBlocks->Add(make(EBlockName::WindowCube, FVector(-11, 6, 2), FVector(4, 4, 1), FRotator(180, 180, 90)));
	UsedBlocks->Add(make(EBlockName::BaseCube, FVector(-10, 6, 5), FVector(1, 1, 4), FRotator(90, 0, 0)));
	UsedBlocks->Add(make(EBlockName::BaseCube, FVector(-13, 4, 5), FVector(1, 1, 4), FRotator(90, 90, 0)));
	UsedBlocks->Add(make(EBlockName::BaseCube, FVector(-8, 6, 2), FVector(1, 1, 4), FRotator(0, 0, 0)));
	UsedBlocks->Add(make(EBlockName::WindowCube, FVector(-11, 3, 5), FVector(4, 4, 1), FRotator(0, 0, 0)));
	UsedBlocks->Add(make(EBlockName::BaseCube, FVector(-8, 3, 5), FVector(1, 1, 4), FRotator(0, 180, 270)));
	UsedBlocks->Add(make(EBlockName::BaseCube, FVector(-10, 1, 5), FVector(1, 1, 4), FRotator(0, 270, 270)));
	UsedBlocks->Add(make(EBlockName::WindowCube, FVector(-8, 3, 3), FVector(4, 4, 1), FRotator(0, 90, 90)));
	UsedBlocks->Add(make(EBlockName::BaseCube, FVector(-13, 6, 0), FVector(1, 1, 1), FRotator(0, 0, 0)));
	UsedBlocks->Add(make(EBlockName::BaseCube, FVector(-8, 6, 0), FVector(1, 1, 1), FRotator(0, 0, 0)));

	return c;
}