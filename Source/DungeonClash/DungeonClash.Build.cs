// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class DungeonClash : ModuleRules
{
	public DungeonClash(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"AIModule",
			"StateTreeModule",
			"GameplayStateTreeModule",
			"UMG",
			"Slate"
		});

		PrivateDependencyModuleNames.AddRange(new string[] { });

		PublicIncludePaths.AddRange(new string[] {
			"DungeonClash",
			"DungeonClash/Variant_Platforming",
			"DungeonClash/Variant_Platforming/Animation",
			"DungeonClash/Variant_Combat",
			"DungeonClash/Variant_Combat/AI",
			"DungeonClash/Variant_Combat/Animation",
			"DungeonClash/Variant_Combat/Gameplay",
			"DungeonClash/Variant_Combat/Interfaces",
			"DungeonClash/Variant_Combat/UI",
			"DungeonClash/Variant_SideScrolling",
			"DungeonClash/Variant_SideScrolling/AI",
			"DungeonClash/Variant_SideScrolling/Gameplay",
			"DungeonClash/Variant_SideScrolling/Interfaces",
			"DungeonClash/Variant_SideScrolling/UI"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
