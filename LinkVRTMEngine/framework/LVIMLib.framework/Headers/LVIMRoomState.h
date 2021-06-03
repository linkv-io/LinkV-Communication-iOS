//
//  LVIMRoomState.h
//  LVIM
//
//  Created by pangbingxin on 2019/7/26.
//  Copyright © 2019 cmcm. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface LVIMRoomState : NSObject
@property (nonatomic, copy) NSString* mRoomID;
@property (nonatomic, assign) int32_t mRoomState;
@property (nonatomic, assign) void* mRoomListener;

/// 房间状态 初始化方法
/// @param rid 房间id
/// @param state 状态
/// @param listener 调用该函数的对象
-(instancetype)initWithRoomID:(NSString*)rid state:(int32_t)state listener:(void*)listener;

/// 是否是无效的
-(BOOL)isInvalid;

/// 房间已经创建完成
-(BOOL)isCreated;

/// 房间正在创建
-(BOOL)isCreating;

/// 已经加入房间
-(BOOL)isJoined;

/// 正在加入房间
-(BOOL)isJoining;

/// 已经离开房间
-(BOOL)isLeaved;

/// 正在离开房间
-(BOOL)isLeaving;

@end
