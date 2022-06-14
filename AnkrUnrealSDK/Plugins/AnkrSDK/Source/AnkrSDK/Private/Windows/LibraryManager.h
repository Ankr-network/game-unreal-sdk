#pragma once

#if PLATFORM_WINDOWS

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <iostream>
#include <string>
#include <functional>

typedef void(__cdecl* dll_DumpMethod)(const wchar_t*, std::function<void(bool, const wchar_t*)>);

class ANKRSDK_API LibraryManager
{
	
public:
	static LibraryManager& GetInstance()
	{
		static LibraryManager INSTANCE;
		return INSTANCE;
	}

private:
	LibraryManager() {}

public:
	LibraryManager(LibraryManager const&) = delete;
	void operator = (LibraryManager const&) = delete;

public:

	HINSTANCE HNDL_DLL = NULL;
	dll_DumpMethod	 dumpMethod = NULL;

	void DumpMethod(FString _dump);
	void Load();
	void Unload();

	static std::wstring GetWString(FString input);
	static std::string GetString(std::wstring _wstring);
	static FString GetFString(std::wstring _wstring);
};

#endif