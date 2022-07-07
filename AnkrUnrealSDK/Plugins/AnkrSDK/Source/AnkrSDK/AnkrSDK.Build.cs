// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System;
using System.Collections.Generic;
using System.IO;

public class AnkrSDK : ModuleRules
{
	public AnkrSDK(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
        
		PublicIncludePaths.AddRange(
			new string[] {
				// ... add public include paths required here ...
				Path.Combine(ModuleDirectory, "Public")
			}
			);
				
		
		PrivateIncludePaths.AddRange(
			new string[] {
				// ... add other private include paths required here ...
				Path.Combine(ModuleDirectory, "Private")
			}
			);
			
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
				"HTTP",
				"Json",
				"JsonUtilities"
				// ... add other public dependencies that you statically link with here ...
			}
			);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				
			}
			);
		
		
		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				// ... add any modules that your module loads dynamically here ...
			}
			);

        if(Target.Platform == UnrealTargetPlatform.Win64)
        {
            PrivateIncludePaths.Add(Path.Combine(ModuleDirectory, "Private/Windows"));
		}

		if (Target.Platform == UnrealTargetPlatform.Mac)
		{
            //PublicIncludePaths.Add(ModuleDirectory + "/Private/Mac/Libraries/Framework.framework/Headers");
            //PublicFrameworks.Add(ModuleDirectory + "/Private/Mac/Libraries/Framework.framework");
           
			PrivateIncludePaths.Add(Path.Combine(ModuleDirectory, "Private/Mac"));
		}

		if (Target.Platform == UnrealTargetPlatform.IOS)
		{
			PrivateDependencyModuleNames.AddRange(new string[] { "Launch" });
            
            PublicIncludePaths.Add(ModuleDirectory + "/Private/iOS/Libraries/AdsBridge.framework/Headers");
            PublicAdditionalFrameworks.Add(new Framework("AdsBridge", ModuleDirectory + "/Private/iOS/Libraries/AdsBridge.framework"));
            
            //PublicIncludePaths.Add(ModuleDirectory + "/Private/iOS/Libraries/Framework.framework/Headers");
            //PublicAdditionalFrameworks.Add(new Framework("Framework", ModuleDirectory + "/Private/iOS/Libraries/Framework.framework"));

            PrivateIncludePaths.Add(Path.Combine(ModuleDirectory, "Private/iOS"));
            string PluginPath = Utils.MakePathRelativeTo(ModuleDirectory, Target.RelativeEnginePath);
            AdditionalPropertiesForReceipt.Add("IOSPlugin", Path.Combine(PluginPath, "AnkrSDKiOS_UPL.xml"));
		}

		if (Target.Platform == UnrealTargetPlatform.Android)
		{
            PrivateDependencyModuleNames.AddRange(new string[] { "Launch" });

			PrivateIncludePaths.Add(Path.Combine(ModuleDirectory, "Private/Android"));
			string PluginPath = Utils.MakePathRelativeTo(ModuleDirectory, Target.RelativeEnginePath);
			AdditionalPropertiesForReceipt.Add("AndroidPlugin", Path.Combine(PluginPath, "AnkrSDKAndroid_UPL.xml"));
		}
	}
}
