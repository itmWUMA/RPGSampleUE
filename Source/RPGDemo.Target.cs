// Copyright 2024 Itmwuma

using UnrealBuildTool;
using System.Collections.Generic;

public class RPGDemoTarget : TargetRules
{
	public RPGDemoTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V4;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_3;
		ExtraModuleNames.Add("RPGDemo");
	}
}
