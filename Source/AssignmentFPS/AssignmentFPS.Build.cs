// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class AssignmentFPS : ModuleRules
{
	public AssignmentFPS(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput",  "AIModule", "UMG", "Slate", "SlateCore" });
    }
}
