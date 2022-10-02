#include "LibraryManager.h"

void RegisterLibrary(FString _dylib, std::string& _src, std::string& _dst)
{
    const FString UserLibrary = FString("/usr/local/lib/");
    
    FString name      = FPaths::GetBaseFilename(*_dylib);
    FString extension = FPaths::GetExtension(*_dylib);
    FString registration(UserLibrary + name + "." + extension);
    
    std::string stdSrc = std::string(TCHAR_TO_UTF8(*_dylib));
    std::string stdDst = std::string(TCHAR_TO_UTF8(*registration));
    
    std::basic_string<TCHAR> source(stdSrc.begin(), stdSrc.end());
    std::basic_string<TCHAR> destination(stdDst.begin(), stdDst.end());
    
    IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
    PlatformFile.DeleteFile(destination.c_str());
    PlatformFile.CopyFile(destination.c_str(), source.c_str());
    
    _src = stdSrc;
    _dst = stdDst;
}

void LibraryManager::LoadLibrary()
{
	isInitialized = false;

	FString dylib = *FPaths::ProjectPluginsDir() + FString("PluginName/Source/PluginName/Private/Mac/Libraries/Name.dylib");
    
	bool exists = FPaths::FileExists(dylib);
	if (exists)
	{
        std::string src;
        std::string dst;
        RegisterLibrary(dylib, src, dst);
		
		char* error;
		HNDL = dlopen(src.c_str(), RTLD_NOW | RTLD_GLOBAL);
		if ((error = dlerror()) != nullptr)
		{
			FString e(error);
			UE_LOG(LogTemp, Warning, TEXT("LibraryManager - dlopen() - failed: %s"), *e);
		}
        
        if(HNDL == NULL)
        {
            UE_LOG(LogTemp, Warning, TEXT("LibraryManager - Library can not be loaded at path: %s"), *dylib);
            return;
        }
			
        dumpMethod = (DumpMethodHandle)dlsym(HNDL, "MethodNameExport");
        
        isInitialized = true;
		
	}
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("%s is missing. If you are using any dynamic library, please copy the dylib to the following path 'Plugins/PluginName/Source/PluginName/Private/Mac/Libraries'"), *dylib);
        
        isInitialized = false;
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

void LibraryManager::DumpMethod()
{
    if (isInitialized)
    {
        (*dumpMethod)(0x00, 0);
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
