//
//  LVCEngine.h
//  LinkV_Zego_iOS
//
//  Created by Wing on 2020/8/5.
//

#import <Foundation/Foundation.h>
#if __has_include(<LinkV_RTC/LinkVRTCEngine.h>)
#import <LinkV_RTC/LinkVRTCEngine.h>
#endif

#if __has_include("LinkVRTCEngine.h")
#import "LinkVRTCEngine.h"
#endif
#import <LVIMLib/LVIMLib.h>

static int ERROR_BUILD_MSG = 1001; // 构建消息失败

@protocol RoomCallbackProtocl<RTCCallbackDelegate>

/// 收到房间消息
/// 如果返回非0, 则这条消息会在下次冷启动或再次检测到时继续调用本方法
- (int)onRoomMessageReceive:(LVIMMessage *)msg;

@end

@interface LVCEngine : NSObject

+ (instancetype)shared;

/// 创建引擎
/// @param appId appId
/// @param appKey appkey
/// @param completion 回调
+ (instancetype)createEngineWithAppId:(NSString *)appId
                   appKey:(NSString *)appKey
                  completion:(LVCodeCompletion)completion;



@property (nonatomic, copy) NSString *uid;

- (void)setLogOpen:(BOOL)isOpen;

#pragma mark - RTC

/// 登录房间
/// @param uid 用户 uid
/// @param roomId 房间 id
/// @param isHost 是否主播
- (void)loginRoom:(NSString *)uid roomId:(NSString *)roomId isHost:(BOOL)isHost delegate:(id<RoomCallbackProtocl>)delegate;

/// 退出房间
- (void)logoutRoom;

/// 推流参数配置
- (void)setVideoConfig:(VideoConfigLevel)configLevel;

/// 开始采集视频流
- (void)startCapture;

/// 停止采集视频流
- (void)stopCapture;

/// 渲染视图
- (UIView *)startPreview:(UIView *)view;

/// 发布音视频数据（推流）
- (void)startPublishing;

/// 播放流
/// @param uid 用户 id
/// @param view 指定 view
- (UIView *)startPlayingStream:(NSString *)uid inView:(UIView *)view;

/// /// 停止播放流
/// @param uid 用户 id
- (void)stopPlayingStream:(NSString *)uid;

/// 转换前后置摄像头
/// @param isFront 是否前置
- (void)useFrontCamera:(BOOL)isFront;

/// 获取连麦SDK的名称
- (NSString *)getSolutionName;

/// 获取支持连麦数
- (int)getSupportLine;

/// 获取SDK类型 3为自研，4为zego
- (SDKType)getSDKType;

/// 设置是否关闭麦克风
/// @param isMute 是否开启关闭
- (void)enableMic:(BOOL)isMute;

/// 释放引擎
- (void)unInitSDK;

/// 获取麦克风音量
- (int)getMicVolume;

/// 设置是否允许 SDK 自动根据设备的方向调整视频的输出方向
/// @param enable 打开和关闭（默认是 YES）
- (void)enableVideoAutoRotation:(BOOL)enable;

#pragma mark - 美颜设置

/**
 * 设置美颜滤镜级别
 * @param beautyLevel 滤镜美颜级别 取值范围[0,1]，设置-1关闭
 */
- (void)setBeautyLevel:(CGFloat)beautyLevel;
/**
 * 设置滤镜明亮度
 * @param brightLevel 滤镜明亮度 取值范围[0,1]，设置-1关闭
 */
- (void)setBrightLevel:(CGFloat)brightLevel;
/**
 * 设置滤镜饱和度
 * @param toneLevel 滤镜饱和度 取值范围[0,1]，设置-1关闭
 */
- (void)setToneLevel:(CGFloat)toneLevel;

#pragma mark - IM

// 登录IM
- (int)loginIM:(NSString *)uid delegate:(id<LVIMModuleEventDelegate>)delegate;

/**
 @brief 设置LVIM的认证码
 @param uid 用户ID
 @param token 认证码(lvim)
 **/
-(void)setIMToken:(NSString*)uid token:(NSString*)token;

// 设置IM鉴权监听
- (void)setIMAuthEventDelegate:(id<LVIMModuleEventDelegate>)delegate;

// 设置全局消息监听
- (void)setIMGlobalReceiveMessageDelegate:(id<LVIMReceiveMessageDelegate>)delegate;

// 设置房间消息监听
- (void)setRoomMsgDelegate:(id<RoomCallbackProtocl>)delegate;

/**
@brief 构建私信请求
@param subType 内容类型(见LVIM_MESSAGE_SUBTYPE)
@param tid 接收者ID(用户ID)
@param type 消息类型别名(类型唯一)
@param content 消息内容
**/
- (LVIMMessage *)sendPrivateMessage:(LVIM_MESSAGE_SUBTYPE)subType
                       tid:(NSString *)tid
                      type:(NSString *)type
                   content:(NSString *)content
                  complete:(LVIM_SEND_MESSAGE_CALLBACK)complete;

/**
@brief 构建私信请求
@param subType 内容类型(见LVIM_MESSAGE_SUBTYPE)
@param tid 接收者ID(用户ID)
@param type 消息类型别名(类型唯一)
@param content 消息内容
@param pushTitle 推送标题
@param pushContent 推送内容
@param extend3 扩展字段(字符串:无特殊情况不要使用)
@param extend4 扩展字段(二进制:无特殊情况不要使用)
@param targetAppID 扩展字段5(目标应用ID)
@param targetAppUID 扩展字段6(目标应用UID)
**/
- (LVIMMessage *)sendPrivateMessage:(LVIM_MESSAGE_SUBTYPE)subType
                       tid:(NSString *)tid
                      type:(NSString *)type
                   content:(NSString *)content
                 pushTitle:(NSString *)pushTitle
               pushContent:(NSString *)pushContent
                   extend3:(NSString *)extend3
                   extend4:(NSData *)extend4
               targetAppID:(NSString *)targetAppID
              targetAppUID:(NSString *)targetAppUID
                  complete:(LVIM_SEND_MESSAGE_CALLBACK)complete;

/**
 * 发送房间消息
 * @param roomId   房间Id
 * @param content 消息内容
 * @param complete   发送结果监听
 */
- (LVIMMessage *)sendRoomMessage:(NSString *)roomId content:(NSString *)content complete:(LVIM_SEND_MESSAGE_CALLBACK)complete;

/**
 * 发送房间消息
 * @param roomId   房间Id
 * @param content 消息内容
 * @param msgType 消息的自定义扩展类型，可作为消息标识，但不能传空或者空串
 * @param complete   发送结果监听
 */
- (LVIMMessage *)sendRoomMessage:(NSString *)roomId content:(NSString *)content msgType:(NSString *)msgType complete:(LVIM_SEND_MESSAGE_CALLBACK)complete;

@end


