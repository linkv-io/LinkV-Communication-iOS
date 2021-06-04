//
//  LVIMConfig.h
//  LVIM
//
//  Created by pangbingxin on 2018/7/31.
//  Copyright © 2018年 cmcm. All rights reserved.
//

#import <Foundation/Foundation.h>
@class LVIMMessage;

//定时刷新消息的代理
@protocol LVIMReceiveMessageFlushDelegate <NSObject>
@optional
/**
 @brief 刷新处理
 @param waitings 队列中等待处理的消息数量
 **/
-(void)onIMReceiveMessageFlush:(int)waitings;
@end

//返回消息处理代理
@protocol LVIMReceiveMessageDelegate <NSObject>
@required
/**
 @brief 消息返回处理
 @param owner 这个消息属于哪个登陆用户
 @param immsg 消息内容
 @param waitings 队列中等待处理的消息数量
 @param packetSize 当前数据包大小
 @param waitLength 等待处理的大小
 @param bufferSize 缓冲区最大大小
 @return 处理成功返回0, 否则返回非0
 @warning 如果返回非0, 则这条消息会在下次冷启动或再次检测到时继续调用本方法
 **/
-(int)onIMReceiveMessageHandler:(NSString*)owner
                          immsg:(LVIMMessage*)immsg
                       waitings:(int)waitings
                     packetSize:(int)packetSize
                     waitLength:(int)waitLength
                     bufferSize:(int)bufferSize;
@optional
/**
 @brief 消息过滤器
 @param diytype 消息类型的CRC32
 @param fromid 发送者ID
 @param toid 接收者ID
 @param msgtype 类型别名
 @param content 消息内容
 @param waitings 队列中等待处理的消息数量
 @param packetSize 当前数据包大小
 @param waitLength 等待处理的大小
 @param bufferSize 缓冲区最大大小
 @return 过滤丢弃掉这个消息返回YES, 继续处理返回NO
 **/
-(BOOL)onIMReceiveMessageFilter:(int32_t)diytype
                           fromid:(const char*)fromid
                             toid:(const char*)toid
                          msgtype:(const char*)msgtype
                          content:(const char*)content
                         waitings:(int)waitings
                       packetSize:(int)packetSize
                       waitLength:(int)waitLength
                       bufferSize:(int)bufferSize;
@end

//信息统计回报代理
@protocol LVIMReportDelegate <NSObject>
@optional
/**
 @brief 回报连接状态
 @param state 状态信息
 **/
-(void)onIMReportConnectState:(void*)state;

/**
 @brief 回报消息状态
 @param state 状态信息
 **/
-(void)onIMReportMessageState:(void*)state;

@end



//事件代理
@protocol LVIMModuleEventDelegate <NSObject>
@required
/**
 @brief 请求token的事件
 **/
-(void)onQueryIMToken;
@optional
/**
 @brief 连接成功事件(暂不提供)
 @param context 状态信息
 **/
-(void)onIMConnected:(void*)context;

/**
 @brief 连接断开事件(暂不提供)
 **/
-(void)onIMLosted;

/**
 @brief 认证成功事件
 @param uid 用户ID
 @param token token (IM)
 @param unReadMsgSize 未读消息数量(粗略)
 **/
-(void)onIMAuthSuccessed:(NSString*)uid token:(NSString*)token unReadMsgSize:(int)unReadMsgSize;

/**
 @brief 认证失败事件
 @param uid 用户ID
 @param token 认证码(im)
 @param ecode 错误码
 @param rcode 响应码
 @param expired 是否是过期了
 **/
-(void)onIMAuthFailed:(NSString*)uid token:(NSString*)token ecode:(int)ecode rcode:(int)rcode expired:(BOOL)expired;

/**
 @brief 认证过期
 @param uid 用户ID
 @param token 认证码(im)
 **/
-(void)onIMTokenExpired:(NSString*)uid token:(NSString*)token;

@end

//上传文件的代理
@protocol LVIMUploadLogFilterDelegate <NSObject>
@optional
/**
 @brief 上传日志文件夹的代理
 @param content 携带参数
 @return 返回需要上传的文件夹路径列表
 **/
-(NSDictionary*)onIMUploadLogFilterDirectory:(NSString*)content;
/**
 @brief 上传日志文件的代理
 @param content 携带参数
 @return 返回需要上传的文件路径列表
 **/
-(NSDictionary*)onIMUploadLogFilterFile:(NSString*)content;
@end



//媒体文件类型
enum LVIM_MEDIA_FILE_TYPE {
    //无效最小值
    LVIM_MEDIA_FILE_TYPE_MIN = 0,
    //图片文件(私信)
    LVIM_MEDIA_FILE_PRIVATE_IMAGE,
    //语音文件(私信)
    LVIM_MEDIA_FILE_PRIVATE_AUDIO,
    //视频文件(私信)
    LVIM_MEDIA_FILE_PRIVATE_VIDEO,
    //图片文件(群组)
    LVIM_MEDIA_FILE_GROUP_IMAGE,
    //语音文件(群组)
    LVIM_MEDIA_FILE_GROUP_AUDIO,
    //视频文件(群组)
    LVIM_MEDIA_FILE_GROUP_VIDEO,
    //无效最大值
    LVIM_MEDIA_FILE_TYPE_MAX
};

