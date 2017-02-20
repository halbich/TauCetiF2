// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class TauCetiF2EditorTarget : TargetRules
{
    public TauCetiF2EditorTarget(TargetInfo Target)
    {
        Type = TargetType.Editor;
    }

    //
    // TargetRules interface.
    //

    public override void SetupBinaries(
        TargetInfo Target,
        ref List<UEBuildBinaryConfiguration> OutBuildBinaryConfigurations,
        ref List<string> OutExtraModuleNames
        )
    {
        OutExtraModuleNames.Add("TauCetiF2");
        OutExtraModuleNames.Add("Blocks");
        OutExtraModuleNames.Add("Commons");
        OutExtraModuleNames.Add("GameSave");
        OutExtraModuleNames.Add("GameWorld");
        OutExtraModuleNames.Add("Inventory");
    }
}
