#pragma once

#include "CoreMinimal.h"
#import <Foundation/Foundation.h>

@interface LibraryManager : NSObject
{
    //@public AnkrAds* ankrAds;
}

-(void) StartSession:(NSString*) appId WithDeviceId:(NSString*) deviceId WithPublicAddress:(NSString*) publicAddress WithLanguage:(NSString*) language;

@end
