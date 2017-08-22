using UnrealBuildTool;

public class Commons : ModuleRules
{
    public Commons(ReadOnlyTargetRules ROTRules) : base(ROTRules)
    {
        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine" });
        PrivateDependencyModuleNames.AddRange(new string[] { "RenderCore" });

        PublicIncludePaths.AddRange(new string[] { "Commons/Public" });

        PrivateIncludePaths.AddRange(new string[] { "Commons/Private" });
    }
}