//
//  LVIMSDK.h
//  LVIM
//
//  Created by pangbingxin on 2018/7/31.
//  Copyright © 2018年 cmcm. All rights reserved.
//

#import "LVIMConfig.h"
#import "LVIMRoomState.h"
#import "LVIMMessage.h"
#import "LVIMSession.h"

/**
 @brief 拉取会话列表的回调
 @param ecode 错误码(无错误为0)
 @param rcode 响应码(成功为200)
 @param rstatus 状态码(成功为200)
 @param sessions 会话列表
 **/
typedef void(^LVIM_QUERY_REMOTE_SESSION_LIST_CALLBACK)(int32_t ecode, int32_t rcode, int32_t rstatus, NSArray* sessions);

/**
 @brief 拉取历史消息的回调
 @param ecode 错误码(无错误为0)
 @param rcode 响应码(成功为200)
 @param rstatus 状态码(成功为200)
 @param messages 聊天记录
 **/
typedef void(^LVIM_QUERY_REMOTE_SESSION_MESSAGE_CALLBACK)(int32_t ecode, int32_t rcode, int32_t rstatus, NSArray* messages);


//----------------------------------------------------------------------------------------------------

#define LVIM_WRITE_DEBUG_LOG(__format, ...) \
    lvim_write_debug_log([NSString stringWithFormat:__format, ##__VA_ARGS__])
FOUNDATION_EXPORT void lvim_write_debug_log(NSString* msg);

#define LVIM_WRITE_WARNING_LOG(__format, ...) \
    lvim_write_warning_log([NSString stringWithFormat:__format, ##__VA_ARGS__])
FOUNDATION_EXPORT void lvim_write_warning_log(NSString* msg);

#define LVIM_WRITE_ERROR_LOG(__format, ...) \
    lvim_write_error_log([NSString stringWithFormat:__format, ##__VA_ARGS__])
FOUNDATION_EXPORT void lvim_write_error_log(NSString* msg);

//----------------------------------------------------------------------------------------------------

//开启缩略图BASE64传输模式
//#define LVIM_IMAGE_THUMB_EXTRA_BASE64_ENABLE

//----------------------------------------------------------------------------------------------------
//是否开启自研im模块(关了它就全失效)
#define LVIM_MODULE_ENABLE

//是否开启自研im的私信(关了它私信全部走融云)
#define LVIM_PRIVATE_MODULE_ENABLE

//是否开启自颜im的群组(关了它群组全部走融云)
#define LVIM_GROUP_MODULE_ENABLE

//是否使用魔方控制(打开后使用魔方控制,关闭后强制开启)
#define LVIM_MODULE_USING_MOFANG

//是否使用魔方来控制日志打印(如果关掉它,强制打印日志)
#define LVIM_LOG_USING_MOFANG

//是否使用私信魔方控制(打开后使用魔方控制,关闭后强制开启)
#define LVIM_PRIVATE_USING_MOFANG

//是否使用群组魔方控制(打开后使用魔方控制,关闭后强制开启)
#define LVIM_GROUP_USING_MOFANG

//----------------------------------------------------------------------------------------------------

//检测房间是否已启用
#ifndef IS_LVIM_CHATROOM_ENABLE
#define IS_LVIM_CHATROOM_ENABLE() \
    [[LVIMSDK sharedInstance] getConfig].mIsChatroomEnabled
#endif //IS_LVIM_CHATROOM_ENABLE

//检测群组是否已启用
#ifndef IS_LVIM_GROUP_ENABLED
#define IS_LVIM_GROUP_ENABLED() \
    [[LVIMSDK sharedInstance] getConfig].mIsGroupEnabled
#endif //IS_LVIM_GROUP_ENABLED

//检测私信是否已启用
#ifndef IS_LVIM_PRIVATE_ENABLED
#define IS_LVIM_PRIVATE_ENABLED() \
    [[LVIMSDK sharedInstance] getConfig].mIsPrivateEnabled
#endif //IS_LVIM_PRIVATE_ENABLED

//检测是否是发布模式
#ifndef IS_LVIM_RELEASE_MODE
#define IS_LVIM_RELEASE_MODE() \
    ![[LVIMSDK sharedInstance] getConfig].mIsDebugMode
#endif //IS_LVIM_RELEASE_MODE

//----------------------------------------------------------------------------------------------------
//获取配置信息
#define GET_LVIM_SDK_CONFIG() \
    [[LVIMSDK sharedInstance] getConfig]
//获取媒体文件随机路径
#define GET_LVIM_MEDIA_CACHE_FILE_NAME(__fremote, __mftype, __fid, __tid, __fsuffix) \
    [[LVIMSDK sharedInstance] getCacheMediaFileName:__fremote mftype:__mftype fid:__fid tid:__tid fsuffix:__fsuffix];
//----------------------------------------------------------------------------------------------------


@interface LVIMSDK : NSObject

/**
 @brief 获取LVIM模块的实例
 **/
+(instancetype)sharedInstance;

//----------------------------------------------------------------------------------------------------

/**
 @brief 获取UUID
 **/
+(NSString*)getUUID;

/**
 @brief 获取当前的配置信息
 @return 未设置返回nil
 **/
-(LVIMConfig*)getConfig;

/**
 @brief 初始化
 @param appId 开放平台申请的appId
 @param secret 开放平台申请的secret
 **/
-(BOOL)initWithAppId:(NSString *)appId secret:(NSString *)secret;

/**
 @brief 设置LVIM的认证码
 @param uid 用户ID
 @param token 认证码(lvim)
 **/
-(void)setIMToken:(NSString*)uid token:(NSString*)token;

/**
 @brief 设置apns的推送token
 @param token 推送APNS的TOKEN
 **/
-(void)setPushToken:(NSString*)token;

/**
 @brief 设置直播间返回消息的处理代理(可根据需要设置)
 @return 成功返回0, 否则返回非0
 **/
-(int)setChatroomReceiveMessageDelegate:(id<LVIMReceiveMessageDelegate>)delegate;

/**
 @brief 设置全局返回消息的处理代理(必须设置)
 @return 成功返回0, 否则返回非0
 **/
-(int)setGlobalReceiveMessageDelegate:(id<LVIMReceiveMessageDelegate>)delegate;

/**
 @brief 设置消息刷新代理(可根据需要设置)
 @param interval 刷新间隔毫秒数
 @return 成功返回0, 否则返回非0
 **/
-(int)setMessageFlushDelegate:(id<LVIMReceiveMessageFlushDelegate>)delegate interval:(int32_t)interval;

/**
 @brief 设置模块事件代理(可根据需要设置)
 @return 成功返回0, 否则返回非0
 **/
-(int)setModuleEventDelegate:(id<LVIMModuleEventDelegate>)delegate;

/**
 @brief 设置信息回报代理(可根据需要设置)
 @return 成功返回0, 否则返回非0
 **/
-(int)setReportDelegate:(id<LVIMReportDelegate>)delegate;

/**
 @brief 设置上传日志的代理(可根据需要设置)
 @return 成功返回0, 否则返回非0
 **/
-(int)setUploadLogFilterDelegate:(id<LVIMUploadLogFilterDelegate>)delegate;

/**
 @brief 设置设备id
 @param appDeviceID 设备id
 **/
-(int)setAppDeviceID:(NSString *)appDeviceID;

/**
 @brief 设置appXAID
 @param appXAID appXAID
 **/
-(int)setAppXAID:(NSString *)appXAID;

/**
 @brief 设置调试模式
 @param isEnable 是否开启调试模式
 **/
-(int)setDebugMode:(BOOL)isEnable;

/**
 @brief 设置日志可见状态
 @param isVisible 日志是否可见
 **/
-(int)setLogVisible:(BOOL)isVisible;

/**
 @brief 设置房间功能是否启用
 **/
-(int)setChatroomEnable:(BOOL)isEnable;

/**
 @brief 设置私信功能是否启用
 **/
-(int)setPrivateEnable:(BOOL)isEnable;

/**
 @brief 设置群组功能是否启用
 **/
-(int)setGroupEnable:(BOOL)isEnable;

//----------------------------------------------------------------------------------------------------

/**
 @brief 检测模块是否已经启动
 **/
-(BOOL)isStarted;

/**
 @brief 检测连接是否已经建立
 **/
-(BOOL)isConnected;

/**
 @brief 检测认证是否完成
 **/
-(BOOL)isAuthed;

/**
 @brief 检测是否登录成功
 **/
-(BOOL)isAppUserLoginSucceed;
//----------------------------------------------------------------------------------------------------

/**
 @brief 开始运行(调用之前必须正确完成配置设置)
 @return 成功返回0, 否则返回非0
 **/
-(int)start;

/**
 @brief 停止运行
 @param timeout 等待停止运行的最大超时毫秒数
 @return 成功停止返回1, 等待超时返回0, 否则返回<0的值
 **/
-(int)stop:(int)timeout;

/**
 @brief 用户登录
 @param uid 用户ID
 @param country 用户的国家代码
 @return 成功返回0, 否则返回非0
 **/
-(int)login:(NSString*)uid country:(NSString*)country;

/**
 @brief 用户登录
 @param uid 用户ID
 **/
-(int)login:(NSString*)uid;

/**
 @brief 用户登出
 @param waitFlushTimeout 等待数据同步超时毫秒数(小于等于0时不等待,默认1000)
 @param waitFinishTimeout 等待完成超时毫秒数(小于等于0时不等待,默认1000)
 @return 成功返回0, 否则返回非0
 **/
-(int)logout:(int)waitFlushTimeout waitFinishTimeout:(int)waitFinishTimeout;

/**
 @brief 用户登出
 @return 成功返回0, 否则返回非0
 **/
-(int)logout;

/**
 @brief 获取缓存的日志路径
 **/
-(NSString*)getCacheLogPath;

/**
 @brief 获取缓存的数据库路径
 **/
-(NSString*)getCacheDatabasePath;

/**
 @brief 获取随机缓存的媒体文件名称
 @param fremote 是否是远程下载的文件
 @param mftype 媒体类型(LVIM_MEDIA_FILE_TYPE)
 @param fid 发送者ID
 @param tid 接收者ID
 @param fsuffix 文件后缀
 **/
-(NSString*)getCacheMediaFileName:(BOOL)fremote mftype:(int)mftype fid:(NSString*)fid tid:(NSString*)tid fsuffix:(NSString*)fsuffix;

/**
 @brief 发送消息
 @param msg 消息内容
 @param context 传入接收代理对象(可空)
 @param callback 结果回调(可空)
 @return 执行成功返回0, 否则返回非0
 **/
-(int)sendMessage:(LVIMMessage*)msg
          context:(id)context
         callback:(LVIM_SEND_MESSAGE_CALLBACK)callback;

/**
 @brief 创建房间
 @param rid 房间ID
 @param context 传入接收代理对象(可空)
 @param callback 结果回调(可空)
 @return 执行成功返回0, 否则返回非0
 **/
-(int)createChatroom:(NSString*)rid
             context:(id)context
            callback:(LVIM_SEND_MESSAGE_CALLBACK)callback;

/**
 @brief 加入房间
 @param rid 房间ID
 @param context 传入接收代理对象(可空)
 @param callback 结果回调(可空)
 @return 执行成功返回0, 否则返回非0
 **/
-(int)joinChatroom:(NSString*)rid
           context:(id)context
          callback:(LVIM_SEND_MESSAGE_CALLBACK)callback;

/**
 @brief 离开房间
 @param rid 房间ID
 @param context 传入接收代理对象(可空)
 @param callback 结果回调(可空)
 @return 执行成功返回0, 否则返回非0
 **/
-(int)leaveChatroom:(NSString*)rid
            context:(id)context
           callback:(LVIM_SEND_MESSAGE_CALLBACK)callback;

/**
 @brief 获取当前房间的状态
 **/
-(LVIMRoomState*)getCurrentRoomState;

/**
 @brief 拉取远程历史会话列表
 @param pageIndex 页面编号
 @param pageSize 页面长度
 @param callback 结果回调
 @return 成功返回0, 否则返回非0
 **/
-(int)queryRemoteSessionList:(int)pageIndex pageSize:(int)pageSize callback:(LVIM_QUERY_REMOTE_SESSION_LIST_CALLBACK)callback;

/**
 @brief 拉取远程某个会话的历史消息
 @param tuid 目标会话ID
 @param smsgid 本地最后一条消息的服务端消息ID
 @param sequence 本地最后一条数据的服务端消息序列
 @param size 要获取的条数
 @param callback 结果回调
 @return 成功返回0, 否则返回非0
 **/
//-(int)queryRemoteSessionMessage:(NSString*)tuid smsgid:(int64_t)smsgid sequence:(int64_t)sequence size:(int)size callback:(LVIM_QUERY_REMOTE_SESSION_MESSAGE_CALLBACK)callback;
- (int)queryRemoteSessionMessage:(NSString *)tuid sequence:(int64_t)sequence size:(int)size callback:(LVIM_QUERY_REMOTE_SESSION_MESSAGE_CALLBACK)callback;

/**
 @brief 删除本地私信历史消息记录
 @param sessionid 某个人的UID
 @return 成功返回删除条数, 失败返回<0的值
 **/
-(int)deleteLocalPrivateHistoryMessage:(NSString*)sessionid;

/**
 @brief 删除本地私信历史消息记录
 @param sessionid 某个群组的ID
 @return 成功返回删除条数, 失败返回<0的值
 **/
-(int)deleteLocalGroupHistoryMessage:(NSString*)sessionid;

/**
@brief 把某个会话中所有未读消息标记为已读
@param targetId 目标的id
@return 成功返回处理条数, 否则返回<0的值
*/
-(int)clearPrivateSessionUnreadMsg:(NSString *)targetId;

/**
@brief 把所有私信未读消息标记为已读
@return 成功返回处理条数, 否则返回<0的值
*/
-(int)clearPrivateAllUnreadMsg;

/**
@brief 把某个会话中小于等于stime的所有未读消息标记为已读
@param targetId 目标的id
@param stime 消息的服务端时间戳
@return 成功返回处理条数, 否则返回<0的值
*/
-(int)updatePrivateMsgAsReadByStime:(NSString *)targetId
                       stime:(int64_t)stime;

/**
 @brief 查询本地私信消息历史记录
 @param sessionid 某个人的UID
 @param dbid 参考数据id
 @param limit 限制结果条数
 @param desc 是否降序查询
 @param rmsgs 结果集
 @return 成功返回结果数量, 失败返回<0的值
 **/
-(int)queryLocalPrivateHistoryMessage:(NSString*)sessionid dbid:(int64_t)dbid limit:(int32_t)limit desc:(BOOL)desc rmsgs:(NSMutableArray<LVIMMessage *> *)rmsgs;

/**
@brief 获取消息会话列表
@param arrM 结果集
@return 成功返回处理条数, 否则返回<0的值
*/
-(int)querySessionList:(NSMutableArray<LVIMSession *> *)arrM;

/**
 @brief 获取所有未读消息的数量
 @return 成功返回未读消息数量, 否则返回<0的值
 */
-(int)queryUnReadCount;

/**
 @brief 查询本地群组消息历史记录
 @param sessionid 某个群组的ID
 @param dbid 参考数据id
 @param limit 限制结果条数
 @param desc 是否降序查询
 @param rmsgs 结果集
 @return 成功返回结果数量, 失败返回<0的值
 **/
-(int)queryLocalGroupHistoryMessage:(NSString*)sessionid dbid:(int64_t)dbid limit:(int32_t)limit desc:(BOOL)desc rmsgs:(NSMutableArray*)rmsgs;

/**
 @brief 是否启用了http消息
 @return 未启用返回0, 独立连接启用返回1, 主连接启用返回2
 **/
-(int)isHttpMessageEnabled;

/**
 @brief 发送http业务消息
 @param path url请求地址后面的路径
 @param method 请求方法(GET/POST/PUT...)
 @param content 请求内容(可空)
 @param headers 附加的请求头部(可空)
 @param sendtmo 发送超时时间(1000毫秒 ~ 60000毫秒)
 @param recvtmo 接收超时时间(1000毫秒 ~ 60000毫秒)
 @param context 自定义上下文(可空)
 @param callback 自定义回调(可空)
 @return 成功返回0, 否则返回非0
 **/
-(int)sendHttpMessage:(NSString*)path
               method:(NSString*)method
              content:(NSString*)content
              headers:(NSString*)headers
              sendtmo:(int)sendtmo
              recvtmo:(int)recvtmo
              context:(id)context
             callback:(LVIM_SEND_HTTP_MESSAGE_CALLBACK)callback;
//error code 返回0表正常 1表示未登录
- (void)requestDebugToken:(void (^)(NSInteger errorCode, NSDictionary *responseDict))callback;

/**
@brief 设置私信消息的最大存储量
@param storageMax 消息最大存储量
*/
- (int)setPrivateDBStorageMax:(int64_t)storageMax;

/**
@brief 设置消息的最大存储量
@param dbtype 数据库类型
@param storageMax 消息最大存储量
*/
-(int)setDBStorageMax:(int)dbtype storageMax:(int64_t)storageMax;

/**
@brief 往数据库中插入某条消息
@param dbtype 数据库类型
@param msg        消息模型，必须有消息id，mLVsgID必须大于0
*/
- (int64_t)insertSendedMessageToDB:(int)dbtype message:(LVIMMessage *)msg;

@end
