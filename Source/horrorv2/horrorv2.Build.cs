// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class horrorv2 : ModuleRules
{
	public horrorv2(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput" });
	}
}
