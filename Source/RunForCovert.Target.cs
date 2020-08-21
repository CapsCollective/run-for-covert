// Caps Collective 2020

using UnrealBuildTool;
using System.Collections.Generic;

public class RunForCovertTarget : TargetRules
{
	public RunForCovertTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V2;

		ExtraModuleNames.AddRange( new string[] { "RunForCovert" } );
	}
}
