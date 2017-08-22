// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

using System.Collections.Generic;
using UnrealBuildTool;

public class TauCetiF2EditorTarget : TargetRules
{
    public TauCetiF2EditorTarget(TargetInfo Target) : base(Target)
    {
        Type = TargetType.Editor;
        ExtraModuleNames.Add("TauCetiF2");
        ExtraModuleNames.Add("Blocks");
        ExtraModuleNames.Add("Commons");
        ExtraModuleNames.Add("GameSave");
        ExtraModuleNames.Add("Inventory");
    }
}