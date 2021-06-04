//
//  LVRTCLive.h
//  iOS_Zego
//
//  Created by Wing on 2020/8/4.
//  Copyright © 2020 Wing. All rights reserved.
//

#import "LinkVRTCEngine.h"

NS_ASSUME_NONNULL_BEGIN

@interface LVRTCLive : LinkVRTCEngine

@property (nonatomic,weak) id<RTCCallbackDelegate> delegate;

@end

NS_ASSUME_NONNULL_END

