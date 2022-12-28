#include "LibraryManager.h"
#include <codecvt>
#include <sstream>
#include <iomanip>

#define WIN32_LEAN_AND_MEAN
#include "windows.h"

void LibraryManager::Load()
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

void LibraryManager::Unload()
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

std::string LibraryManager::GetString(std::wstring _wstring)
{
	int count = WideCharToMultiByte(CP_UTF8, 0, _wstring.c_str(), _wstring.length(), NULL, 0, NULL, NULL);
	std::string str(count, 0);
	WideCharToMultiByte(CP_UTF8, 0, _wstring.c_str(), -1, &str[0], count, NULL, NULL);
	return str;
}
std::wstring LibraryManager::GetWString(FString input)
{
	std::string raw = std::string(TCHAR_TO_UTF8(*input));
	int count = MultiByteToWideChar(CP_UTF8, 0, raw.c_str(), raw.length(), NULL, 0);
	std::wstring wstr(count, 0);
	MultiByteToWideChar(CP_UTF8, 0, raw.c_str(), raw.length(), &wstr[0], count);
	return wstr;
}
