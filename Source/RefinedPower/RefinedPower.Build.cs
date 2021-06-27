// ILikeBanas

using UnrealBuildTool;
using System.IO;
using Tools.DotNETCommon;

public class RefinedPower : ModuleRules
{
	public RefinedPower(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicIncludePaths.AddRange(
			new string[] {
				// ... add public include paths required here ...
			}
			);
				
		
		PrivateIncludePaths.AddRange(
			new string[] {
				// ... add other private include paths required here ...
			}
			);
			
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core", "CoreUObject",
				"Engine",
				"InputCore",
				"OnlineSubsystem", "OnlineSubsystemUtils", "OnlineSubsystemNULL",
				"SignificanceManager",
				"PhysX", "APEX", "PhysXVehicles", "ApexDestruction",
				"AkAudio",
				"ReplicationGraph",
				"UMG",
				"Json",
				"JsonUtilities",
				"AIModule",
				"NavigationSystem",
				"AssetRegistry",
				"GameplayTasks",
				"AnimGraphRuntime",
				"Slate", "SlateCore",
				"Landscape"
			});
		
		if (Target.Type == TargetRules.TargetType.Editor)
		{
			PublicDependencyModuleNames.AddRange(new string[] {"OnlineBlueprintSupport", "AnimGraph"});
		}
		
		PublicDependencyModuleNames.AddRange(new string[] {"FactoryGame", "SML"});

		var factoryGamePchPath = new DirectoryReference(Path.Combine(Target.ProjectFile.Directory.ToString(), "Source", "FactoryGame", "Public", "FactoryGame.h"));
		PrivatePCHHeaderFile = factoryGamePchPath.MakeRelativeTo(new DirectoryReference(ModuleDirectory));

		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"Core", "CoreUObject","Engine","Serialization","InputCore",
				"SML",
				"FactoryGame"
				// ... add private dependencies that you statically link with here ...	
			}
			);


		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				// ... add any modules that your module loads dynamically here ...
			}
			);
	}
}
