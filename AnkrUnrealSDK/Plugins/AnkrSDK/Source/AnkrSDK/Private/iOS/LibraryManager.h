#pragma once

#import <Foundation/Foundation.h>
#import "AdsBridge-Swift.h"

@interface LibraryManager : NSObject
{
}

-(void)Initialize:(NSString*)appId deviceId:(NSString*)deviceId publicAddress:(NSString*)publicAddress language:(NSString*)language;
-(void)LoadAd:(NSString*)unitId;
-(void)Show:(NSString*)unitId;

@end
