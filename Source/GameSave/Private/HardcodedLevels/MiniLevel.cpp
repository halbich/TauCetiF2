#include "GameSave.h"
#include "NewGameSaveHolder.h"

USaveGameCarrier* UNewGameSaveHolder::getMiniLevelGameSave(bool full)
{
	auto c = USaveGameCarrier::GetEmptyCarrier();

	c->SaveName = NSLOCTEXT("TCF2LocSpace", "LC.SaveSystemMiniLevel", "Tìžké zaèátky").ToString();
	c->SaveFullDescription = NSLOCTEXT("TCF2LocSpace", "LC.SaveSystemMiniLevelFullDescr", "Tento level má velice málo poèáteèních zásob, ale poskytuje nejvíce prostoru pro to, aby sis postavil svoji základnu pøesnì podle svých pøedstav.");
	c->FullFilePath = TEXT("_system_miniLevel");
	c->IsSystemSave = true;
	c->SaveLoaded = true;
	c->HardcodedLevelName = ENamedHardcodedLevel::MiniLevel;

	if (!full)
		return c;

	//c->CurrentTime = 14400.0f; 
	//c->CurrentTime = 34920.0f;
	c->PlayerUseFPSCamera = true;
	c->IsCreativeMode = true;
	c->PlayerHealth = 1000;

	c->PlayerOxygenComponent.CurrentObjectOxygen = 1000.0f;
	c->PlayerElectricityComponent.CurrentObjectEnergy = 150000.0f;

	c->inventoryTags = makeDefault();

	c->buildableBlocks.Add(makeBuildable(DeleteID, FVector(1, 1, 1)));

	auto UsedBlocks = &c->usedBlocks;

	auto b_0 = make(0, FVector(-146, 12, 1), FVector(15, 15, 4), FRotator(0, -180, 0), 9000.000000, ""); b_0.HasElectricityData = true; b_0.ElectricityInfo.CurrentObjectEnergy = 0.000000;  UsedBlocks->Add(b_0);
	auto b_1 = make(700, FVector(-142, 9, 9), FVector(7, 7, 11), FRotator(0, 90, 0), 10.000000, ""); b_1.AdditionalFlags.Add(TEXT("DoorOpening"), 0);
	b_1.BlockSpecificData.Add(TEXT("DoorState"), TEXT("0")); b_1.BlockSpecificData.Add(TEXT("DoorYaw"), TEXT("0.0"));
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
	c->PlayerPosition = FVector(-2948, 328, 163);
	c->PlayerRotation = FRotator(0, -57, 0);
	c->PlayerCameraRotation = FRotator(350.181396, 303.457184, 0.0);


	return c;
}