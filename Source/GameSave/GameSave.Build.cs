using UnrealBuildTool;

public class GameSave : ModuleRules
{
    public GameSave(TargetInfo Target)
    {
        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "Commons", "Blocks", "Inventory" });
        PrivateDependencyModuleNames.AddRange(new string[] { "RenderCore" });

        PublicIncludePaths.AddRange(new string[] { "GameSave/Public" });

        PrivateIncludePaths.AddRange(new string[] { "GameSave/Private" });
    }
}