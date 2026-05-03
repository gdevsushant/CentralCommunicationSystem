// Copyright (c) Sushant 2026. All Rights Reserved.

using UnrealBuildTool;

public class CentralCommunicationSystem : ModuleRules
{
    public CentralCommunicationSystem(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[]
        {
            "Core",
            "CoreUObject",
            "Engine",
            "GameplayTags"
        });

        PrivateDependencyModuleNames.AddRange(new string[]
        {
        });
    }
}