#if PLATFORM_WINDOWS

#include "LibraryManager.h"
#include <codecvt>

void LibraryManager::Load()
{
	FString dllPath = *FPaths::ProjectPluginsDir() + FString("AnkrSDK/Source/AnkrSDK/Private/Windows/Libraries/libDump.dll");
	bool exists = FPaths::FileExists(dllPath);

	/*std::string path = std::string(TCHAR_TO_UTF8(*dllPath));
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
	std::wstring conversion = converter.from_bytes(path);*/
	
	HNDL_DLL = LoadLibrary(GetWString(dllPath).c_str());
	if (!HNDL_DLL)
	{
		UE_LOG(LogTemp, Warning, TEXT("LibraryManager - HNDL_DLL can not be loaded."));
		return;
	}
	UE_LOG(LogTemp, Warning, TEXT("LibraryManager - HNDL_DLL loaded."));

	dumpMethod = reinterpret_cast<dll_DumpMethod>(reinterpret_cast<void*>(GetProcAddress(HNDL_DLL, "dumpMethod")));
	if (!dumpMethod)
	{
		UE_LOG(LogTemp, Warning, TEXT("dumpMethod can not be loaded."));
		return;
	}
}

void LibraryManager::DumpMethod(FString _dump)
{
	dumpMethod(GetWString(_dump).c_str(), [](bool success, const wchar_t* data)
		{

		});
}

void LibraryManager::Unload()
{
	if (HNDL_DLL)
	{
		dumpMethod = NULL;

		bool success = FreeLibrary(HNDL_DLL);
		if (success)
		{
			UE_LOG(LogTemp, Warning, TEXT("LibraryManager - UnloadLibrary - Library unloaded successfully."));
		}
	}
}

std::wstring LibraryManager::GetWString(FString input)
{
	std::string raw = std::string(TCHAR_TO_UTF8(*input));
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
	std::wstring conversion = converter.from_bytes(raw);

	return conversion;
}

std::string LibraryManager::GetString(std::wstring _wstring)
{
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
	std::string conversion = converter.to_bytes(_wstring);
	return conversion;
}

FString LibraryManager::GetFString(std::wstring _wstring)
{
	FString conversion = FString(GetString(_wstring).c_str());
	return conversion;
}

#endif