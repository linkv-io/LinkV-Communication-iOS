//
//  LinkVRTCEngine.m
//  iOS_Zego
//
//  Created by Wing on 2020/8/4.
//  Copyright © 2020 Wing. All rights reserved.
//

#import "LinkVRTCEngine.h"
//#import "ZegoLive.h"
#import "LVRTCLive.h"

@implementation LinkVRTCEngine

+ (instancetype)createEngine:(SDKType)type
                       appId:(NSString *)appId
                      appKey:(NSString *)appKey
                   isTestEnv:(BOOL)isTestEnv
                  completion:(LVCodeCompletion)completion {
    
    if (type == SDK_TYPE_ZEGO) {

#if __has_include(<ZegoExpressEngine/ZegoExpressEngine.h>)
        ZegoLive *live = [ZegoLive new];
        [live initSDK:appId appKey:appKey isTestEnv:isTestEnv completion:completion];
        return live;
#endif
        
    } else if (type == SDK_TYPE_LVRTC) {
        LVRTCLive *live = [LVRTCLive new];
        [live initSDK:appId appKey:appKey isTestEnv:isTestEnv completion:completion];
        return live;
    } else {
        NSAssert(false, @"不支持的SDK");
    }
    
    return nil;
}

@end
