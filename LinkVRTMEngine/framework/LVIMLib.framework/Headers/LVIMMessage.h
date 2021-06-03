//
//  LVIMMessage.h
//  LVIM
//
//  Created by pangbingxin on 2018/4/3.
//  Copyright © 2018年 cmcm. All rights reserved.
//

#import <Foundation/Foundation.h>

@class LVIMMessage;

static NSString *const Flag_Event = @"flag_event"; // 赋值extend3字段为flag_event

/**
 @brief 发送消息的回调
 @param ecode 错误代码(0表示无错误)
 @param rcode 服务端返回码
 @param lvsgid 客户端消息id
 @param smsgid 服务端消息id
 @param stime 发送时间戳(毫秒)
 @param msg 发送消息的消息体
 **/
typedef void(^LVIM_SEND_MESSAGE_CALLBACK)(int ecode, int rcode, int64_t lvsgid, int64_t smsgid, int64_t stime, LVIMMessage *msg);

/**
 @brief 发送HTTP消息的回调
 @param ecode 错误代码(0表示无错误)
 @param rcode 服务端返回码
 @param scode 服务端错误码
 @param hcode HTTP错误码
 @param hdelay HTTP耗时
 @param sdelay 服务端整体耗时
 @param qdelay 客户端整体耗时
 @param lvsgid 客户端消息id
 @param smsgid 服务端消息id
 @param stime 发送时间戳(毫秒)
 @param response 响应内容
 **/
typedef void(^LVIM_SEND_HTTP_MESSAGE_CALLBACK)(int ecode, int rcode, int scode, int hcode, int64_t hdelay, int64_t sdelay, int64_t qdelay, int64_t lvsgid, int64_t smsgid, int64_t stime, const char* response);

//消息类型
typedef NS_ENUM(int, LVIM_MESSAGE_LVDTYPE) {
    //最小无效消息类型
    LVIM_MESSAGE_LVDTYPE_MIN = -1,
    //直播间消息
    LVIM_MESSAGE_LVDTYPE_ROOM = 20,
    //群组消息
    LVIM_MESSAGE_LVDTYPE_GROUP = 11,
    //私信消息
    LVIM_MESSAGE_LVDTYPE_PRIVATE = 10,
    //推送消息
    LVIM_MESSAGE_LVDTYPE_PUSH = 12,
    //事件消息
    LVIM_MESSAGE_LVDTYPE_EVENT = 30,
    //公告消息
    LVIM_MESSAGE_LVDTYPE_OFFICIAL = 31,
    //最大无效消息类型
    LVIM_MESSAGE_LVDTYPE_MAX
};

//内容类型
typedef NS_ENUM(int, LVIM_MESSAGE_SUBTYPE) {
    //最小无效内容类型
    LVIM_MESSAGE_SUBTYPE_MIN = -1,
    //自定义文本内容类型
    LVIM_MESSAGE_SUBTYPE_TEXT = 0,
    //自定义图片内容类型
    LVIM_MESSAGE_SUBTYPE_IMAGE = 1,
    //自定义音频内容类型
    LVIM_MESSAGE_SUBTYPE_AUDIO = 2,
    //自定义视频内容类型
    LVIM_MESSAGE_SUBTYPE_VIDEO = 3,
    //最大无效内容类型
    LVIM_MESSAGE_SUBTYPE_MAX
};


typedef NS_ENUM(int, LVIM_MESSAGE_SEND_STATE) {
    //消息发送中
    LVIM_MESSAGE_SENDING = 0,
    //消息发送失败
    LVIM_MESSAGE_FAILED = 1,
    //消息发送成功——对方未读
    LVIM_MESSAGE_SENT_SUCC_UNREAD = 2,
    //消息发送成功——对方已读
    LVIM_MESSAGE_SENT_SUCC_READ = 3,
    //消息已收到——自己未读
    LVIM_MESSAGE_RECEIVED_UNREAD = 4,
    //消息已收到——自己已读
    LVIM_MESSAGE_RECEIVED_READ = 5,
};


