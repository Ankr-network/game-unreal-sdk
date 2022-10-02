#include "LibraryManager.h"
#include <codecvt>

void LibraryManager::LoadLibrary()
{
	isInitialized = false;

	FString dllPath = *FPaths::ProjectPluginsDir() + FString("PluginName/Source/PluginName/Private/Windows/Libraries/Name.dll");
	bool exists = FPaths::FileExists(dllPath);

	if (exists)
	{
		HNDL = FPlatformProcess::GetDllHandle(*dllPath);
		if (!HNDL)
		{
			UE_LOG(LogTemp, Warning, TEXT("HNDL could not be loaded."));
			return;
		}

		dumpMethod = (DumpMethodHandle)FPlatformProcess::GetDllExport(HNDL, *FString("MethodNameExport"));
		if (dumpMethod != NULL)
		{
			isInitialized = true;
		}
	}
	else
	{
        UE_LOG(LogTemp, Warning, TEXT("%s is missing. If you are using any dynamic library, please copy the dll to the following path 'Plugins/PluginName/Source/PluginName/Private/Windows/Libraries'"), *dllPath);
        
		isInitialized = false;
	}
}

void LibraryManager::UnloadLibrary()
{
	if (HNDL)
	{
		dumpMethod = NULL;

		FPlatformProcess::FreeDllHandle(HNDL);
		HNDL = NULL;

		isInitialized = false;
	}
}

void LibraryManager::DumpMethod(bool _isDevelopment, FString _device_id)
{
	if (isInitialized)
	{
		dumpMethod(false, TCHAR_TO_UTF8(*_device_id), [](bool success, const char* result) {UE_LOG(LogTemp, Warning, TEXT("success: %d | result: %s"), success, *result);});
	}
}
