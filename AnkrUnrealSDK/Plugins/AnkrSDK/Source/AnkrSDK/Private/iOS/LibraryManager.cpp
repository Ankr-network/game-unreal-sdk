#include "LibraryManager.h"
#import "AdsBridge-Swift.h"

void LibraryManager::LoadLibrary()
{
    // Nothing to do here for now.
}

void LibraryManager::UnloadLibrary()
{
    // Nothing to do here for now.
}

void LibraryManager::Initialize(FString appId, FString deviceId, FString publicAddress, FString language)
{
    [Bridge InitializeWithAppId: FStringToNSString(appId) deviceId: FStringToNSString(deviceId) publicAddress: FStringToNSString(publicAddress) language: FStringToNSString(language)];
}

void LibraryManager::LoadAd(FString _unitId)
{
    [Bridge LoadAdWithUnitId : FStringToNSString(_unitId)] ;
}

void LibraryManager::ShowView(FString _unitId)
{
    [Bridge ShowWithUnitId : FStringToNSString(_unitId)] ;
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
