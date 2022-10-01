#include "LibraryManager.h"
#include <dlfcn.h>
#include <string>
#include "HAL/PlatformFileManager.h"
#include "GenericPlatform/GenericPlatformFile.h"

void LibraryManager::LoadLibrary()
{
	isInitialized = false;

	const FString UserLibrary = FString("/usr/local/lib/");

	FString dllPath = *FPaths::ProjectPluginsDir() + FString("PluginName/Source/PluginName/Private/Mac/Libraries/Name.dylib");
	bool exists = FPaths::FileExists(dllPath);

	if (FPaths::FileExists(dllPath))
	{
		std::string fullPath = std::string(TCHAR_TO_UTF8(*dllPath));

		FString fileName = FPaths::GetBaseFilename(*dllPath);
		FString fileExt = FPaths::GetExtension(*dllPath);

		FString libraryPath(UserLibrary + fileName + "." + fileExt);
		std::string conversion = std::string(TCHAR_TO_UTF8(*libraryPath));

		std::basic_string<TCHAR> source(fullPath.begin(), fullPath.end());
		std::basic_string<TCHAR> destination(conversion.begin(), conversion.end());

		IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
		PlatformFile.DeleteFile(destination.c_str());
		PlatformFile.CopyFile(destination.c_str(), source.c_str());
		
		char* error;
		HNDL = dlopen(fullPath.c_str(), RTLD_NOW | RTLD_GLOBAL);
		if ((error = dlerror()) != nullptr)
		{
			FString e(error);
			UE_LOG(LogTemp, Warning, TEXT("dlopen failed: %s"), *e);
		}

		if (HNDL != NULL)
		{
			isInitialized = true;
			dumpMethod = (DumpMethodHandle)dlsym(HNDL, "MethodNameExport");
		}
	}
}

void LibraryManager::UnloadLibrary()
{
	if (HNDL)
	{
		dumpMethod = NULL;

		dlclose(HNDL);
		HNDL = NULL;

		isInitialized = false;
	}
}

NSString* LibraryManager::FStringToNSString(FString _input)
{
    std::string source = std::string(TCHAR_TO_UTF8(*_input));
    NSString* conversion = [[NSString alloc] initWithUTF8String:source.c_str()];
    return conversion;
}
FString LibraryManager::NSStringToFString(NSString* _input)
{
    FString conversion([_input UTF8String]);
    return conversion;
}