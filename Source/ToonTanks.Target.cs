// Copyrights Mattia De Prisco 2020

using UnrealBuildTool;

public class ToonTanksTarget : TargetRules
{
    public ToonTanksTarget(TargetInfo Target) : base(Target)
    {
        Type = TargetType.Game;
        DefaultBuildSettings = BuildSettingsVersion.V2;

        ExtraModuleNames.AddRange(new[] {"ToonTanks"});
    }
}