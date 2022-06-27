#include "LibraryManager.h"

@implementation LibraryManager

-(id) init
{
    if(self == [super init])
    {
        UE_LOG(LogTemp, Warning, TEXT("iOS - LibraryManager.h - init"));
    }
    
    return self;
}

-(void)Initialize:(NSString*)appId deviceId:(NSString*)deviceId publicAddress:(NSString*)publicAddress language:(NSString*)language
{
    UE_LOG(LogTemp, Warning, TEXT("iOS - LibraryManager.h - Initialize - appId: %@ | deviceId: %@ | publicAddress: %@ | language: %@"), appId, deviceId, publicAddress, language);
    
    [Bridge InitializeWithAppId:appId deviceId:deviceId publicAddress:publicAddress language:language];
}
-(void)LoadAd:(NSString*)unitId
{
    UE_LOG(LogTemp, Warning, TEXT("iOS - LibraryManager.h - LoadAd - unitId: %@"), unitId);
    
    [Bridge LoadAdWithUnitId: unitId];
}
-(void)Show:(NSString*)unitId
{
    UE_LOG(LogTemp, Warning, TEXT("iOS - LibraryManager.h - Show - unitId: %@"), unitId);
    
    [Bridge ShowWithUnitId:unitId];
}

@end
