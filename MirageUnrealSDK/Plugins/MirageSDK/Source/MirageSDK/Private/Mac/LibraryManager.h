#pragma once

#import <Foundation/Foundation.h>

typedef int(*DumpMethodHandle)(unsigned char, unsigned long);

class MIRAGESDK_API LibraryManager
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

    void* HNDL = NULL;
    DumpMethodHandle dumpMethod = NULL;
    bool isInitialized;

    void LoadLibrary();
    void UnloadLibrary();
    void DumpMethod();

    NSString* FStringToNSString(FString _input);
    FString NSStringToFString(NSString* _input);
};