@interface LVIMMessage : NSObject
//数据库消息ID
@property (nonatomic, assign) int64_t mDBID;
//消息是否已读(目前只有客服接口有)
@property (nonatomic, assign) uint8_t mIsReaded;
//客户端消息ID
@property (nonatomic, assign) int64_t mLVsgID;
//服务端消息ID(只有返回后才有值)
@property (nonatomic, assign) int64_t mSMsgID;
//消息序列(服务端时间戳)
@property (nonatomic, assign) int64_t mSequence;
//消息时间(客户端)
@property (nonatomic, assign) int64_t mClientTimestamp;
//消息时间(服务端)
@property (nonatomic, assign) int64_t mServerTimestamp;
//发送出去的时间戳
@property (nonatomic, assign) int64_t mRequestTimestamp;
//响应回来的时间戳
@property (nonatomic, assign) int64_t mResponseTimestamp;
//消息类型
@property (nonatomic, assign) int32_t mCmdType;
//内容类型
@property (nonatomic, assign) int32_t mSubType;
//发送者
@property (nonatomic, strong) NSString* mFromID;
//接收者
@property (nonatomic, strong) NSString* mToID;
//消息主要内容
@property (nonatomic, strong) NSData*   mContent;
//消息的分组内容
@property (nonatomic, strong) NSString* mMsgType2;
//消息扩展内容1
@property (nonatomic, strong) NSData*   mExtend1;
//消息扩展内容2(推送内容)
@property (nonatomic, strong) NSData*   mExtend2;
//消息扩展内容3
@property (nonatomic, strong) NSData*   mExtend3;
//消息扩展内容4(二进制)
@property (nonatomic, strong) NSData*   mExtend4;
//消息扩展内容5
@property (nonatomic, strong) NSData*   mExtend5;
//消息扩展内容6
@property (nonatomic, strong) NSData*   mExtend6;
//推送标题
@property (nonatomic, strong) NSData*   mPushTitle;
//请求回调
@property (nonatomic, copy) LVIM_SEND_MESSAGE_CALLBACK mSendMessageCallback;
//消息发送状态
@property (nonatomic, assign) LVIM_MESSAGE_SEND_STATE send_state;


/**
 @brief 从json转化
 @param json 内容
 @return 失败返回nil
 **/
+(LVIMMessage*)fromJsonDictory:(NSDictionary*)json;

/**
 @brief 从pb消息转化
 @param msg pb消息(const IMMessage*)
 @return 失败返回nil
 **/
+(LVIMMessage*)fromProtobufMessage:(const void*)msg;

/**
 @brief 从本地数据库结构初始化
 @param data 数据
 @return 失败返回nil
 **/
+(LVIMMessage*)fromLocalDatabase:(void*)data;

/**
 @brief 检测必要属性是否无效
 **/
-(BOOL)invalid;

/**
 @brief 检测消息大小是否超标
 @return 超长返回非0, 未超长返回0
 **/
-(int)isInvalidSize;

/**
 @brief 检测发送请求的参数是否无效
 **/
-(int)isInvalidRequestParams;

/**
 @brief 是否是回执
 **/
-(BOOL)isAckMessage;

/**
 @brief 是否是直播间消息
 **/
-(BOOL)isChatroomMessage;

/**
 @brief 是否是私信消息
 **/
-(BOOL)isPrivateMessage;

/**
 @brief 是否是群组消息
 **/
-(BOOL)isGroupMessage;

/**
 @brief 是否是系统消息
 **/
-(BOOL)isSystemMessage;

/**
@brief 是否是事件消息
**/
-(BOOL)isEventMessage;

/**
 @brief 内容是否是图片(私信/群组)
 **/
-(BOOL)isTextMessageContent;

/**
 @brief 内容是否是图片(私信/群组)
 **/
-(BOOL)isImageMessageContent;

/**
 @brief 内容是否是音频(私信/群组)
 **/
-(BOOL)isAudioMessageContent;

/**
 @brief 内容是否是视频(私信/群组)
 **/
-(BOOL)isVideoMessageContent;

/**
 @brief 获取消息类型
 @return 失败返回nil
 **/
-(NSString*)getMessageStringType;

/**
 @brief 获取消息内容
 @return 失败返回nil
 **/
-(NSString*)getMessageStringContent;


//--------------------------------------------------------------------------------------------------------------

/**
 @brief 构建房间请求消息
 @param fid 发送者ID(用户ID)
 @param tid 接收者ID(房间ID)
 @param type 消息类型别名(类型唯一)
 @param content 消息内容
 @return 成功返回0, 否则返回非0
 **/
