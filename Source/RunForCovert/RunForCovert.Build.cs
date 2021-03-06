// Caps Collective 2020

using UnrealBuildTool;
using System.IO;

public class RunForCovert : ModuleRules
{
	public RunForCovert(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core", "CoreUObject", "Engine", "InputCore", "AIModule", "NavigationSystem", "ProceduralMeshComponent",
			"UMG"
		});

		PrivateDependencyModuleNames.AddRange(new string[] {"Slate", "SlateCore"});

		PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
