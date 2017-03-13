#include "GameSave.h"
#include "NewGameSaveHolder.h"

UNewGameSaveHolder* UNewGameSaveHolder::Instance()
{
	auto instance = NewObject<UNewGameSaveHolder>();
	instance->init();
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
	auto i = Instance();
	if (i && i->IsValidLowLevel())
	{
		if (i->newGameSaves.Num() > 0)
		{
			auto sav = i->newGameSaves[0];
			if (sav && sav->IsValidLowLevel())
				return sav;
		}
	}
	return nullptr;
}

USaveGameCarrier* UNewGameSaveHolder::GetSaveForMainMenu()
{
	return Instance()->mainMenuSave;
}

USaveGameCarrier* UNewGameSaveHolder::getDefaultGameSave()
{
	auto c = USaveGameCarrier::GetEmptyCarrier();

	c->SaveName = NSLOCTEXT("TCF2LocSpace", "LC.SaveSystemDefault", "Výchozí hra").ToString();
	c->FullFilePath = TEXT("_system_default");
	c->IsSystemSave = true;
	c->SaveLoaded = true;
	c->HardcodedLevelName = ENamedHardcodedLevel::DefaultLevel;

	c->PlayerPosition = FVector(-600, 0, 90);
	c->CurrentTime = 34920.0f;
	c->PlayerUseFPSCamera = true;

	c->PlayerOxygenComponent.CurrentFillingValue = 150.0f;
	c->PlayerElectricityComponent.CurrentObjectEnergy = 500.0f;

	auto invTags = UInventoryTags::GetDefault();

	USaveHelpers::ToContainer(c->inventoryTags, invTags);

	c->buildableBlocks.Add(makeBuildable(EmptyHandID, FVector(1, 1, 1)));
	c->buildableBlocks.Add(makeBuildable(DeleteID, FVector(1, 1, 1)));

	////TODO
	///*auto door = makeBuildable(EBlockName::Door, FVector(7, 7, 11));
	//door->AdditionalFlags.Add(GetNameForTag(ENamedTag::DoorOpening), (int32)EDoorOpening::Right);
	//door->Name = TEXT("Dveøe");
	//buildable->Add(door);*/

	auto cb = makeBuildable(CubeBaseID, FVector(20, 20, 4));
	//cb.AdditionalFlags.Add(TEXT("MyTag"), 0);
	c->buildableBlocks.Add(cb);

	c->buildableBlocks.Add(makeBuildable(CubePolycarbonateID, FVector(4, 4, 1)));

	c->buildableBlocks.Add(makeBuildable(CubeBuildingID, FVector(6, 6, 1)));
	c->buildableBlocks.Add(makeBuildable(CubeSideBaseID, FVector(20, 20, 4)));

	c->buildableBlocks.Add(makeBuildable(CubeSideBuildingID, FVector(1, 4, 1)));

	c->buildableBlocks.Add(makeBuildable(CubeBodyBuildingID, FVector(1, 1, 1)));

	c->buildableBlocks.Add(makeBuildable(OxygenTankID, FVector(2, 2, 2)));

	auto t = makeBuildable(TerminalID, FVector(1, 1, 1));
	t.AdditionalFlags.Add(TEXT("IsUsable"), 1);
	c->buildableBlocks.Add(t);

	c->buildableBlocks.Add(makeBuildable(WindowID, FVector(5, 1, 4)));

	auto UsedBlocks = &c->usedBlocks;

	UsedBlocks->Add(make(0, FVector(-27, -21, 1), FVector(20, 20, 4), FRotator(0, -90, 0)));
	UsedBlocks->Add(make(1, FVector(-27, -22, 4), FVector(6, 6, 1), FRotator(0, -90, 0)));
	UsedBlocks->Add(make(2, FVector(-27, -20, 7), FVector(4, 4, 1), FRotator(-90, 0, -90)));
	UsedBlocks->Add(make(2, FVector(-27, -25, 7), FVector(4, 4, 1), FRotator(-90, 0, -90)));
	UsedBlocks->Add(make(2, FVector(-24, -22, 7), FVector(4, 4, 1), FRotator(-90, 0, 180)));
	UsedBlocks->Add(make(2, FVector(-29, -22, 7), FVector(4, 4, 1), FRotator(-90, 0, 180)));
	UsedBlocks->Add(make(101, FVector(-29, -25, 7), FVector(4, 1, 1), FRotator(-90, 0, 180)));
	UsedBlocks->Add(make(101, FVector(-24, -25, 7), FVector(4, 1, 1), FRotator(-90, 0, -90)));
	UsedBlocks->Add(make(101, FVector(-24, -20, 7), FVector(4, 1, 1), FRotator(-90, 0, 0)));
	UsedBlocks->Add(make(101, FVector(-29, -20, 7), FVector(4, 1, 1), FRotator(-90, 0, 90)));
	UsedBlocks->Add(make(101, FVector(-29, -22, 9), FVector(4, 1, 1), FRotator(0, -90, -90)));
	UsedBlocks->Add(make(101, FVector(-26, -20, 9), FVector(4, 1, 1), FRotator(0, 180, -90)));
	UsedBlocks->Add(make(101, FVector(-24, -23, 9), FVector(4, 1, 1), FRotator(0, 90, -90)));
	UsedBlocks->Add(make(101, FVector(-27, -25, 9), FVector(4, 1, 1), FRotator(0, 0, -90)));
	UsedBlocks->Add(make(2, FVector(-26, -22, 9), FVector(4, 4, 1), FRotator(0, -180, 0)));
	UsedBlocks->Add(make(201, FVector(-29, -20, 9), FVector(1, 1, 1), FRotator(0, 90, 0)));
	UsedBlocks->Add(make(201, FVector(-29, -25, 9), FVector(1, 1, 1), FRotator(0, -180, 0)));
	UsedBlocks->Add(make(201, FVector(-24, -25, 9), FVector(1, 1, 1), FRotator(0, -90, 0)));
	UsedBlocks->Add(make(201, FVector(-24, -20, 9), FVector(1, 1, 1), FRotator(0, 0, 0)));

	auto ox1 = make(1500, FVector(-32, -12, 4), FVector(2, 2, 2), FRotator(0, -90, 0));
	makeOxygen(ox1, 5);
	UsedBlocks->Add(ox1);

	auto ox2 = make(1500, FVector(-18, -12, 4), FVector(2, 2, 2), FRotator(0, -90, 0));
	makeOxygen(ox2, 9);
	UsedBlocks->Add(ox2);




	auto oxInventory1 = makeInventoryBuildable(OxygenTankID, FVector(2, 2, 2));
	makeOxygen(oxInventory1, 7.5f);
	c->inventoryBuildableBlocks.Add(oxInventory1);


	return c;
}