-(int)buildChatroomRequest:(NSString*)fid tid:(NSString*)tid type:(NSString*)type content:(NSString*)content;

/**
 @brief 构建私信请求
 @param subType 内容类型(见LVIM_MESSAGE_SUBTYPE)
 @param fid 发送者ID(用户ID)
 @param tid 接收者ID(用户ID)
 @param type 消息类型别名(类型唯一)
 @param content 消息内容
 @param pushTitle 推送标题
 @param pushContent 推送内容
 @param extend3 扩展字段(字符串:无特殊情况不要使用)
 @param extend4 扩展字段(二进制:无特殊情况不要使用)
 @param targetAppID 扩展字段5(目标应用ID)
 @param targetAppUID 扩展字段6(目标应用UID)
 @return 成功返回0, 否则返回非0
 **/
-(int)buildPrivateRequest:(int)subType fid:(NSString*)fid tid:(NSString*)tid type:(NSString*)type content:(NSString*)content
                pushTitle:(NSString*)pushTitle pushContent:(NSString*)pushContent extend3:(NSString*)extend3 extend4:(NSData*)extend4
                targetAppID:(NSString*)targetAppID targetAppUID:(NSString*)targetAppUID;

/**
 @brief 构建事件请求
 @param fid 发送者ID(用户ID)
 @param tid 接收者ID(用户ID)
 @param type 消息类型别名(类型唯一)
 @param content 消息内容
 @param pushTitle 推送标题
 @param pushContent 推送内容
 @param targetAppID 扩展字段5(目标应用ID)
 @param targetAppUID 扩展字段6(目标应用UID)
 @return 成功返回0, 否则返回非0
 **/
-(int)buildEventRequest:(NSString*)fid tid:(NSString*)tid type:(NSString*)type content:(NSString*)content
                pushTitle:(NSString*)pushTitle pushContent:(NSString*)pushContent
                targetAppID:(NSString*)targetAppID targetAppUID:(NSString*)targetAppUID;

/**
 @brief 构建私信请求
 @param subType 内容类型(见LVIM_MESSAGE_SUBTYPE)
 @param cmdType 消息类型(见CMIM_CMD_TYPE)
 @param fid 发送者ID(用户ID)
 @param tid 接收者ID(用户ID)
 @param type 消息类型别名(类型唯一)
 @param content 消息内容
 @param pushTitle 推送标题
 @param pushContent 推送内容
 @param extend3 扩展字段(字符串:无特殊情况不要使用)
 @param extend4 扩展字段(二进制:无特殊情况不要使用)
 @param targetAppID 扩展字段5(目标应用ID)
 @param targetAppUID 扩展字段6(目标应用UID)
 @return 成功返回0, 否则返回非0
 **/
-(int)buildPrivateRequest:(int)subType cmdType:(int)cmdType fid:(NSString*)fid tid:(NSString*)tid type:(NSString*)type content:(NSString*)content
                pushTitle:(NSString*)pushTitle pushContent:(NSString*)pushContent extend3:(NSString*)extend3 extend4:(NSData*)extend4
                targetAppID:(NSString*)targetAppID targetAppUID:(NSString*)targetAppUID;

/**
 @brief 构建文本私信请求
 @param fid 发送者ID(用户ID)
 @param tid 接收者ID(用户ID)
 @param type 消息类型别名(类型唯一)
 @param content 消息内容
 @param pushTitle 推送标题
 @param pushContent 推送内容
 @param extend3 扩展字段(字符串:无特殊情况不要使用)
 @param extend4 扩展字段(二进制:无特殊情况不要使用)
 @param targetAppID 扩展字段5(目标应用ID)
 @param targetAppUID 扩展字段6(目标应用UID)
 @return 成功返回0, 否则返回非0
 **/
-(int)buildTextPrivateRequest:(NSString*)fid tid:(NSString*)tid type:(NSString*)type content:(NSString*)content
                pushTitle:(NSString*)pushTitle pushContent:(NSString*)pushContent extend3:(NSString*)extend3 extend4:(NSData*)extend4
                targetAppID:(NSString*)targetAppID targetAppUID:(NSString*)targetAppUID;

