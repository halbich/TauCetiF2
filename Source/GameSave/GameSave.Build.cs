using UnrealBuildTool;

public class GameSave : ModuleRules
{
    public GameSave(ReadOnlyTargetRules ROTRules) : base(ROTRules)
    {
        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "Commons" });
        PrivateDependencyModuleNames.AddRange(new string[] { "RenderCore" });

        PublicIncludePaths.AddRange(new string[] { "GameSave/Public", "GameSave/Public/BlockComponents", "GameSave/Public/Inventory" });

        PrivateIncludePaths.AddRange(new string[] { "GameSave/Private" });
    }
}