USaveGameCarrier* UNewGameSaveHolder::getEmptyGameSave()
{
	auto c = USaveGameCarrier::GetEmptyCarrier();

	c->SaveName = NSLOCTEXT("TCF2LocSpace", "LC.SaveSystemEmpty", "Prázdná hra").ToString();
	c->CurrentTime = 34920.0f;
	c->IsSystemSave = true;
	c->SaveLoaded = true;
	c->HardcodedLevelName = ENamedHardcodedLevel::DefaultLevel;

	c->PlayerPosition = FVector(0, 0, 90);
	c->CurrentTime = 0.5f;
	c->PlayerUseFPSCamera = false;

	return c;
}

USaveGameCarrier* UNewGameSaveHolder::getMainMenuSave()
{
	auto c = USaveGameCarrier::GetEmptyCarrier();

	c->SaveName = TEXT("Main Menu Save");
	c->IsSystemSave = true;
	c->SaveLoaded = true;
	c->HardcodedLevelName = ENamedHardcodedLevel::MainMenu;

	c->PlayerPosition = FVector(-600, 0, 90);
	c->CurrentTime = 0.5f;
	c->PlayerUseFPSCamera = true;

	//auto UsedBlocks = &c->UsedBlocks;

	// TODO
	/*UsedBlocks->Add(make(EBlockName::WindowCube, FVector(-11, 3, 0), FVector(4, 4, 1), FRotator(0, 0, 0)));
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
	UsedBlocks->Add(make(EBlockName::BaseCube, FVector(-8, 6, 0), FVector(1, 1, 1), FRotator(0, 0, 0)));*/

	return c;
}

USaveGameCarrier* UNewGameSaveHolder::GetSaveByPath(FString path)
{
	if (path.IsEmpty())
		return NULL;

	if (path == TEXT("_system_default"))
		return getDefaultGameSave();

	if (path == TEXT("_system_empty"))
		return getEmptyGameSave();

	TArray<FText> errorList;
	auto currentSaves = USaveGameCarrier::GetSaveGameInfoList(errorList);

	for (auto save : currentSaves)
	{
		if (save->FullFilePath == path)
			return save;
	}

	return NULL;

}