/**
 @brief 构建图片私信请求
 @param fid 发送者ID(用户ID)
 @param tid 接收者ID(用户ID)
 @param type 消息类型别名(类型唯一)
 @param content 消息内容
 @param pushTitle 推送标题
 @param pushContent 推送内容
 @param extend3 扩展字段(字符串:无特殊情况不要使用)
 @param extend4 扩展字段(二进制:无特殊情况不要使用)
 @param targetAppID 扩展字段5(目标应用ID)
 @param targetAppUID 扩展字段6(目标应用UID)
 @return 成功返回0, 否则返回非0
 **/
-(int)buildImagePrivateRequest:(NSString*)fid tid:(NSString*)tid type:(NSString*)type content:(NSString*)content
                pushTitle:(NSString*)pushTitle pushContent:(NSString*)pushContent extend3:(NSString*)extend3 extend4:(NSData*)extend4
                targetAppID:(NSString*)targetAppID targetAppUID:(NSString*)targetAppUID;

/**
 @brief 构建音频私信请求
 @param fid 发送者ID(用户ID)
 @param tid 接收者ID(用户ID)
 @param type 消息类型别名(类型唯一)
 @param content 消息内容
 @param pushTitle 推送标题
 @param pushContent 推送内容
 @param extend3 扩展字段(字符串:无特殊情况不要使用)
 @param extend4 扩展字段(二进制:无特殊情况不要使用)
 @param targetAppID 扩展字段5(目标应用ID)
 @param targetAppUID 扩展字段6(目标应用UID)
 @return 成功返回0, 否则返回非0
 **/
-(int)buildAudioPrivateRequest:(NSString*)fid tid:(NSString*)tid type:(NSString*)type content:(NSString*)content
                pushTitle:(NSString*)pushTitle pushContent:(NSString*)pushContent extend3:(NSString*)extend3 extend4:(NSData*)extend4
                targetAppID:(NSString*)targetAppID targetAppUID:(NSString*)targetAppUID;

/**
 @brief 构建视频私信请求

 @param fid 发送者ID(用户ID)
 @param tid 接收者ID(用户ID)
 @param type 消息类型别名(类型唯一)
 @param content 消息内容
 @param pushTitle 推送标题
 @param pushContent 推送内容
 @param extend3 扩展字段(字符串:无特殊情况不要使用)
 @param extend4 扩展字段(二进制:无特殊情况不要使用)
 @param targetAppID 扩展字段5(目标应用ID)
 @param targetAppUID 扩展字段6(目标应用UID)
 @return 成功返回0, 否则返回非0
 **/
-(int)buildVideoPrivateRequest:(NSString*)fid tid:(NSString*)tid type:(NSString*)type content:(NSString*)content
                pushTitle:(NSString*)pushTitle pushContent:(NSString*)pushContent extend3:(NSString*)extend3 extend4:(NSData*)extend4
                targetAppID:(NSString*)targetAppID targetAppUID:(NSString*)targetAppUID;

/**
 @brief 构建私信请求
 @param subType 内容类型(见LVIM_MESSAGE_SUBTYPE)
 @param fid 发送者ID(用户ID)
 @param tid 接收者ID(用户ID)
 @param type 消息类型别名(类型唯一)
 @param content 消息内容
 @param pushTitle 推送标题
 @param pushContent 推送内容
 @param extend3 扩展字段(字符串:无特殊情况不要使用)
 @param extend4 扩展字段(二进制:无特殊情况不要使用)
 @return 成功返回0, 否则返回非0
 **/
-(int)buildGroupRequest:(int)subType fid:(NSString*)fid tid:(NSString*)tid type:(NSString*)type content:(NSString*)content
              pushTitle:(NSString*)pushTitle pushContent:(NSString*)pushContent extend3:(NSString*)extend3 extend4:(NSData*)extend4;

//--------------------------------------------------------------------------------------------------------------

/**
 @brief 构建一条直播间聊天消息
 @param fromid 发送者ID
 @param toid 接收者ID(房间ID)
 @param uname 发送者名称
 @param content 聊天内容
 @return 失败返回nil
 **/
+(LVIMMessage*)buildChatRoomTextMessage:(NSString*)fromid toid:(NSString*)toid uname:(NSString*)uname content:(NSString*)content;

@end
