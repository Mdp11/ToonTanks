// Copyrights Mattia De Prisco 2020

using UnrealBuildTool;

public class ToonTanksEditorTarget : TargetRules
{
    public ToonTanksEditorTarget(TargetInfo Target) : base(Target)
    {
        Type = TargetType.Editor;
        DefaultBuildSettings = BuildSettingsVersion.V2;

        ExtraModuleNames.AddRange(new[] {"ToonTanks"});
    }
}