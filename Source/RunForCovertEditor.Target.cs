// Caps Collective 2020

using UnrealBuildTool;
using System.Collections.Generic;

public class RunForCovertEditorTarget : TargetRules
{
	public RunForCovertEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V2;

		ExtraModuleNames.AddRange( new string[] { "RunForCovert" } );
	}
}
