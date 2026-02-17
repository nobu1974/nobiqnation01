// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class NobiqNation01 : ModuleRules
{
	public NobiqNation01(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"ProceduralMeshComponent",
			"EnhancedInput",
			"AIModule",
			"StateTreeModule",
			"GameplayStateTreeModule",
			"UMG",
			"Slate"
		});

		PrivateDependencyModuleNames.AddRange(new string[] { });

		PublicIncludePaths.AddRange(new string[] {
			"NobiqNation01",
			"NobiqNation01/Variant_Platforming",
			"NobiqNation01/Variant_Platforming/Animation",
			"NobiqNation01/Variant_Combat",
			"NobiqNation01/Variant_Combat/AI",
			"NobiqNation01/Variant_Combat/Animation",
			"NobiqNation01/Variant_Combat/Gameplay",
			"NobiqNation01/Variant_Combat/Interfaces",
			"NobiqNation01/Variant_Combat/UI",
			"NobiqNation01/Variant_SideScrolling",
			"NobiqNation01/Variant_SideScrolling/AI",
			"NobiqNation01/Variant_SideScrolling/Gameplay",
			"NobiqNation01/Variant_SideScrolling/Interfaces",
			"NobiqNation01/Variant_SideScrolling/UI"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
