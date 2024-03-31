// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class CtrlAltEscape : ModuleRules
{
	public CtrlAltEscape(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput" });
		PublicDependencyModuleNames.AddRange(new string[] { "AIModule", "NavigationSystem", "GameplayTasks" }); // AI components
		PublicDependencyModuleNames.AddRange(new string[] { "GameplayAbilities", "GameplayTags", "GameplayTasks" }); // Gameplay Ability System components
		PublicDependencyModuleNames.AddRange(new string[] { "UMG", "Slate", "SlateCore" }); // UWidget components. UMG requires #include #include "Components/Widget.h"
        PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });// Stealth bar widget manipulation components
        

    }
}
