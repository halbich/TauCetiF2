#include "GameSave.h"
#include "Private/HardcodedLevels/DefaultLevel.cpp"
#include "Private/HardcodedLevels/MainMenuLevel.cpp"
#include "Private/HardcodedLevels/MiniLevel.cpp"
#include "Private/HardcodedLevels/SuperSizeLevel.cpp"
#include "NewGameSaveHolder.h"

UNewGameSaveHolder::UNewGameSaveHolder()
{
	systemSaves = TMap<ENamedHardcodedLevel, FunctionPtrType>();

	systemSaves.Add(ENamedHardcodedLevel::MainMenu, &UNewGameSaveHolder::getMainMenuSave);
	systemSaves.Add(ENamedHardcodedLevel::DefaultLevel, &UNewGameSaveHolder::getDefaultGameSave);
	systemSaves.Add(ENamedHardcodedLevel::SuperSize, &UNewGameSaveHolder::getSuperSizeGameSave);
	systemSaves.Add(ENamedHardcodedLevel::MiniLevel, &UNewGameSaveHolder::getMiniLevelGameSave);
	systemSaves.Add(ENamedHardcodedLevel::EmptyLevel, &UNewGameSaveHolder::getEmptyGameSave);
}

UNewGameSaveHolder::~UNewGameSaveHolder()
{
	systemSaves.Empty();
}

UNewGameSaveHolder* UNewGameSaveHolder::Instance()
{
	return NewObject<UNewGameSaveHolder>();
}

TArray<USaveGameCarrier*> UNewGameSaveHolder::GetNewSaveGamesList()
{
	TArray<USaveGameCarrier*> result;
	auto i = Instance();

	for (auto level : systemSaves)
	{
		if (level.Key != ENamedHardcodedLevel::MainMenu)
			result.Add((i->* (level.Value))(false));
	}

	return result;
}

USaveGameCarrier* UNewGameSaveHolder::GetSaveForMainMenu()
{
	auto i = Instance();
	auto fn = i->systemSaves[ENamedHardcodedLevel::MainMenu];

	return (i->* (fn))(false);
}



USaveGameCarrier* UNewGameSaveHolder::getEmptyGameSave(bool full)
{
	auto c = USaveGameCarrier::GetEmptyCarrier();

	c->SaveName = NSLOCTEXT("TCF2LocSpace", "LC.SaveSystemEmpty", "Prázdná hra").ToString();
	c->SaveFullDescription = NSLOCTEXT("TCF2LocSpace", "LC.SaveSystemEmptyFullDescr", "Pokud se chceš jen tak proběhnout po planetě, tak začni tuto hru. Ale upozorňuji tě, že tam dlouho nepřežiješ.");

	c->FullFilePath = TEXT("_system_empty");
	c->CurrentTime = 34920.0f;
	c->IsSystemSave = true;
	c->SaveLoaded = true;
	c->HardcodedLevelName = ENamedHardcodedLevel::EmptyLevel;

	if (!full)
		return c;

	c->PlayerPosition = FVector(0, 0, 90);
	c->CurrentTime = 0.5f;
	c->PlayerUseFPSCamera = false;

	c->inventoryTags = makeDefault();

	auto UsedBlocks = &c->usedBlocks;

	c->PlayerOxygenComponent.CurrentObjectOxygen = 1000.0f;
	c->PlayerElectricityComponent.CurrentObjectEnergy = 150000.0f;

	return c;
}



USaveGameCarrier* UNewGameSaveHolder::GetSaveByPath(FString path)
{
	if (path.IsEmpty())
		return NULL;

	if (path.StartsWith(TEXT("_system_")))
	{
		for (auto level : systemSaves)
		{
			auto lvl = (this->* (level.Value))(false);
			if (lvl->FullFilePath == path)
				return (this->* (level.Value))(true);
		}
		checkNoEntry();
	}

	TArray<FText> errorList;
	auto currentSaves = USaveGameCarrier::GetSaveGameInfoList(errorList);

	for (auto save : currentSaves)
	{
		if (save->FullFilePath == path)
			return save;
	}

	return NULL;
}


