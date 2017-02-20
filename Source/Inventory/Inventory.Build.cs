using UnrealBuildTool;

public class Inventory : ModuleRules
{
    public Inventory(TargetInfo Target)
    {
        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "Commons" });
        PrivateDependencyModuleNames.AddRange(new string[] { "RenderCore" });
        PrivateDependencyModuleNames.AddRange(new string[] { "PakFile", "StreamingFile" });


        PublicIncludePaths.AddRange(new string[] { "Inventory/Public" });

        PrivateIncludePaths.AddRange(new string[] { "Inventory/Private" });


    }
}