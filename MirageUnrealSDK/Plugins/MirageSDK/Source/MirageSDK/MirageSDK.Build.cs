// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System;
using System.Collections.Generic;
using System.IO;

public class MirageSDK : ModuleRules
{
	public MirageSDK(ReadOnlyTargetRules Target) : base(Target)
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

		if (Target.Platform == UnrealTargetPlatform.Win64)
		{
			SetWindowsDependencies();
		}

		else if (Target.Platform == UnrealTargetPlatform.Mac)
		{
			SetMacDependencies();
		}

		else if (Target.Platform == UnrealTargetPlatform.IOS)
		{
			SetiOSDependencies();
		}

		else if (Target.Platform == UnrealTargetPlatform.Android)
		{
			SetAndroidDependencies();
		}
	}

	void SetWindowsDependencies()
    {
        PrivateIncludePaths.Add(Path.Combine(ModuleDirectory, "Private/Windows"));

		//string LibPath = Path.Combine(ModuleDirectory, "Private", "Windows", "Libraries", "lib.lib");
		//string DLLPath = Path.Combine(ModuleDirectory, "Private", "Windows", "Libraries", "dll.dll");

		//PublicAdditionalLibraries.Add(LibPath);
		//RuntimeDependencies.Add(DLLPath);
		//PublicDelayLoadDLLs.Add("dll.dll");
	}

	void SetMacDependencies()
    {
        //PublicIncludePaths.Add(ModuleDirectory + "/Private/Mac/Libraries/Framework.framework/Headers");
        //PublicFrameworks.Add(ModuleDirectory + "/Private/Mac/Libraries/Framework.framework");

        PrivateIncludePaths.Add(Path.Combine(ModuleDirectory, "Private/Mac"));
    }

	void SetiOSDependencies()
    {
        //PublicIncludePaths.Add(ModuleDirectory + "/Private/iOS/Libraries/Framework.framework/Headers");
        //PublicAdditionalFrameworks.Add(new Framework("Framework", ModuleDirectory + "/Private/iOS/Libraries/Framework.framework"));

        PrivateDependencyModuleNames.AddRange(new string[] { "Launch" });

		string FrameworkPath = ModuleDirectory + "/Private/iOS/Libraries/AdsBridge.framework";
		if (Directory.Exists(FrameworkPath))
		{
			PublicIncludePaths.Add(FrameworkPath + "/Headers");
			PublicAdditionalFrameworks.Add(new Framework("AdsBridge", FrameworkPath));
		}

        PrivateIncludePaths.Add(Path.Combine(ModuleDirectory, "Private/iOS"));
        string PluginPath = Utils.MakePathRelativeTo(ModuleDirectory, Target.RelativeEnginePath);
        AdditionalPropertiesForReceipt.Add("IOSPlugin", Path.Combine(PluginPath, "MirageSDKiOS_UPL.xml"));
    }

	void SetAndroidDependencies()
    {
        PrivateDependencyModuleNames.AddRange(new string[] { "Launch" });

        PrivateIncludePaths.Add(Path.Combine(ModuleDirectory, "Private/Android"));
        string PluginPath = Utils.MakePathRelativeTo(ModuleDirectory, Target.RelativeEnginePath);
        AdditionalPropertiesForReceipt.Add("AndroidPlugin", Path.Combine(PluginPath, "MirageSDKAndroid_UPL.xml"));
	}
}
