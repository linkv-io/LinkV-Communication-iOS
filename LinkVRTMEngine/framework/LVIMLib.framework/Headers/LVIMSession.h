//
//  LVIMSession.h
//  CMIMLib
//
//  Created by Wing on 2020/8/31.
//  Copyright © 2020 cmcm. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "LVIMMessage.h"

NS_ASSUME_NONNULL_BEGIN

@interface LVIMSession : NSObject

@property (nonatomic, strong) LVIMMessage *lastMsg;
@property (nonatomic, assign) NSInteger unreadCount;

@end

NS_ASSUME_NONNULL_END
