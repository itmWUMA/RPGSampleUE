// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class RPGDemo : ModuleRules
{
	public RPGDemo(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput"
		});

		PrivateDependencyModuleNames.AddRange(new string[]
		{
			"ITM_Framework",
			"GameplayAbilities",
			"GameplayTags",
			"GameplayTasks",
			"UMG",
			"SlateCore",
			"MotionWarping",
			"Niagara",
		});
		
		PrivateIncludePaths.AddRange(new string[]
		{
			"RPGDemo",
		});
	}
}
