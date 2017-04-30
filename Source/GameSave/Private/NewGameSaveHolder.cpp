#include "GameSave.h"
#include "NewGameSaveHolder.h"

UNewGameSaveHolder::UNewGameSaveHolder()
{
	systemSaves = TMap<ENamedHardcodedLevel, FunctionPtrType>();

	systemSaves.Add(ENamedHardcodedLevel::MainMenu, &UNewGameSaveHolder::getMainMenuSave);
	systemSaves.Add(ENamedHardcodedLevel::DefaultLevel, &UNewGameSaveHolder::getDefaultGameSave);
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

USaveGameCarrier* UNewGameSaveHolder::getDefaultGameSave(bool full)
{
	return getMiniLevelGameSave(full);

	auto c = USaveGameCarrier::GetEmptyCarrier();

	c->SaveName = NSLOCTEXT("TCF2LocSpace", "LC.SaveSystemDefault", "Výchozí hra").ToString();
	c->FullFilePath = TEXT("_system_default");
	c->IsSystemSave = true;
	c->SaveLoaded = true;
	c->HardcodedLevelName = ENamedHardcodedLevel::DefaultLevel;

	if (!full)
		return c;

	//c->CurrentTime = 14400.0f; 
	c->CurrentTime = 34920.0f;
	c->PlayerUseFPSCamera = true;
	c->IsCreativeMode = true;
	c->PlayerHealth = 1000;

	c->PlayerOxygenComponent.CurrentObjectOxygen = 1000.0f;
	c->PlayerElectricityComponent.CurrentObjectEnergy = 150000.0f;

	c->inventoryTags = makeDefault();

	c->buildableBlocks.Add(makeBuildable(DeleteID, FVector(1, 1, 1)));
	c->buildableBlocks.Add(makeBuildable(TerminalID, FVector(1, 1, 1)));

	c->buildableBlocks.Add(makeBuildable(OxygenTankID, FVector(2, 2, 2)));
	c->buildableBlocks.Add(makeBuildable(SwitcherID, FVector(1, 1, 1)));
	c->buildableBlocks.Add(makeBuildable(LightSmallID, FVector(1, 1, 1)));
	c->buildableBlocks.Add(makeBuildable(CreatorID, FVector(3, 3, 2)));

	auto UsedBlocks = &c->usedBlocks;

	auto b_0 = make(0, FVector(-6, -2, 1), FVector(20, 20, 4), FRotator(0, 0, 0), 16000.000000, ""); b_0.HasElectricityData = true; b_0.ElectricityInfo.CurrentObjectEnergy = 0.000000;  UsedBlocks->Add(b_0);
	auto b_1 = make(0, FVector(14, -2, 1), FVector(20, 20, 4), FRotator(0, 0, 0), 16000.000000, ""); b_1.HasElectricityData = true; b_1.ElectricityInfo.CurrentObjectEnergy = 0.000000;  UsedBlocks->Add(b_1);
	auto b_2 = make(0, FVector(14, -22, 1), FVector(20, 20, 4), FRotator(0, 0, 0), 16000.000000, ""); b_2.HasElectricityData = true; b_2.ElectricityInfo.CurrentObjectEnergy = 0.000000;  UsedBlocks->Add(b_2);
	auto b_3 = make(0, FVector(-6, -22, 1), FVector(20, 20, 4), FRotator(0, 0, 0), 16000.000000, ""); b_3.HasElectricityData = true; b_3.ElectricityInfo.CurrentObjectEnergy = 0.000000;  UsedBlocks->Add(b_3);
	auto b_4 = make(500, FVector(0, -17, 6), FVector(1, 1, 1), FRotator(0, 0, 0), 10.000000, ""); b_4.HasElectricityData = true; b_4.ElectricityInfo.CurrentObjectEnergy = 100000.000000;  UsedBlocks->Add(b_4);
	auto b_5 = make(1, FVector(-15, -30, 9), FVector(20, 1, 11), FRotator(0, 90, 0), 2200.000000, ""); b_5.HasElectricityData = true; b_5.ElectricityInfo.CurrentObjectEnergy = 0.000000;  UsedBlocks->Add(b_5);
	auto b_6 = make(1, FVector(24, -21, 6), FVector(20, 1, 6), FRotator(0, -90, 0), 1200.000000, ""); b_6.HasElectricityData = true; b_6.ElectricityInfo.CurrentObjectEnergy = 0.000000;  UsedBlocks->Add(b_6);
	auto b_7 = make(1, FVector(24, -1, 6), FVector(20, 1, 6), FRotator(0, -90, 0), 1200.000000, ""); b_7.HasElectricityData = true; b_7.ElectricityInfo.CurrentObjectEnergy = 0.000000;  UsedBlocks->Add(b_7);
	auto b_8 = make(1, FVector(14, -31, 6), FVector(20, 1, 6), FRotator(0, 180, 0), 1200.000000, ""); b_8.HasElectricityData = true; b_8.ElectricityInfo.CurrentObjectEnergy = 0.000000;  UsedBlocks->Add(b_8);
	auto b_9 = make(1, FVector(-15, -1, 6), FVector(20, 1, 6), FRotator(0, -90, 0), 1200.000000, ""); b_9.HasElectricityData = true; b_9.ElectricityInfo.CurrentObjectEnergy = 0.000000;  UsedBlocks->Add(b_9);
	auto b_10 = make(600, FVector(-12, -39, 9), FVector(6, 1, 11), FRotator(0, 0, 0), 660.000000, ""); b_10.HasElectricityData = true; b_10.ElectricityInfo.CurrentObjectEnergy = 26400.000000;  UsedBlocks->Add(b_10);
	auto b_11 = make(100, FVector(10, -35, 1), FVector(8, 14, 4), FRotator(0, -90, 0), 1493.333374, ""); b_11.HasElectricityData = true; b_11.ElectricityInfo.CurrentObjectEnergy = 0.000000;  UsedBlocks->Add(b_11);
	auto b_12 = make(1000, FVector(25, -13, 8), FVector(1, 1, 1), FRotator(0, 90, -90), 10.000000, ""); b_12.BlockSpecificData.Add(TEXT("IsAutoregulated"), TEXT("1")); b_12.BlockSpecificData.Add(TEXT("IsOn"), TEXT("1"));
	b_12.HasElectricityData = true; b_12.ElectricityInfo.CurrentObjectEnergy = 2000.000000;
	b_12.HasRelationshipData = true; FGuid b_12_rel_id; FGuid::Parse("703EFD1844A5E576A5932288A38E20B9", b_12_rel_id); b_12.RelationshipInfo.ID = b_12_rel_id;
	FRelationshipInfo b_12_rel0; FGuid b_12_rel0_id; FGuid::Parse("5CCE09794A8BA7EDD12133AFE8BA3E40", b_12_rel0_id); b_12_rel0.TargetID = b_12_rel0_id; b_12_rel0.RelationshipType = 0;
	b_12.RelationshipInfo.Relationships.Add(b_12_rel0); UsedBlocks->Add(b_12);
	auto b_13 = make(1000, FVector(13, 9, 3), FVector(1, 1, 1), FRotator(0, -180, -90), 10.000000, ""); b_13.BlockSpecificData.Add(TEXT("IsAutoregulated"), TEXT("1")); b_13.BlockSpecificData.Add(TEXT("IsOn"), TEXT("1"));
	b_13.HasElectricityData = true; b_13.ElectricityInfo.CurrentObjectEnergy = 2000.000000;
	b_13.HasRelationshipData = true; FGuid b_13_rel_id; FGuid::Parse("2EC04B3D4081FD6E3E1AB98141F4AD19", b_13_rel_id); b_13.RelationshipInfo.ID = b_13_rel_id;
	FRelationshipInfo b_13_rel0; FGuid b_13_rel0_id; FGuid::Parse("5CCE09794A8BA7EDD12133AFE8BA3E40", b_13_rel0_id); b_13_rel0.TargetID = b_13_rel0_id; b_13_rel0.RelationshipType = 0;
	b_13.RelationshipInfo.Relationships.Add(b_13_rel0); UsedBlocks->Add(b_13);
	auto b_14 = make(1000, FVector(-16, -10, 2), FVector(1, 1, 1), FRotator(0, -90, -90), 10.000000, ""); b_14.BlockSpecificData.Add(TEXT("IsAutoregulated"), TEXT("1")); b_14.BlockSpecificData.Add(TEXT("IsOn"), TEXT("1"));
	b_14.HasElectricityData = true; b_14.ElectricityInfo.CurrentObjectEnergy = 2000.000000;
	b_14.HasRelationshipData = true; FGuid b_14_rel_id; FGuid::Parse("4C5484244F03C6E7F8402D8363064A80", b_14_rel_id); b_14.RelationshipInfo.ID = b_14_rel_id;
	FRelationshipInfo b_14_rel0; FGuid b_14_rel0_id; FGuid::Parse("5CCE09794A8BA7EDD12133AFE8BA3E40", b_14_rel0_id); b_14_rel0.TargetID = b_14_rel0_id; b_14_rel0.RelationshipType = 0;
	b_14.RelationshipInfo.Relationships.Add(b_14_rel0); UsedBlocks->Add(b_14);
	auto b_15 = make(0, FVector(-6, -35, 1), FVector(19, 8, 4), FRotator(0, 180, 0), 6080.000000, ""); b_15.HasElectricityData = true; b_15.ElectricityInfo.CurrentObjectEnergy = 0.000000;  UsedBlocks->Add(b_15);
	auto b_16 = make(700, FVector(0, -35, 9), FVector(7, 7, 11), FRotator(0, 90, 0), 10.000000, ""); b_16.AdditionalFlags.Add(TEXT("DoorOpening"), 0);
	b_16.BlockSpecificData.Add(TEXT("DoorState"), TEXT("2")); b_16.BlockSpecificData.Add(TEXT("DoorYaw"), TEXT("89.0"));
	b_16.HasElectricityData = true; b_16.ElectricityInfo.CurrentObjectEnergy = 0.000000;  UsedBlocks->Add(b_16);
	auto b_17 = make(1, FVector(3, -39, 9), FVector(1, 1, 11), FRotator(0, 0, 0), 110.000000, ""); b_17.HasElectricityData = true; b_17.ElectricityInfo.CurrentObjectEnergy = 0.000000;  UsedBlocks->Add(b_17);
	auto b_18 = make(600, FVector(0, -39, 9), FVector(6, 1, 11), FRotator(0, -180, 0), 660.000000, ""); b_18.HasElectricityData = true; b_18.ElectricityInfo.CurrentObjectEnergy = 26400.000000;  UsedBlocks->Add(b_18);
	auto b_19 = make(600, FVector(-6, -39, 9), FVector(5, 1, 11), FRotator(0, 0, 0), 550.000000, ""); b_19.HasElectricityData = true; b_19.ElectricityInfo.CurrentObjectEnergy = 22000.000000;  UsedBlocks->Add(b_19);
	auto b_20 = make(2, FVector(4, -27, 4), FVector(2, 7, 1), FRotator(0, 0, 0), 140.000000, "");  UsedBlocks->Add(b_20);
	auto b_21 = make(2, FVector(6, -27, 5), FVector(2, 7, 1), FRotator(0, 0, 0), 140.000000, "");  UsedBlocks->Add(b_21);
	auto b_22 = make(2, FVector(8, -27, 6), FVector(2, 7, 1), FRotator(0, 0, 0), 140.000000, "");  UsedBlocks->Add(b_22);
	auto b_23 = make(2, FVector(10, -27, 7), FVector(2, 7, 1), FRotator(0, 0, 0), 140.000000, "");  UsedBlocks->Add(b_23);
	auto b_24 = make(2, FVector(12, -27, 8), FVector(2, 7, 1), FRotator(0, 0, 0), 140.000000, "");  UsedBlocks->Add(b_24);
	auto b_25 = make(2, FVector(14, -27, 9), FVector(2, 7, 1), FRotator(0, 0, 0), 140.000000, "");  UsedBlocks->Add(b_25);
	auto b_26 = make(1, FVector(21, -31, 13), FVector(8, 7, 1), FRotator(90, 0, 90), 560.000000, ""); b_26.HasElectricityData = true; b_26.ElectricityInfo.CurrentObjectEnergy = 0.000000;  UsedBlocks->Add(b_26);
	auto b_27 = make(1, FVector(14, -31, 13), FVector(8, 7, 1), FRotator(90, 0, 90), 560.000000, ""); b_27.HasElectricityData = true; b_27.ElectricityInfo.CurrentObjectEnergy = 0.000000;  UsedBlocks->Add(b_27);
	auto b_28 = make(1, FVector(24, -27, 13), FVector(8, 7, 1), FRotator(90, 0, -180), 560.000000, ""); b_28.HasElectricityData = true; b_28.ElectricityInfo.CurrentObjectEnergy = 0.000000;  UsedBlocks->Add(b_28);
	auto b_29 = make(1, FVector(24, -20, 13), FVector(8, 7, 1), FRotator(90, 0, -180), 560.000000, ""); b_29.HasElectricityData = true; b_29.ElectricityInfo.CurrentObjectEnergy = 0.000000;  UsedBlocks->Add(b_29);
	auto b_30 = make(2, FVector(16, -27, 10), FVector(2, 7, 1), FRotator(0, 0, 0), 140.000000, "");  UsedBlocks->Add(b_30);
	auto b_31 = make(1, FVector(20, -27, 10), FVector(6, 6, 1), FRotator(0, -90, 0), 360.000000, ""); b_31.HasElectricityData = true; b_31.ElectricityInfo.CurrentObjectEnergy = 0.000000;  UsedBlocks->Add(b_31);
	auto b_32 = make(2, FVector(20, -23, 11), FVector(2, 7, 1), FRotator(0, -90, 0), 140.000000, "");  UsedBlocks->Add(b_32);
	auto b_33 = make(2, FVector(20, -21, 12), FVector(2, 7, 1), FRotator(0, -90, 0), 140.000000, "");  UsedBlocks->Add(b_33);
	auto b_34 = make(2, FVector(20, -19, 13), FVector(2, 7, 1), FRotator(0, -90, 0), 140.000000, "");  UsedBlocks->Add(b_34);
	auto b_35 = make(2, FVector(20, -17, 14), FVector(2, 7, 1), FRotator(0, -90, 0), 140.000000, "");  UsedBlocks->Add(b_35);
	auto b_36 = make(1, FVector(24, -13, 14), FVector(8, 7, 1), FRotator(-90, 0, 0), 560.000000, ""); b_36.HasElectricityData = true; b_36.ElectricityInfo.CurrentObjectEnergy = 0.000000;  UsedBlocks->Add(b_36);
	auto b_37 = make(2, FVector(20, -15, 15), FVector(2, 7, 1), FRotator(0, -90, 0), 140.000000, "");  UsedBlocks->Add(b_37);
	auto b_38 = make(2, FVector(20, -13, 16), FVector(2, 7, 1), FRotator(0, -90, 0), 140.000000, "");  UsedBlocks->Add(b_38);
	auto b_39 = make(2, FVector(20, -11, 17), FVector(2, 7, 1), FRotator(0, -90, 0), 140.000000, "");  UsedBlocks->Add(b_39);
	auto b_40 = make(1, FVector(3, -31, 9), FVector(1, 1, 11), FRotator(0, 0, 0), 110.000000, ""); b_40.HasElectricityData = true; b_40.ElectricityInfo.CurrentObjectEnergy = 0.000000;  UsedBlocks->Add(b_40);
	auto b_41 = make(1, FVector(16, -23, 9), FVector(1, 1, 11), FRotator(0, 0, 0), 110.000000, ""); b_41.HasElectricityData = true; b_41.ElectricityInfo.CurrentObjectEnergy = 0.000000;  UsedBlocks->Add(b_41);
	auto b_42 = make(1100, FVector(11, 18, 0), FVector(20, 20, 2), FRotator(0, 0, 0), 8000.000000, ""); b_42.HasElectricityData = true; b_42.ElectricityInfo.CurrentObjectEnergy = 160000.000000;  UsedBlocks->Add(b_42);
	auto b_43 = make(1100, FVector(-9, 18, 0), FVector(20, 20, 2), FRotator(0, 0, 0), 8000.000000, ""); b_43.HasElectricityData = true; b_43.ElectricityInfo.CurrentObjectEnergy = 160000.000000;  UsedBlocks->Add(b_43);
	auto b_44 = make(1100, FVector(-9, 38, 0), FVector(20, 20, 2), FRotator(0, 0, 0), 8000.000000, ""); b_44.HasElectricityData = true; b_44.ElectricityInfo.CurrentObjectEnergy = 160000.000000;  UsedBlocks->Add(b_44);
	auto b_45 = make(1100, FVector(11, 38, 0), FVector(20, 20, 2), FRotator(0, 0, 0), 8000.000000, ""); b_45.HasElectricityData = true; b_45.ElectricityInfo.CurrentObjectEnergy = 160000.000000;  UsedBlocks->Add(b_45);
	auto b_46 = make(1100, FVector(31, 18, 0), FVector(20, 20, 2), FRotator(0, 0, 0), 8000.000000, ""); b_46.HasElectricityData = true; b_46.ElectricityInfo.CurrentObjectEnergy = 160000.000000;  UsedBlocks->Add(b_46);
	auto b_47 = make(1100, FVector(31, 38, 0), FVector(20, 20, 2), FRotator(0, 0, 0), 8000.000000, ""); b_47.HasElectricityData = true; b_47.ElectricityInfo.CurrentObjectEnergy = 160000.000000;  UsedBlocks->Add(b_47);
	auto b_48 = make(1100, FVector(31, 58, 0), FVector(20, 20, 2), FRotator(0, 0, 0), 8000.000000, ""); b_48.HasElectricityData = true; b_48.ElectricityInfo.CurrentObjectEnergy = 160000.000000;  UsedBlocks->Add(b_48);
	auto b_49 = make(1100, FVector(11, 59, 0), FVector(20, 20, 2), FRotator(0, -90, 0), 8000.000000, ""); b_49.HasElectricityData = true; b_49.ElectricityInfo.CurrentObjectEnergy = 160000.000000;  UsedBlocks->Add(b_49);
	auto b_50 = make(1100, FVector(-9, 59, 0), FVector(20, 20, 2), FRotator(0, -90, 0), 8000.000000, ""); b_50.HasElectricityData = true; b_50.ElectricityInfo.CurrentObjectEnergy = 160000.000000;  UsedBlocks->Add(b_50);
	auto b_51 = make(1050, FVector(-3, -9, 4), FVector(1, 1, 1), FRotator(0, 90, 0), 10.000000, "Tlacitko na zemi"); b_51.BlockSpecificData.Add(TEXT("IsON"), TEXT("0"));
	b_51.HasElectricityData = true; b_51.ElectricityInfo.CurrentObjectEnergy = 0.000000;
	b_51.HasRelationshipData = true; FGuid b_51_rel_id; FGuid::Parse("797CEC8E499C456A5E1C46B8312736AB", b_51_rel_id); b_51.RelationshipInfo.ID = b_51_rel_id; UsedBlocks->Add(b_51);
	auto b_52 = make(600, FVector(24, -7, 11), FVector(5, 1, 4), FRotator(0, 90, 0), 200.000000, ""); b_52.HasElectricityData = true; b_52.ElectricityInfo.CurrentObjectEnergy = 8000.000000;  UsedBlocks->Add(b_52);
	auto b_53 = make(600, FVector(24, -7, 15), FVector(5, 1, 4), FRotator(0, 90, 0), 200.000000, ""); b_53.HasElectricityData = true; b_53.ElectricityInfo.CurrentObjectEnergy = 8000.000000;  UsedBlocks->Add(b_53);
	auto b_54 = make(600, FVector(24, -2, 11), FVector(5, 1, 4), FRotator(0, 90, 0), 200.000000, ""); b_54.HasElectricityData = true; b_54.ElectricityInfo.CurrentObjectEnergy = 8000.000000;  UsedBlocks->Add(b_54);
	auto b_55 = make(600, FVector(24, -2, 15), FVector(5, 1, 4), FRotator(0, 90, 0), 200.000000, ""); b_55.HasElectricityData = true; b_55.ElectricityInfo.CurrentObjectEnergy = 8000.000000;  UsedBlocks->Add(b_55);
	auto b_56 = make(600, FVector(24, 3, 11), FVector(5, 1, 4), FRotator(0, 90, 0), 200.000000, ""); b_56.HasElectricityData = true; b_56.ElectricityInfo.CurrentObjectEnergy = 8000.000000;  UsedBlocks->Add(b_56);
	auto b_57 = make(600, FVector(24, 3, 15), FVector(5, 1, 4), FRotator(0, 90, 0), 200.000000, ""); b_57.HasElectricityData = true; b_57.ElectricityInfo.CurrentObjectEnergy = 8000.000000;  UsedBlocks->Add(b_57);
	auto b_58 = make(1000, FVector(23, -20, 12), FVector(1, 1, 1), FRotator(0, 90, 90), 10.000000, "stena"); b_58.BlockSpecificData.Add(TEXT("IsAutoregulated"), TEXT("1")); b_58.BlockSpecificData.Add(TEXT("IsOn"), TEXT("1"));
	b_58.HasElectricityData = true; b_58.ElectricityInfo.CurrentObjectEnergy = 2000.000000;
	b_58.HasRelationshipData = true; FGuid b_58_rel_id; FGuid::Parse("053D69F542E0BAEAD45D9D9ED75F3844", b_58_rel_id); b_58.RelationshipInfo.ID = b_58_rel_id;
	FRelationshipInfo b_58_rel0; FGuid b_58_rel0_id; FGuid::Parse("5CCE09794A8BA7EDD12133AFE8BA3E40", b_58_rel0_id); b_58_rel0.TargetID = b_58_rel0_id; b_58_rel0.RelationshipType = 0;
	b_58.RelationshipInfo.Relationships.Add(b_58_rel0); UsedBlocks->Add(b_58);
	auto b_59 = make(1150, FVector(-12, 2, 4), FVector(3, 3, 2), FRotator(0, 0, 0), 180.000000, ""); b_59.HasElectricityData = true; b_59.ElectricityInfo.CurrentObjectEnergy = 18000.000000;  UsedBlocks->Add(b_59);
	auto b_60 = make(1150, FVector(-12, -1, 4), FVector(3, 3, 2), FRotator(0, 0, 0), 180.000000, ""); b_60.HasElectricityData = true; b_60.ElectricityInfo.CurrentObjectEnergy = 18000.000000;  UsedBlocks->Add(b_60);
	auto b_61 = make(1150, FVector(-9, 2, 4), FVector(3, 3, 2), FRotator(0, 0, 0), 180.000000, ""); b_61.HasElectricityData = true; b_61.ElectricityInfo.CurrentObjectEnergy = 18000.000000;  UsedBlocks->Add(b_61);
	auto b_62 = make(1150, FVector(-9, -1, 4), FVector(3, 3, 2), FRotator(0, 0, 0), 180.000000, ""); b_62.HasElectricityData = true; b_62.ElectricityInfo.CurrentObjectEnergy = 18000.000000;  UsedBlocks->Add(b_62);
	auto b_63 = make(1200, FVector(22, -29, 5), FVector(3, 3, 3), FRotator(0, -90, 0), 10.000000, ""); b_63.HasElectricityData = true; b_63.ElectricityInfo.CurrentObjectEnergy = 100000.000000;  UsedBlocks->Add(b_63);
	auto b_64 = make(1200, FVector(19, -29, 5), FVector(3, 3, 3), FRotator(0, -90, 0), 10.000000, ""); b_64.HasElectricityData = true; b_64.ElectricityInfo.CurrentObjectEnergy = 100000.000000;  UsedBlocks->Add(b_64);
	auto b_65 = make(1200, FVector(16, -29, 5), FVector(3, 3, 3), FRotator(0, -90, 0), 10.000000, ""); b_65.HasElectricityData = true; b_65.ElectricityInfo.CurrentObjectEnergy = 100000.000000;  UsedBlocks->Add(b_65);
	auto b_66 = make(1200, FVector(13, -29, 5), FVector(3, 3, 3), FRotator(0, -90, 0), 10.000000, ""); b_66.HasElectricityData = true; b_66.ElectricityInfo.CurrentObjectEnergy = 100000.000000;  UsedBlocks->Add(b_66);
	auto b_67 = make(1200, FVector(22, -29, 8), FVector(3, 3, 3), FRotator(0, -90, 0), 10.000000, ""); b_67.HasElectricityData = true; b_67.ElectricityInfo.CurrentObjectEnergy = 100000.000000;  UsedBlocks->Add(b_67);
	auto b_68 = make(1200, FVector(19, -29, 8), FVector(3, 3, 3), FRotator(0, -90, 0), 10.000000, ""); b_68.HasElectricityData = true; b_68.ElectricityInfo.CurrentObjectEnergy = 100000.000000;  UsedBlocks->Add(b_68);
	auto b_69 = make(1200, FVector(22, -23, 5), FVector(3, 3, 3), FRotator(0, -90, 0), 10.000000, ""); b_69.HasElectricityData = true; b_69.ElectricityInfo.CurrentObjectEnergy = 100000.000000;  UsedBlocks->Add(b_69);
	auto b_70 = make(1200, FVector(22, -23, 8), FVector(3, 3, 3), FRotator(0, -90, 0), 10.000000, ""); b_70.HasElectricityData = true; b_70.ElectricityInfo.CurrentObjectEnergy = 100000.000000;  UsedBlocks->Add(b_70);
	auto b_71 = make(1200, FVector(22, -20, 5), FVector(3, 3, 3), FRotator(0, -90, 0), 10.000000, ""); b_71.HasElectricityData = true; b_71.ElectricityInfo.CurrentObjectEnergy = 100000.000000;  UsedBlocks->Add(b_71);
	auto b_72 = make(1200, FVector(22, -20, 8), FVector(3, 3, 3), FRotator(0, -90, 0), 10.000000, ""); b_72.HasElectricityData = true; b_72.ElectricityInfo.CurrentObjectEnergy = 100000.000000;  UsedBlocks->Add(b_72);
	auto b_73 = make(1200, FVector(22, -17, 5), FVector(3, 3, 3), FRotator(0, -90, 0), 10.000000, ""); b_73.HasElectricityData = true; b_73.ElectricityInfo.CurrentObjectEnergy = 100000.000000;  UsedBlocks->Add(b_73);
	auto b_74 = make(1200, FVector(22, -17, 8), FVector(3, 3, 3), FRotator(0, -90, 0), 10.000000, ""); b_74.HasElectricityData = true; b_74.ElectricityInfo.CurrentObjectEnergy = 100000.000000;  UsedBlocks->Add(b_74);
	auto b_75 = make(1200, FVector(22, -14, 5), FVector(3, 3, 3), FRotator(0, 0, 0), 10.000000, ""); b_75.HasElectricityData = true; b_75.ElectricityInfo.CurrentObjectEnergy = 100000.000000;  UsedBlocks->Add(b_75);
	auto b_76 = make(1200, FVector(22, -14, 8), FVector(3, 3, 3), FRotator(0, 0, 0), 10.000000, ""); b_76.HasElectricityData = true; b_76.ElectricityInfo.CurrentObjectEnergy = 100000.000000;  UsedBlocks->Add(b_76);
	auto b_77 = make(1400, FVector(22, -10, 5), FVector(4, 3, 4), FRotator(0, 90, 0), 10.000000, "Plnicka"); b_77.BlockSpecificData.Add(TEXT("CurrentFilling"), TEXT("0.0")); b_77.BlockSpecificData.Add(TEXT("HasItem"), TEXT("0"));
	b_77.HasElectricityData = true; b_77.ElectricityInfo.CurrentObjectEnergy = 150.000000; b_77.HasOxygenData = true; b_77.OxygenInfo.CurrentObjectOxygen = 150.000000;
	b_77.HasRelationshipData = true; FGuid b_77_rel_id; FGuid::Parse("A9F299C54AB0F2B6ABEBC280C23BF218", b_77_rel_id); b_77.RelationshipInfo.ID = b_77_rel_id; UsedBlocks->Add(b_77);
	auto b_78 = make(1050, FVector(23, -11, 11), FVector(1, 1, 1), FRotator(0, 90, 90), 10.000000, "Vypinac plnicka"); b_78.BlockSpecificData.Add(TEXT("IsON"), TEXT("1"));
	b_78.HasElectricityData = true; b_78.ElectricityInfo.CurrentObjectEnergy = 0.000000;
	b_78.HasRelationshipData = true; FGuid b_78_rel_id; FGuid::Parse("6EEA645949C55B8F90F421AB704204C1", b_78_rel_id); b_78.RelationshipInfo.ID = b_78_rel_id; UsedBlocks->Add(b_78);
	auto b_79 = make(1, FVector(-6, -35, 16), FVector(20, 9, 3), FRotator(0, 0, 0), 5400.000000, ""); b_79.HasElectricityData = true; b_79.ElectricityInfo.CurrentObjectEnergy = 0.000000;  UsedBlocks->Add(b_79);
	auto b_80 = make(1, FVector(15, -35, 19), FVector(20, 9, 3), FRotator(0, 180, 0), 5400.000000, ""); b_80.HasElectricityData = true; b_80.ElectricityInfo.CurrentObjectEnergy = 0.000000;  UsedBlocks->Add(b_80);
	auto b_81 = make(101, FVector(-5, -42, 16), FVector(20, 5, 3), FRotator(0, -180, 0), 1000.000122, ""); b_81.HasElectricityData = true; b_81.ElectricityInfo.CurrentObjectEnergy = 0.000000;  UsedBlocks->Add(b_81);
	auto b_82 = make(101, FVector(15, -42, 16), FVector(20, 5, 3), FRotator(0, -180, 0), 1000.000122, ""); b_82.HasElectricityData = true; b_82.ElectricityInfo.CurrentObjectEnergy = 0.000000;  UsedBlocks->Add(b_82);
	auto b_83 = make(101, FVector(27, -29, 16), FVector(20, 5, 3), FRotator(0, -90, 0), 1000.000122, ""); b_83.HasElectricityData = true; b_83.ElectricityInfo.CurrentObjectEnergy = 0.000000;  UsedBlocks->Add(b_83);
	auto b_84 = make(101, FVector(-18, -30, 16), FVector(20, 5, 3), FRotator(0, 90, 0), 1000.000122, ""); b_84.HasElectricityData = true; b_84.ElectricityInfo.CurrentObjectEnergy = 0.000000;  UsedBlocks->Add(b_84);
	auto b_85 = make(101, FVector(-18, -10, 16), FVector(20, 5, 3), FRotator(0, 90, 0), 1000.000122, ""); b_85.HasElectricityData = true; b_85.ElectricityInfo.CurrentObjectEnergy = 0.000000;  UsedBlocks->Add(b_85);
	auto b_86 = make(101, FVector(-18, 10, 16), FVector(20, 5, 3), FRotator(0, 90, 0), 1000.000122, ""); b_86.HasElectricityData = true; b_86.ElectricityInfo.CurrentObjectEnergy = 0.000000;  UsedBlocks->Add(b_86);
	auto b_87 = make(201, FVector(-18, -42, 16), FVector(5, 5, 3), FRotator(0, -180, 0), 375.000000, ""); b_87.HasElectricityData = true; b_87.ElectricityInfo.CurrentObjectEnergy = 0.000000;  UsedBlocks->Add(b_87);
	auto b_88 = make(201, FVector(27, -42, 16), FVector(5, 5, 3), FRotator(0, -90, 0), 375.000000, ""); b_88.HasElectricityData = true; b_88.ElectricityInfo.CurrentObjectEnergy = 0.000000;  UsedBlocks->Add(b_88);
	auto b_89 = make(201, FVector(27, -17, 16), FVector(5, 5, 3), FRotator(0, 0, 0), 375.000000, ""); b_89.HasElectricityData = true; b_89.ElectricityInfo.CurrentObjectEnergy = 0.000000;  UsedBlocks->Add(b_89);
	auto b_90 = make(201, FVector(-18, 23, 16), FVector(5, 5, 3), FRotator(0, 90, 0), 375.000000, ""); b_90.HasElectricityData = true; b_90.ElectricityInfo.CurrentObjectEnergy = 0.000000;  UsedBlocks->Add(b_90);
	auto b_91 = make(101, FVector(-6, 23, 16), FVector(20, 5, 3), FRotator(0, 0, 0), 1000.000122, ""); b_91.HasElectricityData = true; b_91.ElectricityInfo.CurrentObjectEnergy = 0.000000;  UsedBlocks->Add(b_91);
	auto b_92 = make(101, FVector(14, 23, 16), FVector(20, 5, 3), FRotator(0, 0, 0), 1000.000122, ""); b_92.HasElectricityData = true; b_92.ElectricityInfo.CurrentObjectEnergy = 0.000000;  UsedBlocks->Add(b_92);
	auto b_93 = make(201, FVector(27, 23, 16), FVector(5, 5, 3), FRotator(0, 0, 0), 375.000000, ""); b_93.HasElectricityData = true; b_93.ElectricityInfo.CurrentObjectEnergy = 0.000000;  UsedBlocks->Add(b_93);
	auto b_94 = make(1, FVector(4, -31, 12), FVector(5, 1, 1), FRotator(90, 0, 0), 50.000000, ""); b_94.HasElectricityData = true; b_94.ElectricityInfo.CurrentObjectEnergy = 0.000000;  UsedBlocks->Add(b_94);
	auto b_95 = make(600, FVector(7, -31, 13), FVector(8, 1, 6), FRotator(90, 0, -180), 480.000000, ""); b_95.HasElectricityData = true; b_95.ElectricityInfo.CurrentObjectEnergy = 19200.000000;  UsedBlocks->Add(b_95);
	auto b_96 = make(1050, FVector(11, -32, 10), FVector(1, 1, 1), FRotator(0, -180, 90), 10.000000, ""); b_96.BlockSpecificData.Add(TEXT("IsON"), TEXT("1"));
	b_96.HasElectricityData = true; b_96.ElectricityInfo.CurrentObjectEnergy = 0.000000;
	b_96.HasRelationshipData = true; FGuid b_96_rel_id; FGuid::Parse("5CCE09794A8BA7EDD12133AFE8BA3E40", b_96_rel_id); b_96.RelationshipInfo.ID = b_96_rel_id;
	FRelationshipInfo b_96_rel0; FGuid b_96_rel0_id; FGuid::Parse("703EFD1844A5E576A5932288A38E20B9", b_96_rel0_id); b_96_rel0.TargetID = b_96_rel0_id; b_96_rel0.RelationshipType = 1;
	b_96.RelationshipInfo.Relationships.Add(b_96_rel0);
	FRelationshipInfo b_96_rel1; FGuid b_96_rel1_id; FGuid::Parse("2EC04B3D4081FD6E3E1AB98141F4AD19", b_96_rel1_id); b_96_rel1.TargetID = b_96_rel1_id; b_96_rel1.RelationshipType = 1;
	b_96.RelationshipInfo.Relationships.Add(b_96_rel1);
	FRelationshipInfo b_96_rel2; FGuid b_96_rel2_id; FGuid::Parse("4C5484244F03C6E7F8402D8363064A80", b_96_rel2_id); b_96_rel2.TargetID = b_96_rel2_id; b_96_rel2.RelationshipType = 1;
	b_96.RelationshipInfo.Relationships.Add(b_96_rel2);
	FRelationshipInfo b_96_rel3; FGuid b_96_rel3_id; FGuid::Parse("053D69F542E0BAEAD45D9D9ED75F3844", b_96_rel3_id); b_96_rel3.TargetID = b_96_rel3_id; b_96_rel3.RelationshipType = 1;
	b_96.RelationshipInfo.Relationships.Add(b_96_rel3); UsedBlocks->Add(b_96);
	auto b_97 = make(1000, FVector(32, -24, 0), FVector(1, 1, 1), FRotator(0, 90, 0), 10.000000, ""); b_97.BlockSpecificData.Add(TEXT("IsAutoregulated"), TEXT("1")); b_97.BlockSpecificData.Add(TEXT("IsOn"), TEXT("0"));
	b_97.HasElectricityData = true; b_97.ElectricityInfo.CurrentObjectEnergy = 2000.000000;
	b_97.HasRelationshipData = true; FGuid b_97_rel_id; FGuid::Parse("99A46CF04D56857AC88CA19EF91110B7", b_97_rel_id); b_97.RelationshipInfo.ID = b_97_rel_id; UsedBlocks->Add(b_97);
	c->PlayerPosition = FVector(-181, -476, 163);
	c->PlayerRotation = FRotator(0, 29, 0);

	/*c->PlayerPosition = FVector(-214, -227, 163);
	c->PlayerRotation = FRotator(0, -32, 0);
*/
	return c;
}

USaveGameCarrier* UNewGameSaveHolder::getEmptyGameSave(bool full)
{
	auto c = USaveGameCarrier::GetEmptyCarrier();

	c->SaveName = NSLOCTEXT("TCF2LocSpace", "LC.SaveSystemEmpty", "Prázdná hra").ToString();
	c->FullFilePath = TEXT("_system_empty");
	c->CurrentTime = 34920.0f;
	c->IsSystemSave = true;
	c->SaveLoaded = true;
	c->HardcodedLevelName = ENamedHardcodedLevel::DefaultLevel;

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

USaveGameCarrier* UNewGameSaveHolder::getMainMenuSave(bool full)
{
	auto c = USaveGameCarrier::GetEmptyCarrier();

	c->SaveName = TEXT("Main Menu Save");
	c->FullFilePath = TEXT("_system_mainMenu");
	c->IsSystemSave = true;
	c->SaveLoaded = true;
	c->HardcodedLevelName = ENamedHardcodedLevel::MainMenu;

	if (!full)
		return c;

	c->PlayerPosition = FVector(-600, 0, 90);
	auto n = FDateTime::Now().GetTimeOfDay();
	c->CurrentTime = (float)n.GetTotalSeconds();
	c->PlayerUseFPSCamera = true;

#if WITH_EDITORONLY_DATA
#endif
	//c->CurrentTime = 34920.0f;

	auto UsedBlocks = &c->usedBlocks;

	// TODO

	auto b_0 = make(0, FVector(-6, -2, 1), FVector(20, 20, 4), FRotator(0, 0, 0), 0, ""); b_0.HasElectricityData = true; b_0.ElectricityInfo.CurrentObjectEnergy = 0.000000;  UsedBlocks->Add(b_0);
	auto b_1 = make(0, FVector(14, -2, 1), FVector(20, 20, 4), FRotator(0, 0, 0), 0, ""); b_1.HasElectricityData = true; b_1.ElectricityInfo.CurrentObjectEnergy = 0.000000;  UsedBlocks->Add(b_1);
	auto b_2 = make(0, FVector(14, -22, 1), FVector(20, 20, 4), FRotator(0, 0, 0), 0, ""); b_2.HasElectricityData = true; b_2.ElectricityInfo.CurrentObjectEnergy = 0.000000;  UsedBlocks->Add(b_2);
	auto b_3 = make(0, FVector(-6, -22, 1), FVector(20, 20, 4), FRotator(0, 0, 0), 0, ""); b_3.HasElectricityData = true; b_3.ElectricityInfo.CurrentObjectEnergy = 0.000000;  UsedBlocks->Add(b_3);
	auto b_4 = make(500, FVector(0, -17, 6), FVector(1, 1, 1), FRotator(0, 0, 0), 500, ""); b_4.HasElectricityData = true; b_4.ElectricityInfo.CurrentObjectEnergy = 100000.000000;  UsedBlocks->Add(b_4);
	auto b_5 = make(1, FVector(-15, -30, 9), FVector(20, 1, 11), FRotator(0, 90, 0), 1, ""); b_5.HasElectricityData = true; b_5.ElectricityInfo.CurrentObjectEnergy = 0.000000;  UsedBlocks->Add(b_5);
	auto b_6 = make(1, FVector(24, -21, 6), FVector(20, 1, 6), FRotator(0, -90, 0), 1, ""); b_6.HasElectricityData = true; b_6.ElectricityInfo.CurrentObjectEnergy = 0.000000;  UsedBlocks->Add(b_6);
	auto b_7 = make(1, FVector(24, -1, 6), FVector(20, 1, 6), FRotator(0, -90, 0), 1, ""); b_7.HasElectricityData = true; b_7.ElectricityInfo.CurrentObjectEnergy = 0.000000;  UsedBlocks->Add(b_7);
	auto b_8 = make(1, FVector(14, -31, 6), FVector(20, 1, 6), FRotator(0, 180, 0), 1, ""); b_8.HasElectricityData = true; b_8.ElectricityInfo.CurrentObjectEnergy = 0.000000;  UsedBlocks->Add(b_8);
	auto b_9 = make(1, FVector(-15, -1, 6), FVector(20, 1, 6), FRotator(0, -90, 0), 1, ""); b_9.HasElectricityData = true; b_9.ElectricityInfo.CurrentObjectEnergy = 0.000000;  UsedBlocks->Add(b_9);
	auto b_10 = make(600, FVector(-12, -39, 9), FVector(6, 1, 11), FRotator(0, 0, 0), 600, ""); b_10.HasElectricityData = true; b_10.ElectricityInfo.CurrentObjectEnergy = 26400.000000;  UsedBlocks->Add(b_10);
	auto b_11 = make(100, FVector(10, -35, 1), FVector(8, 14, 4), FRotator(0, -90, 0), 100, ""); b_11.HasElectricityData = true; b_11.ElectricityInfo.CurrentObjectEnergy = 0.000000;  UsedBlocks->Add(b_11);
	auto b_12 = make(1000, FVector(25, -13, 8), FVector(1, 1, 1), FRotator(0, 90, -90), 1000, ""); b_12.BlockSpecificData.Add(TEXT("IsAutoregulated"), TEXT("1")); b_12.BlockSpecificData.Add(TEXT("IsOn"), TEXT("0"));
	b_12.HasElectricityData = true; b_12.ElectricityInfo.CurrentObjectEnergy = 200.000000;
	b_12.HasRelationshipData = true; FGuid b_12_rel_id; FGuid::Parse("703EFD1844A5E576A5932288A38E20B9", b_12_rel_id); b_12.RelationshipInfo.ID = b_12_rel_id; UsedBlocks->Add(b_12);
	auto b_13 = make(1000, FVector(13, 9, 3), FVector(1, 1, 1), FRotator(0, -180, -90), 1000, ""); b_13.BlockSpecificData.Add(TEXT("IsAutoregulated"), TEXT("1")); b_13.BlockSpecificData.Add(TEXT("IsOn"), TEXT("0"));
	b_13.HasElectricityData = true; b_13.ElectricityInfo.CurrentObjectEnergy = 200.000000;
	b_13.HasRelationshipData = true; FGuid b_13_rel_id; FGuid::Parse("2EC04B3D4081FD6E3E1AB98141F4AD19", b_13_rel_id); b_13.RelationshipInfo.ID = b_13_rel_id; UsedBlocks->Add(b_13);
	auto b_14 = make(1000, FVector(-16, -10, 2), FVector(1, 1, 1), FRotator(0, -90, -90), 1000, ""); b_14.BlockSpecificData.Add(TEXT("IsAutoregulated"), TEXT("1")); b_14.BlockSpecificData.Add(TEXT("IsOn"), TEXT("0"));
	b_14.HasElectricityData = true; b_14.ElectricityInfo.CurrentObjectEnergy = 200.000000;
	b_14.HasRelationshipData = true; FGuid b_14_rel_id; FGuid::Parse("4C5484244F03C6E7F8402D8363064A80", b_14_rel_id); b_14.RelationshipInfo.ID = b_14_rel_id; UsedBlocks->Add(b_14);
	auto b_15 = make(0, FVector(-6, -35, 1), FVector(19, 8, 4), FRotator(0, 180, 0), 0, ""); b_15.HasElectricityData = true; b_15.ElectricityInfo.CurrentObjectEnergy = 0.000000;  UsedBlocks->Add(b_15);
	auto b_16 = make(700, FVector(0, -35, 9), FVector(7, 7, 11), FRotator(0, 90, 0), 700, ""); b_16.AdditionalFlags.Add(TEXT("DoorOpening"), 0);
	b_16.BlockSpecificData.Add(TEXT("DoorState"), TEXT("2")); b_16.BlockSpecificData.Add(TEXT("DoorYaw"), TEXT("89.0"));
	b_16.HasElectricityData = true; b_16.ElectricityInfo.CurrentObjectEnergy = 0.000000;  UsedBlocks->Add(b_16);
	auto b_17 = make(1, FVector(3, -39, 9), FVector(1, 1, 11), FRotator(0, 0, 0), 1, ""); b_17.HasElectricityData = true; b_17.ElectricityInfo.CurrentObjectEnergy = 0.000000;  UsedBlocks->Add(b_17);
	auto b_18 = make(600, FVector(0, -39, 9), FVector(6, 1, 11), FRotator(0, -180, 0), 600, ""); b_18.HasElectricityData = true; b_18.ElectricityInfo.CurrentObjectEnergy = 26400.000000;  UsedBlocks->Add(b_18);
	auto b_19 = make(600, FVector(-6, -39, 9), FVector(5, 1, 11), FRotator(0, 0, 0), 600, ""); b_19.HasElectricityData = true; b_19.ElectricityInfo.CurrentObjectEnergy = 22000.000000;  UsedBlocks->Add(b_19);
	auto b_20 = make(2, FVector(4, -27, 4), FVector(2, 7, 1), FRotator(0, 0, 0), 2, "");  UsedBlocks->Add(b_20);
	auto b_21 = make(2, FVector(6, -27, 5), FVector(2, 7, 1), FRotator(0, 0, 0), 2, "");  UsedBlocks->Add(b_21);
	auto b_22 = make(2, FVector(8, -27, 6), FVector(2, 7, 1), FRotator(0, 0, 0), 2, "");  UsedBlocks->Add(b_22);
	auto b_23 = make(2, FVector(10, -27, 7), FVector(2, 7, 1), FRotator(0, 0, 0), 2, "");  UsedBlocks->Add(b_23);
	auto b_24 = make(2, FVector(12, -27, 8), FVector(2, 7, 1), FRotator(0, 0, 0), 2, "");  UsedBlocks->Add(b_24);
	auto b_25 = make(2, FVector(14, -27, 9), FVector(2, 7, 1), FRotator(0, 0, 0), 2, "");  UsedBlocks->Add(b_25);
	auto b_26 = make(1, FVector(21, -31, 13), FVector(8, 7, 1), FRotator(90, 0, 90), 1, ""); b_26.HasElectricityData = true; b_26.ElectricityInfo.CurrentObjectEnergy = 0.000000;  UsedBlocks->Add(b_26);
	auto b_27 = make(1, FVector(14, -31, 13), FVector(8, 7, 1), FRotator(90, 0, 90), 1, ""); b_27.HasElectricityData = true; b_27.ElectricityInfo.CurrentObjectEnergy = 0.000000;  UsedBlocks->Add(b_27);
	auto b_28 = make(1, FVector(24, -27, 13), FVector(8, 7, 1), FRotator(90, 0, -180), 1, ""); b_28.HasElectricityData = true; b_28.ElectricityInfo.CurrentObjectEnergy = 0.000000;  UsedBlocks->Add(b_28);
	auto b_29 = make(1, FVector(24, -20, 13), FVector(8, 7, 1), FRotator(90, 0, -180), 1, ""); b_29.HasElectricityData = true; b_29.ElectricityInfo.CurrentObjectEnergy = 0.000000;  UsedBlocks->Add(b_29);
	auto b_30 = make(2, FVector(16, -27, 10), FVector(2, 7, 1), FRotator(0, 0, 0), 2, "");  UsedBlocks->Add(b_30);
	auto b_31 = make(1, FVector(20, -27, 10), FVector(6, 6, 1), FRotator(0, -90, 0), 1, ""); b_31.HasElectricityData = true; b_31.ElectricityInfo.CurrentObjectEnergy = 0.000000;  UsedBlocks->Add(b_31);
	auto b_32 = make(2, FVector(20, -23, 11), FVector(2, 7, 1), FRotator(0, -90, 0), 2, "");  UsedBlocks->Add(b_32);
	auto b_33 = make(2, FVector(20, -21, 12), FVector(2, 7, 1), FRotator(0, -90, 0), 2, "");  UsedBlocks->Add(b_33);
	auto b_34 = make(2, FVector(20, -19, 13), FVector(2, 7, 1), FRotator(0, -90, 0), 2, "");  UsedBlocks->Add(b_34);
	auto b_35 = make(2, FVector(20, -17, 14), FVector(2, 7, 1), FRotator(0, -90, 0), 2, "");  UsedBlocks->Add(b_35);
	auto b_36 = make(1, FVector(24, -13, 14), FVector(8, 7, 1), FRotator(-90, 0, 0), 1, ""); b_36.HasElectricityData = true; b_36.ElectricityInfo.CurrentObjectEnergy = 0.000000;  UsedBlocks->Add(b_36);
	auto b_37 = make(2, FVector(20, -15, 15), FVector(2, 7, 1), FRotator(0, -90, 0), 2, "");  UsedBlocks->Add(b_37);
	auto b_38 = make(2, FVector(20, -13, 16), FVector(2, 7, 1), FRotator(0, -90, 0), 2, "");  UsedBlocks->Add(b_38);
	auto b_39 = make(2, FVector(20, -11, 17), FVector(2, 7, 1), FRotator(0, -90, 0), 2, "");  UsedBlocks->Add(b_39);
	auto b_40 = make(1, FVector(3, -31, 9), FVector(1, 1, 11), FRotator(0, 0, 0), 1, ""); b_40.HasElectricityData = true; b_40.ElectricityInfo.CurrentObjectEnergy = 0.000000;  UsedBlocks->Add(b_40);
	auto b_41 = make(1, FVector(16, -23, 9), FVector(1, 1, 11), FRotator(0, 0, 0), 1, ""); b_41.HasElectricityData = true; b_41.ElectricityInfo.CurrentObjectEnergy = 0.000000;  UsedBlocks->Add(b_41);
	auto b_42 = make(1100, FVector(11, 18, 0), FVector(20, 20, 2), FRotator(0, 0, 0), 1100, ""); b_42.HasElectricityData = true; b_42.ElectricityInfo.CurrentObjectEnergy = 159405.000000;  UsedBlocks->Add(b_42);
	auto b_43 = make(1100, FVector(-9, 18, 0), FVector(20, 20, 2), FRotator(0, 0, 0), 1100, ""); b_43.HasElectricityData = true; b_43.ElectricityInfo.CurrentObjectEnergy = 159709.000000;  UsedBlocks->Add(b_43);
	auto b_44 = make(1100, FVector(-9, 38, 0), FVector(20, 20, 2), FRotator(0, 0, 0), 1100, ""); b_44.HasElectricityData = true; b_44.ElectricityInfo.CurrentObjectEnergy = 159353.000000;  UsedBlocks->Add(b_44);
	auto b_45 = make(1100, FVector(11, 38, 0), FVector(20, 20, 2), FRotator(0, 0, 0), 1100, ""); b_45.HasElectricityData = true; b_45.ElectricityInfo.CurrentObjectEnergy = 159328.000000;  UsedBlocks->Add(b_45);
	auto b_46 = make(1100, FVector(31, 18, 0), FVector(20, 20, 2), FRotator(0, 0, 0), 1100, ""); b_46.HasElectricityData = true; b_46.ElectricityInfo.CurrentObjectEnergy = 159338.000000;  UsedBlocks->Add(b_46);
	auto b_47 = make(1100, FVector(31, 38, 0), FVector(20, 20, 2), FRotator(0, 0, 0), 1100, ""); b_47.HasElectricityData = true; b_47.ElectricityInfo.CurrentObjectEnergy = 159292.000000;  UsedBlocks->Add(b_47);
	auto b_48 = make(1100, FVector(31, 58, 0), FVector(20, 20, 2), FRotator(0, 0, 0), 1100, ""); b_48.HasElectricityData = true; b_48.ElectricityInfo.CurrentObjectEnergy = 159627.000000;  UsedBlocks->Add(b_48);
	auto b_49 = make(1100, FVector(11, 59, 0), FVector(20, 20, 2), FRotator(0, -90, 0), 1100, ""); b_49.HasElectricityData = true; b_49.ElectricityInfo.CurrentObjectEnergy = 159665.000000;  UsedBlocks->Add(b_49);
	auto b_50 = make(1100, FVector(-9, 59, 0), FVector(20, 20, 2), FRotator(0, -90, 0), 1100, ""); b_50.HasElectricityData = true; b_50.ElectricityInfo.CurrentObjectEnergy = 159719.000000;  UsedBlocks->Add(b_50);
	auto b_51 = make(1050, FVector(-3, -9, 4), FVector(1, 1, 1), FRotator(0, 90, 0), 1050, "Tlacitko na zemi"); b_51.BlockSpecificData.Add(TEXT("IsON"), TEXT("0"));
	b_51.HasElectricityData = true; b_51.ElectricityInfo.CurrentObjectEnergy = 0.000000;
	b_51.HasRelationshipData = true; FGuid b_51_rel_id; FGuid::Parse("797CEC8E499C456A5E1C46B8312736AB", b_51_rel_id); b_51.RelationshipInfo.ID = b_51_rel_id; UsedBlocks->Add(b_51);
	auto b_52 = make(600, FVector(24, -7, 11), FVector(5, 1, 4), FRotator(0, 90, 0), 600, ""); b_52.HasElectricityData = true; b_52.ElectricityInfo.CurrentObjectEnergy = 8000.000000;  UsedBlocks->Add(b_52);
	auto b_53 = make(600, FVector(24, -7, 15), FVector(5, 1, 4), FRotator(0, 90, 0), 600, ""); b_53.HasElectricityData = true; b_53.ElectricityInfo.CurrentObjectEnergy = 8000.000000;  UsedBlocks->Add(b_53);
	auto b_54 = make(600, FVector(24, -2, 11), FVector(5, 1, 4), FRotator(0, 90, 0), 600, ""); b_54.HasElectricityData = true; b_54.ElectricityInfo.CurrentObjectEnergy = 8000.000000;  UsedBlocks->Add(b_54);
	auto b_55 = make(600, FVector(24, -2, 15), FVector(5, 1, 4), FRotator(0, 90, 0), 600, ""); b_55.HasElectricityData = true; b_55.ElectricityInfo.CurrentObjectEnergy = 8000.000000;  UsedBlocks->Add(b_55);
	auto b_56 = make(600, FVector(24, 3, 11), FVector(5, 1, 4), FRotator(0, 90, 0), 600, ""); b_56.HasElectricityData = true; b_56.ElectricityInfo.CurrentObjectEnergy = 8000.000000;  UsedBlocks->Add(b_56);
	auto b_57 = make(600, FVector(24, 3, 15), FVector(5, 1, 4), FRotator(0, 90, 0), 600, ""); b_57.HasElectricityData = true; b_57.ElectricityInfo.CurrentObjectEnergy = 8000.000000;  UsedBlocks->Add(b_57);
	auto b_58 = make(1000, FVector(23, -20, 12), FVector(1, 1, 1), FRotator(0, 90, 90), 1000, "stena"); b_58.BlockSpecificData.Add(TEXT("IsAutoregulated"), TEXT("1")); b_58.BlockSpecificData.Add(TEXT("IsOn"), TEXT("0"));
	b_58.HasElectricityData = true; b_58.ElectricityInfo.CurrentObjectEnergy = 200.000000;
	b_58.HasRelationshipData = true; FGuid b_58_rel_id; FGuid::Parse("053D69F542E0BAEAD45D9D9ED75F3844", b_58_rel_id); b_58.RelationshipInfo.ID = b_58_rel_id; UsedBlocks->Add(b_58);
	auto b_59 = make(1150, FVector(-12, 2, 4), FVector(3, 3, 2), FRotator(0, 0, 0), 1150, ""); b_59.HasElectricityData = true; b_59.ElectricityInfo.CurrentObjectEnergy = 18000.000000;  UsedBlocks->Add(b_59);
	auto b_60 = make(1150, FVector(-12, -1, 4), FVector(3, 3, 2), FRotator(0, 0, 0), 1150, ""); b_60.HasElectricityData = true; b_60.ElectricityInfo.CurrentObjectEnergy = 18000.000000;  UsedBlocks->Add(b_60);
	auto b_61 = make(1150, FVector(-9, 2, 4), FVector(3, 3, 2), FRotator(0, 0, 0), 1150, ""); b_61.HasElectricityData = true; b_61.ElectricityInfo.CurrentObjectEnergy = 18000.000000;  UsedBlocks->Add(b_61);
	auto b_62 = make(1150, FVector(-9, -1, 4), FVector(3, 3, 2), FRotator(0, 0, 0), 1150, ""); b_62.HasElectricityData = true; b_62.ElectricityInfo.CurrentObjectEnergy = 18000.000000;  UsedBlocks->Add(b_62);
	auto b_63 = make(1200, FVector(22, -29, 5), FVector(3, 3, 3), FRotator(0, -90, 0), 1200, ""); b_63.HasElectricityData = true; b_63.ElectricityInfo.CurrentObjectEnergy = 100000.000000;  UsedBlocks->Add(b_63);
	auto b_64 = make(1200, FVector(19, -29, 5), FVector(3, 3, 3), FRotator(0, -90, 0), 1200, ""); b_64.HasElectricityData = true; b_64.ElectricityInfo.CurrentObjectEnergy = 100000.000000;  UsedBlocks->Add(b_64);
	auto b_65 = make(1200, FVector(16, -29, 5), FVector(3, 3, 3), FRotator(0, -90, 0), 1200, ""); b_65.HasElectricityData = true; b_65.ElectricityInfo.CurrentObjectEnergy = 100000.000000;  UsedBlocks->Add(b_65);
	auto b_66 = make(1200, FVector(13, -29, 5), FVector(3, 3, 3), FRotator(0, -90, 0), 1200, ""); b_66.HasElectricityData = true; b_66.ElectricityInfo.CurrentObjectEnergy = 100000.000000;  UsedBlocks->Add(b_66);
	auto b_67 = make(1200, FVector(22, -29, 8), FVector(3, 3, 3), FRotator(0, -90, 0), 1200, ""); b_67.HasElectricityData = true; b_67.ElectricityInfo.CurrentObjectEnergy = 100000.000000;  UsedBlocks->Add(b_67);
	auto b_68 = make(1200, FVector(19, -29, 8), FVector(3, 3, 3), FRotator(0, -90, 0), 1200, ""); b_68.HasElectricityData = true; b_68.ElectricityInfo.CurrentObjectEnergy = 100000.000000;  UsedBlocks->Add(b_68);
	auto b_69 = make(1200, FVector(22, -23, 5), FVector(3, 3, 3), FRotator(0, -90, 0), 1200, ""); b_69.HasElectricityData = true; b_69.ElectricityInfo.CurrentObjectEnergy = 100000.000000;  UsedBlocks->Add(b_69);
	auto b_70 = make(1200, FVector(22, -23, 8), FVector(3, 3, 3), FRotator(0, -90, 0), 1200, ""); b_70.HasElectricityData = true; b_70.ElectricityInfo.CurrentObjectEnergy = 100000.000000;  UsedBlocks->Add(b_70);
	auto b_71 = make(1200, FVector(22, -20, 5), FVector(3, 3, 3), FRotator(0, -90, 0), 1200, ""); b_71.HasElectricityData = true; b_71.ElectricityInfo.CurrentObjectEnergy = 100000.000000;  UsedBlocks->Add(b_71);
	auto b_72 = make(1200, FVector(22, -20, 8), FVector(3, 3, 3), FRotator(0, -90, 0), 1200, ""); b_72.HasElectricityData = true; b_72.ElectricityInfo.CurrentObjectEnergy = 100000.000000;  UsedBlocks->Add(b_72);
	auto b_73 = make(1200, FVector(22, -17, 5), FVector(3, 3, 3), FRotator(0, -90, 0), 1200, ""); b_73.HasElectricityData = true; b_73.ElectricityInfo.CurrentObjectEnergy = 100000.000000;  UsedBlocks->Add(b_73);
	auto b_74 = make(1200, FVector(22, -17, 8), FVector(3, 3, 3), FRotator(0, -90, 0), 1200, ""); b_74.HasElectricityData = true; b_74.ElectricityInfo.CurrentObjectEnergy = 100000.000000;  UsedBlocks->Add(b_74);
	auto b_75 = make(1200, FVector(22, -14, 5), FVector(3, 3, 3), FRotator(0, 0, 0), 1200, ""); b_75.HasElectricityData = true; b_75.ElectricityInfo.CurrentObjectEnergy = 100000.000000;  UsedBlocks->Add(b_75);
	auto b_76 = make(1200, FVector(22, -14, 8), FVector(3, 3, 3), FRotator(0, 0, 0), 1200, ""); b_76.HasElectricityData = true; b_76.ElectricityInfo.CurrentObjectEnergy = 100000.000000;  UsedBlocks->Add(b_76);
	auto b_77 = make(1400, FVector(22, -10, 5), FVector(4, 3, 4), FRotator(0, 90, 0), 1400, "Plnicka"); b_77.BlockSpecificData.Add(TEXT("CurrentFilling"), TEXT("0.0")); b_77.BlockSpecificData.Add(TEXT("HasItem"), TEXT("0"));
	b_77.HasElectricityData = true; b_77.ElectricityInfo.CurrentObjectEnergy = 150.000000; b_77.HasOxygenData = true; b_77.OxygenInfo.CurrentObjectOxygen = 150.000000;
	b_77.HasRelationshipData = true; FGuid b_77_rel_id; FGuid::Parse("A9F299C54AB0F2B6ABEBC280C23BF218", b_77_rel_id); b_77.RelationshipInfo.ID = b_77_rel_id; UsedBlocks->Add(b_77);
	auto b_78 = make(1050, FVector(23, -11, 11), FVector(1, 1, 1), FRotator(0, 90, 90), 1050, "Vypinac plnicka"); b_78.BlockSpecificData.Add(TEXT("IsON"), TEXT("1"));
	b_78.HasElectricityData = true; b_78.ElectricityInfo.CurrentObjectEnergy = 0.000000;
	b_78.HasRelationshipData = true; FGuid b_78_rel_id; FGuid::Parse("6EEA645949C55B8F90F421AB704204C1", b_78_rel_id); b_78.RelationshipInfo.ID = b_78_rel_id; UsedBlocks->Add(b_78);
	auto b_79 = make(1, FVector(-6, -35, 16), FVector(20, 9, 3), FRotator(0, 0, 0), 1, ""); b_79.HasElectricityData = true; b_79.ElectricityInfo.CurrentObjectEnergy = 0.000000;  UsedBlocks->Add(b_79);
	auto b_80 = make(1, FVector(15, -35, 19), FVector(20, 9, 3), FRotator(0, 180, 0), 1, ""); b_80.HasElectricityData = true; b_80.ElectricityInfo.CurrentObjectEnergy = 0.000000;  UsedBlocks->Add(b_80);
	auto b_81 = make(101, FVector(-5, -42, 16), FVector(20, 5, 3), FRotator(0, -180, 0), 101, ""); b_81.HasElectricityData = true; b_81.ElectricityInfo.CurrentObjectEnergy = 0.000000;  UsedBlocks->Add(b_81);
	auto b_82 = make(101, FVector(15, -42, 16), FVector(20, 5, 3), FRotator(0, -180, 0), 101, ""); b_82.HasElectricityData = true; b_82.ElectricityInfo.CurrentObjectEnergy = 0.000000;  UsedBlocks->Add(b_82);
	auto b_83 = make(101, FVector(27, -29, 16), FVector(20, 5, 3), FRotator(0, -90, 0), 101, ""); b_83.HasElectricityData = true; b_83.ElectricityInfo.CurrentObjectEnergy = 0.000000;  UsedBlocks->Add(b_83);
	auto b_84 = make(101, FVector(-18, -30, 16), FVector(20, 5, 3), FRotator(0, 90, 0), 101, ""); b_84.HasElectricityData = true; b_84.ElectricityInfo.CurrentObjectEnergy = 0.000000;  UsedBlocks->Add(b_84);
	auto b_85 = make(101, FVector(-18, -10, 16), FVector(20, 5, 3), FRotator(0, 90, 0), 101, ""); b_85.HasElectricityData = true; b_85.ElectricityInfo.CurrentObjectEnergy = 0.000000;  UsedBlocks->Add(b_85);
	auto b_86 = make(101, FVector(-18, 10, 16), FVector(20, 5, 3), FRotator(0, 90, 0), 101, ""); b_86.HasElectricityData = true; b_86.ElectricityInfo.CurrentObjectEnergy = 0.000000;  UsedBlocks->Add(b_86);
	auto b_87 = make(201, FVector(-18, -42, 16), FVector(5, 5, 3), FRotator(0, -180, 0), 201, ""); b_87.HasElectricityData = true; b_87.ElectricityInfo.CurrentObjectEnergy = 0.000000;  UsedBlocks->Add(b_87);
	auto b_88 = make(201, FVector(27, -42, 16), FVector(5, 5, 3), FRotator(0, -90, 0), 201, ""); b_88.HasElectricityData = true; b_88.ElectricityInfo.CurrentObjectEnergy = 0.000000;  UsedBlocks->Add(b_88);
	auto b_89 = make(201, FVector(27, -17, 16), FVector(5, 5, 3), FRotator(0, 0, 0), 201, ""); b_89.HasElectricityData = true; b_89.ElectricityInfo.CurrentObjectEnergy = 0.000000;  UsedBlocks->Add(b_89);
	auto b_90 = make(201, FVector(-18, 23, 16), FVector(5, 5, 3), FRotator(0, 90, 0), 201, ""); b_90.HasElectricityData = true; b_90.ElectricityInfo.CurrentObjectEnergy = 0.000000;  UsedBlocks->Add(b_90);
	auto b_91 = make(101, FVector(-6, 23, 16), FVector(20, 5, 3), FRotator(0, 0, 0), 101, ""); b_91.HasElectricityData = true; b_91.ElectricityInfo.CurrentObjectEnergy = 0.000000;  UsedBlocks->Add(b_91);
	auto b_92 = make(101, FVector(14, 23, 16), FVector(20, 5, 3), FRotator(0, 0, 0), 101, ""); b_92.HasElectricityData = true; b_92.ElectricityInfo.CurrentObjectEnergy = 0.000000;  UsedBlocks->Add(b_92);
	auto b_93 = make(201, FVector(27, 23, 16), FVector(5, 5, 3), FRotator(0, 0, 0), 201, ""); b_93.HasElectricityData = true; b_93.ElectricityInfo.CurrentObjectEnergy = 0.000000;  UsedBlocks->Add(b_93);
	auto b_94 = make(1, FVector(4, -31, 12), FVector(5, 1, 1), FRotator(90, 0, 0), 1, ""); b_94.HasElectricityData = true; b_94.ElectricityInfo.CurrentObjectEnergy = 0.000000;  UsedBlocks->Add(b_94);
	auto b_95 = make(600, FVector(7, -31, 13), FVector(8, 1, 6), FRotator(90, 0, -180), 600, ""); b_95.HasElectricityData = true; b_95.ElectricityInfo.CurrentObjectEnergy = 19200.000000;  UsedBlocks->Add(b_95);
	auto b_96 = make(1050, FVector(11, -32, 10), FVector(1, 1, 1), FRotator(0, -180, 90), 1050, ""); b_96.BlockSpecificData.Add(TEXT("IsON"), TEXT("0"));
	b_96.HasElectricityData = true; b_96.ElectricityInfo.CurrentObjectEnergy = 0.000000;
	b_96.HasRelationshipData = true; FGuid b_96_rel_id; FGuid::Parse("5CCE09794A8BA7EDD12133AFE8BA3E40", b_96_rel_id); b_96.RelationshipInfo.ID = b_96_rel_id; UsedBlocks->Add(b_96);
	auto b_97 = make(1000, FVector(8, -35, 17), FVector(1, 1, 1), FRotator(0, 90, -180), 1000, "venku"); b_97.BlockSpecificData.Add(TEXT("IsON"), TEXT("0")); b_97.BlockSpecificData.Add(TEXT("IsAutoregulated"), TEXT("1"));
	b_97.HasElectricityData = true; b_97.ElectricityInfo.CurrentObjectEnergy = 200.000000;
	b_97.HasRelationshipData = true; FGuid b_97_rel_id; FGuid::Parse("5CCE09794A8BA7EDD12133AFE8BA3E40", b_97_rel_id); b_97.RelationshipInfo.ID = b_97_rel_id; UsedBlocks->Add(b_97);
	auto b_98 = make(1000, FVector(15, -35, 17), FVector(1, 1, 1), FRotator(0, 90, -180), 1000, "venku"); b_98.BlockSpecificData.Add(TEXT("IsON"), TEXT("0")); b_98.BlockSpecificData.Add(TEXT("IsAutoregulated"), TEXT("1"));
	b_98.HasElectricityData = true; b_98.ElectricityInfo.CurrentObjectEnergy = 200.000000;
	b_98.HasRelationshipData = true; FGuid b_98_rel_id; FGuid::Parse("5CCE09794A8BA7EDD12133AFE8BA3E40", b_98_rel_id); b_98.RelationshipInfo.ID = b_98_rel_id; UsedBlocks->Add(b_98);
	auto b_99 = make(1000, FVector(23, -35, 17), FVector(1, 1, 1), FRotator(0, 90, -180), 1000, "venku"); b_99.BlockSpecificData.Add(TEXT("IsON"), TEXT("0")); b_99.BlockSpecificData.Add(TEXT("IsAutoregulated"), TEXT("1"));
	b_99.HasElectricityData = true; b_99.ElectricityInfo.CurrentObjectEnergy = 200.000000;
	b_99.HasRelationshipData = true; FGuid b_99_rel_id; FGuid::Parse("5CCE09794A8BA7EDD12133AFE8BA3E40", b_99_rel_id); b_99.RelationshipInfo.ID = b_99_rel_id; UsedBlocks->Add(b_99);
	auto b_100 = make(1000, FVector(26, -41, 14), FVector(1, 1, 1), FRotator(0, 90, -180), 1000, "venku"); b_100.BlockSpecificData.Add(TEXT("IsON"), TEXT("0")); b_100.BlockSpecificData.Add(TEXT("IsAutoregulated"), TEXT("1"));
	b_100.HasElectricityData = true; b_100.ElectricityInfo.CurrentObjectEnergy = 200.000000;
	b_100.HasRelationshipData = true; FGuid b_100_rel_id; FGuid::Parse("5CCE09794A8BA7EDD12133AFE8BA3E40", b_100_rel_id); b_100.RelationshipInfo.ID = b_100_rel_id; UsedBlocks->Add(b_100);
	auto b_101 = make(1000, FVector(5, -42, 14), FVector(1, 1, 1), FRotator(0, 90, -180), 1000, "venku"); b_101.BlockSpecificData.Add(TEXT("IsON"), TEXT("0")); b_101.BlockSpecificData.Add(TEXT("IsAutoregulated"), TEXT("1"));
	b_101.HasElectricityData = true; b_101.ElectricityInfo.CurrentObjectEnergy = 200.000000;
	b_101.HasRelationshipData = true; FGuid b_101_rel_id; FGuid::Parse("5CCE09794A8BA7EDD12133AFE8BA3E40", b_101_rel_id); b_101.RelationshipInfo.ID = b_101_rel_id; UsedBlocks->Add(b_101);
	auto b_102 = make(1000, FVector(-17, -41, 14), FVector(1, 1, 1), FRotator(0, 90, -180), 1000, "venku"); b_102.BlockSpecificData.Add(TEXT("IsON"), TEXT("0")); b_102.BlockSpecificData.Add(TEXT("IsAutoregulated"), TEXT("1"));
	b_102.HasElectricityData = true; b_102.ElectricityInfo.CurrentObjectEnergy = 200.000000;
	b_102.HasRelationshipData = true; FGuid b_102_rel_id; FGuid::Parse("5CCE09794A8BA7EDD12133AFE8BA3E40", b_102_rel_id); b_102.RelationshipInfo.ID = b_102_rel_id; UsedBlocks->Add(b_102);
	auto b_103 = make(1000, FVector(-18, -19, 14), FVector(1, 1, 1), FRotator(0, 90, -180), 1000, "venku"); b_103.BlockSpecificData.Add(TEXT("IsON"), TEXT("0")); b_103.BlockSpecificData.Add(TEXT("IsAutoregulated"), TEXT("1"));
	b_103.HasElectricityData = true; b_103.ElectricityInfo.CurrentObjectEnergy = 200.000000;
	b_103.HasRelationshipData = true; FGuid b_103_rel_id; FGuid::Parse("5CCE09794A8BA7EDD12133AFE8BA3E40", b_103_rel_id); b_103.RelationshipInfo.ID = b_103_rel_id; UsedBlocks->Add(b_103);
	auto b_104 = make(1000, FVector(-18, 1, 14), FVector(1, 1, 1), FRotator(0, 90, -180), 1000, "venku"); b_104.BlockSpecificData.Add(TEXT("IsON"), TEXT("0")); b_104.BlockSpecificData.Add(TEXT("IsAutoregulated"), TEXT("1"));
	b_104.HasElectricityData = true; b_104.ElectricityInfo.CurrentObjectEnergy = 200.000000;
	b_104.HasRelationshipData = true; FGuid b_104_rel_id; FGuid::Parse("5CCE09794A8BA7EDD12133AFE8BA3E40", b_104_rel_id); b_104.RelationshipInfo.ID = b_104_rel_id; UsedBlocks->Add(b_104);
	auto b_105 = make(1000, FVector(-17, 22, 14), FVector(1, 1, 1), FRotator(0, 90, -180), 1000, "venku"); b_105.BlockSpecificData.Add(TEXT("IsON"), TEXT("0")); b_105.BlockSpecificData.Add(TEXT("IsAutoregulated"), TEXT("1"));
	b_105.HasElectricityData = true; b_105.ElectricityInfo.CurrentObjectEnergy = 200.000000;
	b_105.HasRelationshipData = true; FGuid b_105_rel_id; FGuid::Parse("5CCE09794A8BA7EDD12133AFE8BA3E40", b_105_rel_id); b_105.RelationshipInfo.ID = b_105_rel_id; UsedBlocks->Add(b_105);
	auto b_106 = make(1000, FVector(5, 23, 14), FVector(1, 1, 1), FRotator(0, 90, -180), 1000, "venku"); b_106.BlockSpecificData.Add(TEXT("IsON"), TEXT("0")); b_106.BlockSpecificData.Add(TEXT("IsAutoregulated"), TEXT("1"));
	b_106.HasElectricityData = true; b_106.ElectricityInfo.CurrentObjectEnergy = 200.000000;
	b_106.HasRelationshipData = true; FGuid b_106_rel_id; FGuid::Parse("5CCE09794A8BA7EDD12133AFE8BA3E40", b_106_rel_id); b_106.RelationshipInfo.ID = b_106_rel_id; UsedBlocks->Add(b_106);
	auto b_107 = make(1000, FVector(26, 22, 14), FVector(1, 1, 1), FRotator(0, 90, -180), 1000, "venku"); b_107.BlockSpecificData.Add(TEXT("IsON"), TEXT("0")); b_107.BlockSpecificData.Add(TEXT("IsAutoregulated"), TEXT("1"));
	b_107.HasElectricityData = true; b_107.ElectricityInfo.CurrentObjectEnergy = 200.000000;
	b_107.HasRelationshipData = true; FGuid b_107_rel_id; FGuid::Parse("5CCE09794A8BA7EDD12133AFE8BA3E40", b_107_rel_id); b_107.RelationshipInfo.ID = b_107_rel_id; UsedBlocks->Add(b_107);
	auto b_108 = make(1000, FVector(26, -18, 14), FVector(1, 1, 1), FRotator(0, -90, -180), 1000, "venku"); b_108.BlockSpecificData.Add(TEXT("IsON"), TEXT("0")); b_108.BlockSpecificData.Add(TEXT("IsAutoregulated"), TEXT("1"));
	b_108.HasElectricityData = true; b_108.ElectricityInfo.CurrentObjectEnergy = 200.000000;
	b_108.HasRelationshipData = true; FGuid b_108_rel_id; FGuid::Parse("5CCE09794A8BA7EDD12133AFE8BA3E40", b_108_rel_id); b_108.RelationshipInfo.ID = b_108_rel_id; UsedBlocks->Add(b_108);
	auto b_109 = make(1000, FVector(32, -24, 0), FVector(1, 1, 1), FRotator(0, 90, 0), 1000, ""); b_109.BlockSpecificData.Add(TEXT("IsAutoregulated"), TEXT("1")); b_109.BlockSpecificData.Add(TEXT("IsOn"), TEXT("0"));
	b_109.HasElectricityData = true; b_109.ElectricityInfo.CurrentObjectEnergy = 0.000000;
	b_109.HasRelationshipData = true; FGuid b_109_rel_id; FGuid::Parse("99A46CF04D56857AC88CA19EF91110B7", b_109_rel_id); b_109.RelationshipInfo.ID = b_109_rel_id; UsedBlocks->Add(b_109);
	c->PlayerPosition = FVector(983, -712, 82);
	c->PlayerRotation = FRotator(0, 146, 0);

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

USaveGameCarrier* UNewGameSaveHolder::getMiniLevelGameSave(bool full)
{
	auto c = USaveGameCarrier::GetEmptyCarrier();

	c->SaveName = NSLOCTEXT("TCF2LocSpace", "LC.SaveSystemMiniLevel", "Těžké začátky").ToString();
	c->FullFilePath = TEXT("_system_miniLevel");
	c->IsSystemSave = true;
	c->SaveLoaded = true;
	c->HardcodedLevelName = ENamedHardcodedLevel::MiniLevel;

	if (!full)
		return c;

	//c->CurrentTime = 14400.0f; 
	c->CurrentTime = 34920.0f;
	c->PlayerUseFPSCamera = true;
	c->IsCreativeMode = true;
	c->PlayerHealth = 1000;

	c->PlayerOxygenComponent.CurrentObjectOxygen = 1000.0f;
	c->PlayerElectricityComponent.CurrentObjectEnergy = 150000.0f;

	c->inventoryTags = makeDefault();

	c->buildableBlocks.Add(makeBuildable(DeleteID, FVector(1, 1, 1)));
	c->buildableBlocks.Add(makeBuildable(TerminalID, FVector(1, 8, 5)));

	c->buildableBlocks.Add(makeBuildable(OxygenTankID, FVector(2, 2, 2)));
	c->buildableBlocks.Add(makeBuildable(SwitcherID, FVector(1, 1, 1)));
	c->buildableBlocks.Add(makeBuildable(LightSmallID, FVector(1, 1, 1)));
	c->buildableBlocks.Add(makeBuildable(CreatorID, FVector(3, 3, 2)));

	auto UsedBlocks = &c->usedBlocks;

	auto b_0 = make(0, FVector(-146, 12, 1), FVector(15, 15, 4), FRotator(0, -180, 0), 9000.000000, ""); b_0.HasElectricityData = true; b_0.ElectricityInfo.CurrentObjectEnergy = 0.000000;  UsedBlocks->Add(b_0);
	auto b_1 = make(700, FVector(-142, 9, 9), FVector(7, 7, 11), FRotator(0, 90, 0), 10.000000, ""); b_1.AdditionalFlags.Add(TEXT("DoorOpening"), 0);
	b_1.BlockSpecificData.Add(TEXT("DoorState"), TEXT("2")); b_1.BlockSpecificData.Add(TEXT("DoorYaw"), TEXT("89.999992"));
	b_1.HasElectricityData = true; b_1.ElectricityInfo.CurrentObjectEnergy = 0.000000;  UsedBlocks->Add(b_1);
	auto b_2 = make(1150, FVector(-150, 8, 4), FVector(3, 3, 2), FRotator(0, -90, 0), 180.000000, ""); b_2.HasElectricityData = true; b_2.ElectricityInfo.CurrentObjectEnergy = 18000.000000;  UsedBlocks->Add(b_2);
	auto b_3 = make(2, FVector(-150, 10, 6), FVector(5, 4, 1), FRotator(-90, 0, -90), 200.000000, "");  UsedBlocks->Add(b_3);
	auto b_4 = make(2, FVector(-148, 7, 6), FVector(5, 4, 1), FRotator(-90, 0, 0), 200.000000, "");  UsedBlocks->Add(b_4);
	auto b_5 = make(2, FVector(-150, 6, 6), FVector(5, 4, 1), FRotator(-90, 0, 90), 200.000000, "");  UsedBlocks->Add(b_5);
	auto b_6 = make(2, FVector(-152, 9, 6), FVector(5, 4, 1), FRotator(-90, 0, -180), 200.000000, "");  UsedBlocks->Add(b_6);
	auto b_7 = make(2, FVector(-150, 8, 9), FVector(5, 5, 1), FRotator(0, -90, 0), 250.000000, "");  UsedBlocks->Add(b_7);
	auto b_8 = make(500, FVector(-152, 10, 12), FVector(1, 1, 1), FRotator(0, -180, 0), 10.000000, ""); b_8.HasElectricityData = true; b_8.ElectricityInfo.CurrentObjectEnergy = 100000.000000;  UsedBlocks->Add(b_8);
	auto b_9 = make(1, FVector(-153, 12, 9), FVector(14, 1, 11), FRotator(0, 90, 0), 1540.000000, ""); b_9.HasElectricityData = true; b_9.ElectricityInfo.CurrentObjectEnergy = 0.000000;  UsedBlocks->Add(b_9);
	auto b_10 = make(1, FVector(-139, 13, 9), FVector(1, 1, 11), FRotator(0, 90, 0), 110.000000, ""); b_10.HasElectricityData = true; b_10.ElectricityInfo.CurrentObjectEnergy = 0.000000;  UsedBlocks->Add(b_10);
	auto b_11 = make(1, FVector(-139, 19, 9), FVector(1, 1, 11), FRotator(0, 90, 0), 110.000000, ""); b_11.HasElectricityData = true; b_11.ElectricityInfo.CurrentObjectEnergy = 0.000000;  UsedBlocks->Add(b_11);
	auto b_12 = make(1, FVector(-139, 5, 9), FVector(1, 1, 11), FRotator(0, 90, 0), 110.000000, ""); b_12.HasElectricityData = true; b_12.ElectricityInfo.CurrentObjectEnergy = 0.000000;  UsedBlocks->Add(b_12);
	auto b_13 = make(1, FVector(-150, 5, 9), FVector(6, 1, 11), FRotator(0, -180, 0), 660.000000, ""); b_13.HasElectricityData = true; b_13.ElectricityInfo.CurrentObjectEnergy = 0.000000;  UsedBlocks->Add(b_13);
	auto b_14 = make(600, FVector(-146, 19, 9), FVector(13, 1, 11), FRotator(0, 0, 0), 1430.000000, ""); b_14.HasElectricityData = true; b_14.ElectricityInfo.CurrentObjectEnergy = 57200.000000;  UsedBlocks->Add(b_14);
	auto b_15 = make(600, FVector(-139, 16, 9), FVector(5, 1, 11), FRotator(0, -90, 0), 550.000000, ""); b_15.HasElectricityData = true; b_15.ElectricityInfo.CurrentObjectEnergy = 22000.000000;  UsedBlocks->Add(b_15);
	auto b_16 = make(600, FVector(-143, 5, 9), FVector(8, 1, 11), FRotator(0, -180, 0), 880.000000, ""); b_16.HasElectricityData = true; b_16.ElectricityInfo.CurrentObjectEnergy = 35200.000000;  UsedBlocks->Add(b_16);
	auto b_17 = make(1100, FVector(-146, 12, 15), FVector(15, 15, 2), FRotator(0, 180, 0), 4500.000000, ""); b_17.HasElectricityData = true; b_17.ElectricityInfo.CurrentObjectEnergy = 90000.000000;  UsedBlocks->Add(b_17);
	auto b_18 = make(1050, FVector(-140, 13, 11), FVector(1, 1, 1), FRotator(90, 0, 0), 10.000000, ""); b_18.BlockSpecificData.Add(TEXT("IsON"), TEXT("1"));
	b_18.HasElectricityData = true; b_18.ElectricityInfo.CurrentObjectEnergy = 0.000000;
	b_18.HasRelationshipData = true; FGuid b_18_rel_id; FGuid::Parse("E0DE5F0C4742F701EBA06DB8EA53DD81", b_18_rel_id); b_18.RelationshipInfo.ID = b_18_rel_id;
	FRelationshipInfo b_18_rel0; FGuid b_18_rel0_id; FGuid::Parse("4A29A8B643CA566204D5FEB7C7F5ABB1", b_18_rel0_id); b_18_rel0.TargetID = b_18_rel0_id; b_18_rel0.RelationshipType = 1;
	b_18.RelationshipInfo.Relationships.Add(b_18_rel0);
	FRelationshipInfo b_18_rel1; FGuid b_18_rel1_id; FGuid::Parse("0BE05C674E43F2D52E3F558BF192E772", b_18_rel1_id); b_18_rel1.TargetID = b_18_rel1_id; b_18_rel1.RelationshipType = 1;
	b_18.RelationshipInfo.Relationships.Add(b_18_rel1);
	FRelationshipInfo b_18_rel2; FGuid b_18_rel2_id; FGuid::Parse("F76582C9406D13677F9251A376D755FD", b_18_rel2_id); b_18_rel2.TargetID = b_18_rel2_id; b_18_rel2.RelationshipType = 1;
	b_18.RelationshipInfo.Relationships.Add(b_18_rel2); UsedBlocks->Add(b_18);
	auto b_19 = make(1000, FVector(-148, 7, 14), FVector(1, 1, 1), FRotator(0, -90, -180), 10.000000, ""); b_19.BlockSpecificData.Add(TEXT("IsON"), TEXT("1")); b_19.BlockSpecificData.Add(TEXT("IsAutoregulated"), TEXT("0"));
	b_19.HasElectricityData = true; b_19.ElectricityInfo.CurrentObjectEnergy = 2000.000000;
	b_19.HasRelationshipData = true; FGuid b_19_rel_id; FGuid::Parse("F76582C9406D13677F9251A376D755FD", b_19_rel_id); b_19.RelationshipInfo.ID = b_19_rel_id;
	FRelationshipInfo b_19_rel0; FGuid b_19_rel0_id; FGuid::Parse("E0DE5F0C4742F701EBA06DB8EA53DD81", b_19_rel0_id); b_19_rel0.TargetID = b_19_rel0_id; b_19_rel0.RelationshipType = 0;
	b_19.RelationshipInfo.Relationships.Add(b_19_rel0); UsedBlocks->Add(b_19);
	auto b_20 = make(1000, FVector(-152, 18, 14), FVector(1, 1, 1), FRotator(0, -90, -180), 10.000000, ""); b_20.BlockSpecificData.Add(TEXT("IsON"), TEXT("1")); b_20.BlockSpecificData.Add(TEXT("IsAutoregulated"), TEXT("0"));
	b_20.HasElectricityData = true; b_20.ElectricityInfo.CurrentObjectEnergy = 2000.000000;
	b_20.HasRelationshipData = true; FGuid b_20_rel_id; FGuid::Parse("0BE05C674E43F2D52E3F558BF192E772", b_20_rel_id); b_20.RelationshipInfo.ID = b_20_rel_id;
	FRelationshipInfo b_20_rel0; FGuid b_20_rel0_id; FGuid::Parse("E0DE5F0C4742F701EBA06DB8EA53DD81", b_20_rel0_id); b_20_rel0.TargetID = b_20_rel0_id; b_20_rel0.RelationshipType = 0;
	b_20.RelationshipInfo.Relationships.Add(b_20_rel0); UsedBlocks->Add(b_20);
	auto b_21 = make(1000, FVector(-140, 18, 14), FVector(1, 1, 1), FRotator(0, -90, -180), 10.000000, ""); b_21.BlockSpecificData.Add(TEXT("IsON"), TEXT("1")); b_21.BlockSpecificData.Add(TEXT("IsAutoregulated"), TEXT("0"));
	b_21.HasElectricityData = true; b_21.ElectricityInfo.CurrentObjectEnergy = 2000.000000;
	b_21.HasRelationshipData = true; FGuid b_21_rel_id; FGuid::Parse("4A29A8B643CA566204D5FEB7C7F5ABB1", b_21_rel_id); b_21.RelationshipInfo.ID = b_21_rel_id;
	FRelationshipInfo b_21_rel0; FGuid b_21_rel0_id; FGuid::Parse("E0DE5F0C4742F701EBA06DB8EA53DD81", b_21_rel0_id); b_21_rel0.TargetID = b_21_rel0_id; b_21_rel0.RelationshipType = 0;
	b_21.RelationshipInfo.Relationships.Add(b_21_rel0); UsedBlocks->Add(b_21);
	auto b_22 = make(1200, FVector(-151, 17, 5), FVector(3, 3, 3), FRotator(0, 90, 0), 10.000000, ""); b_22.HasElectricityData = true; b_22.ElectricityInfo.CurrentObjectEnergy = 100000.000000;  UsedBlocks->Add(b_22);
	auto b_23 = make(1200, FVector(-151, 14, 5), FVector(3, 3, 3), FRotator(0, 90, 0), 10.000000, ""); b_23.HasElectricityData = true; b_23.ElectricityInfo.CurrentObjectEnergy = 100000.000000;  UsedBlocks->Add(b_23);
	auto b_24 = make(1200, FVector(-151, 17, 8), FVector(3, 3, 3), FRotator(0, 90, 0), 10.000000, ""); b_24.HasElectricityData = true; b_24.ElectricityInfo.CurrentObjectEnergy = 100000.000000;  UsedBlocks->Add(b_24);
	auto b_25 = make(1200, FVector(-151, 14, 8), FVector(3, 3, 3), FRotator(0, 90, 0), 10.000000, ""); b_25.HasElectricityData = true; b_25.ElectricityInfo.CurrentObjectEnergy = 100000.000000;  UsedBlocks->Add(b_25);
	auto b_26 = make(1500, FVector(-151, 12, 4), FVector(2, 2, 2), FRotator(0, -180, 0), 10.000000, ""); b_26.HasOxygenData = true; b_26.OxygenInfo.CurrentObjectOxygen = 1000.000000;  UsedBlocks->Add(b_26);
	auto b_27 = make(1500, FVector(-151, 12, 6), FVector(2, 2, 2), FRotator(0, 180, 0), 10.000000, ""); b_27.HasOxygenData = true; b_27.OxygenInfo.CurrentObjectOxygen = 1000.000000;  UsedBlocks->Add(b_27);
	auto b_28 = make(1500, FVector(-151, 12, 8), FVector(2, 2, 2), FRotator(0, 180, 0), 10.000000, ""); b_28.HasOxygenData = true; b_28.OxygenInfo.CurrentObjectOxygen = 1000.000000;  UsedBlocks->Add(b_28);
	auto b_29 = make(1500, FVector(-140, 17, 4), FVector(2, 2, 2), FRotator(0, 90, 0), 10.000000, ""); b_29.HasOxygenData = true; b_29.OxygenInfo.CurrentObjectOxygen = 1000.000000;  UsedBlocks->Add(b_29);
	auto b_30 = make(1500, FVector(-142, 17, 4), FVector(2, 2, 2), FRotator(0, 90, 0), 10.000000, ""); b_30.HasOxygenData = true; b_30.OxygenInfo.CurrentObjectOxygen = 1000.000000;  UsedBlocks->Add(b_30);
	auto b_31 = make(1500, FVector(-140, 17, 6), FVector(2, 2, 2), FRotator(0, 90, 0), 10.000000, ""); b_31.HasOxygenData = true; b_31.OxygenInfo.CurrentObjectOxygen = 1000.000000;  UsedBlocks->Add(b_31);
	auto b_32 = make(1500, FVector(-142, 17, 6), FVector(2, 2, 2), FRotator(0, 90, 0), 10.000000, ""); b_32.HasOxygenData = true; b_32.OxygenInfo.CurrentObjectOxygen = 1000.000000;  UsedBlocks->Add(b_32);
	auto b_33 = make(1500, FVector(-140, 17, 8), FVector(2, 2, 2), FRotator(0, 90, 0), 10.000000, ""); b_33.HasOxygenData = true; b_33.OxygenInfo.CurrentObjectOxygen = 1000.000000;  UsedBlocks->Add(b_33);
	auto b_34 = make(1500, FVector(-142, 17, 8), FVector(2, 2, 2), FRotator(0, 90, 0), 10.000000, ""); b_34.HasOxygenData = true; b_34.OxygenInfo.CurrentObjectOxygen = 1000.000000;  UsedBlocks->Add(b_34);
	c->PlayerPosition = FVector(-2925, 327, 163);
	c->PlayerRotation = FRotator(0, -53, 0);


	return c;
}

