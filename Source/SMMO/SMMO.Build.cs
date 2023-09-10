// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class SMMO : ModuleRules
{
	public SMMO(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(
			new string[] { 
				"Core", 
				"CoreUObject", 
				"Engine", 
				"InputCore",
				"UMG",
                "libprotobuf"
            });

		PrivateDependencyModuleNames.AddRange(
			new string[]{
				"Slate",
				"SlateCore"
			});
    }
}
