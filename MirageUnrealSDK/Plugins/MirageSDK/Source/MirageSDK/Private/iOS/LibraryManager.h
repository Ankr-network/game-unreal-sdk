#pragma once

#import <Foundation/Foundation.h>

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

    void LoadLibrary();
    void UnloadLibrary();
	void Initialize(FString appId, FString deviceId, FString publicAddress, FString language);
	void LoadAd(FString _unitId);
	void ShowView(FString _unitId);
    
    NSString* FStringToNSString(FString _input);
    FString NSStringToFString(NSString* _input);
};
