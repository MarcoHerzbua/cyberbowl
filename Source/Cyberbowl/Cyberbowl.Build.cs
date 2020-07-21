// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

using System.IO;
using UnrealBuildTool;

public class Cyberbowl : ModuleRules
{
	public Cyberbowl(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "Niagara", "FMODStudio" });

        PublicIncludePaths.AddRange(new string[] {
            Path.Combine(EngineDirectory, "../Engine/Plugins/FX/Niagara/Source/")
        });
	}
}
