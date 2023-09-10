using UnrealBuildTool;
using System;
using System.IO;
using System.Collections.Generic;

public class libprotobuf : ModuleRules
{
	public libprotobuf(ReadOnlyTargetRules Target) : base(Target)
	{
		Type = ModuleType.External;

        string protobufPath = ModuleDirectory;

        PublicDefinitions.AddRange(
			new string[]
			{
					"GOOGLE_PROTOBUF_NO_RTTI=1",
					"GOOGLE_PROTOBUF_USE_UNALIGNED=0",
					"GOOGLE_PROTOBUF_INTERNAL_DONATE_STEAL_INLINE=0"
			});

        PublicSystemIncludePaths.Add(Path.Combine(protobufPath, "include"));
		PublicSystemIncludePaths.Add(Path.Combine(protobufPath, "lib"));

        if (Target.Platform == UnrealTargetPlatform.Win64)
		{
            PublicAdditionalLibraries.Add(Path.Combine(protobufPath, "lib", ConfigurationDir(Target.Configuration), "libprotobuf.lib"));
        }
		else
		{
			throw new Exception("Currently " + Target.Platform.ToString() + " is unsupported");
		}

    }

	public string ConfigurationDir(UnrealTargetConfiguration Configuration)
	{
		if (Configuration == UnrealTargetConfiguration.Debug || Configuration == UnrealTargetConfiguration.DebugGame)
		{
			return "Debug";
		}
		else
		{
			return "Release";
		}
	}
}

