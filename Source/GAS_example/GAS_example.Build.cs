// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class GAS_example : ModuleRules
{
	public GAS_example(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core",
			"CoreOnline",
			"CoreUObject",
			"ApplicationCore",
			"Engine",
			"PhysicsCore",
			"GameplayTags",
			"GameplayTasks",
			"GameplayAbilities",
			"AIModule",
			"ModularGameplay",
			"DataRegistry",
			"ReplicationGraph",
			"GameFeatures",
			"SignificanceManager",
			"Hotfix",
			"Niagara",
			"ControlFlows",
			"PropertyPath"
		});

        PrivateDependencyModuleNames.AddRange(new string[]
        {
	        "InputCore",
	        "Slate",
	        "SlateCore",
	        "RenderCore",
	        "DeveloperSettings",
	        "EnhancedInput",
	        "NetCore",
	        "RHI",
	        "Projects",
	        "Gauntlet",
	        "UMG",
	        "CommonUI",
	        "CommonInput",
	        "AudioMixer",
	        "NetworkReplayStreaming",
	        "ClientPilot",
	        "AudioModulation",
	        "EngineSettings",
	        "DTLSHandlerComponent",
        });
        
        SetupGameplayDebuggerSupport(Target);
        SetupIrisSupport(Target);
    }
}