//对应媒体文件的文件夹名称
extern NSString* g_lvim_media_file_directory_names[LVIM_MEDIA_FILE_TYPE_MAX];

//媒体配置信息
struct lvim_media_config {
    //图片文件最大字节数限制(最大1024KB)
    int     image_fsize_max;
    //图片最高质量(最小30,默认70,最大100)
    int     image_quality_max;
    //图片最小宽高度限制(默认4)
    int     image_size_min;
    //图片最大宽度限制(默认1024,超过会被裁减)
    int     image_size_max;
    
    //缩略图最高质量(最小30,默认70,最大100)
    int     thumb_quality_max;
    //缩略图最小宽高度限制(默认4)
    int     thumb_size_min;
    //缩略图最大宽度限制(默认64,超过会被裁减)
    int     thumb_size_max;
    
    //音频需要走上传的大小(低于这个值走im,高于这个值走上传,默认5KB)
    int     audio_size_upload_limit;
    //音频需要走上传的秒数(低于这个值走im,高于这个值走上传,默认5秒)
    int     audio_duration_upload_limit;
};
typedef struct lvim_media_config lvim_media_config;

@interface LVIMConfig : NSObject<NSCopying>
//----------------------------------------------------------------------
//应用ID
@property (nonatomic, copy) NSString* mAppID;
//应用KEY
@property (nonatomic, copy) NSString* mAppKey;
//应用AID
@property (nonatomic, copy) NSString* mAppXAID;
//应用的设备ID
@property (nonatomic, copy) NSString* mAppDeviceID;
//----------------------------------------------------------------------
//应用的用户ID(初始化可空)
@property (nonatomic, copy) NSString* mAppUserID;
//应用的用户国家码(初始化可空)
@property (nonatomic, copy) NSString* mAppUserCountry;
//----------------------------------------------------------------------
//获取IM的配置地址(正式)
@property (nonatomic, copy) NSString* mIMQueryConfigURL;
//获取IM的配置地址(测试)
@property (nonatomic, copy) NSString* mIMQueryConfigURL_DEBUG;
//----------------------------------------------------------------------
//获取IM的未读消息地址(正式)
@property (nonatomic, copy) NSString* mIMQueryUnReadMessageURL;
//获取IM的未读消息地址(测试)
@property (nonatomic, copy) NSString* mIMQueryUnReadMessageURL_DEBUG;
//----------------------------------------------------------------------
//获取IM的历史消息地址(正式)
@property (nonatomic, copy) NSString* mIMQuerySessionMessageURL;
//获取IM的历史消息地址(测试)
@property (nonatomic, copy) NSString* mIMQuerySessionMessageURL_DEBUG;
//----------------------------------------------------------------------
//获取IM的会话列表地址(正式)
@property (nonatomic, copy) NSString* mIMQuerySessionListURL;
//获取IM的会话列表地址(测试)
@property (nonatomic, copy) NSString* mIMQuerySessionListURL_DEBUG;
//----------------------------------------------------------------------
//获取上传IM的文件的TOKEN地址(正式)
@property (nonatomic, copy) NSString* mIMQueryUploadTokenURL;
//获取上传IM的文件的TOKEN地址(测试)
@property (nonatomic, copy) NSString* mIMQueryUploadTokenURL_DEBUG;
//----------------------------------------------------------------------
//IM的设备ID
@property (nonatomic, copy) NSString* mIMDeviceID;
//APNS推送TOKEN
@property (nonatomic, copy) NSString* mPushToken;
//----------------------------------------------------------------------
//私信媒体文件的配置信息
@property (nonatomic, assign) lvim_media_config mIMPrivateMediaConfig;
//群组媒体文件的配置信息
@property (nonatomic, assign) lvim_media_config mIMGroupMediaConfig;
//----------------------------------------------------------------------
//是否是调试模式(默认为关)
@property (nonatomic, assign) BOOL mIsDebugMode;
//是否显示日志(默认显示)
@property (nonatomic, assign) BOOL mIsLogVisible;
//是否启用派发线程(默认启用)
@property (nonatomic, assign) BOOL mIsDispatchThreadEnable;
//是否启用房间(默认直播间为开)
@property (nonatomic, assign) BOOL mIsChatroomEnabled;
//是否启用私信(默认私信为关)
@property (nonatomic, assign) BOOL mIsPrivateEnabled;
//是否启用群组(默认群组为关)
@property (nonatomic, assign) BOOL mIsGroupEnabled;
//----------------------------------------------------------------------

/**
 @brief 检测内容是否无效
 @return 无效返回非0, 有效返回0
 **/
-(int)invalid;

/**
 @brief 判断是否登录成功
 **/
-(BOOL)isAppUserLoginSuccessed;

@end
