// Copyright 2024 Itmwuma

using UnrealBuildTool;
using System.Collections.Generic;

public class RPGDemoEditorTarget : TargetRules
{
	public RPGDemoEditorTarget( TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V4;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_3;
		ExtraModuleNames.Add("RPGDemo");
	}
}
