//
//  LVIMSDK.h
//  LVIMSDK
//
//  Created by pangbingxin on 2019/9/9.
//  Copyright © 2019 cmcm. All rights reserved.
//

#import <UIKit/UIKit.h>

//! Project version number for LVIMSDK.
FOUNDATION_EXPORT double LVIMSDKVersionNumber;
//! Project version string for LVIMSDK.
FOUNDATION_EXPORT const unsigned char LVIMSDKVersionString[];


#if __has_include(<YYKit/YYKit.h>)
    #import <LVIMSDK/LVIMRoomState.h>
    #import <LVIMSDK/LVIMMessage.h>
    #import <LVIMSDK/LVIMConfig.h>
    #import <LVIMSDK/LVIMSDK.h>
    #import <LVIMSDK/LVIMSession.h>
#else
    #import "LVIMRoomState.h"
    #import "LVIMMessage.h"
    #import "LVIMConfig.h"
    #import "LVIMSDK.h"
    #import "LVIMSession.h"
#endif





