// Caps Collective 2020

using UnrealBuildTool;
using System.IO;

public class RunForCovert : ModuleRules
{
	public RunForCovert(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "AIModule", "NavigationSystem", "ProceduralMeshComponent", "UMG" });

		PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true

		LoadLib(Target, "LibNoise", "LIB_NOISE"); // LibNoise is the name of the folder where you copied the files previously 
	}

	public bool LoadLib(ReadOnlyTargetRules Target, string libPath, string libName)
	{
		bool isLibrarySupported = false;

		if (Target.Platform == UnrealTargetPlatform.Win64 || Target.Platform == UnrealTargetPlatform.Win32 || Target.Platform == UnrealTargetPlatform.Mac) {
			isLibrarySupported = true;

			string PlatformString;
			switch (Target.Platform)
            {
              case UnrealTargetPlatform.Win64:
                  PlatformString = "x64";
                  break;
              case UnrealTargetPlatform.Win32:
                  PlatformString = "x86";
                  break;
              case UnrealTargetPlatform.Mac:
                  PlatformString = "macOS";
                  break;
            }

            string LibExtension = (Target.Platform == UnrealTargetPlatform.Mac) ? ".dylib" : ".lib";
			string ThirdPartyPath = Path.GetFullPath(Path.Combine(ModuleDirectory, "../../ThirdParty/"));
			string LibrariesPath = Path.Combine(ThirdPartyPath, libPath, "Libraries");

			PublicAdditionalLibraries.Add(Path.Combine(LibrariesPath, libPath + "." + PlatformString + LibExtension));
			PublicIncludePaths.Add(Path.Combine(ThirdPartyPath, libPath, "Includes"));
		}
		Definitions.Add(string.Format("WITH_" + libName + "_BINDING={0}", isLibrarySupported ? 1 : 0));
		return isLibrarySupported;
	}
}
