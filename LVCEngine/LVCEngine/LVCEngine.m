//
//  LVCEngine.m
//  LinkV_Zego_iOS
//
//  Created by Wing on 2020/8/5.
//

#import "LVCEngine.h"

@interface LVCEngine ()<LVIMReceiveMessageDelegate>

@property (nonatomic, strong) LinkVRTCEngine *engine;
@property (nonatomic, strong) LVIMSDK *imSDK;

@property (nonatomic,weak) id<LVIMModuleEventDelegate, LVIMReceiveMessageDelegate, RoomCallbackProtocl> delegate;
@property (nonatomic,weak) id<RoomCallbackProtocl> roomMsgDelegate;

@end

@implementation LVCEngine

//+ (instancetype)sharedForLinkv;
+ (instancetype)shared {
    //仅共享IM
    static dispatch_once_t onceToken;
    static LVCEngine *_engine;
    dispatch_once(&onceToken, ^{
        _engine = [self new];
    });
    
    return _engine;
}

+ (instancetype)createEngineWithAppId:(NSString *)appId
                   appKey:(NSString *)appKey
                   isTestEnv:(BOOL)isTestEnv
                           completion:(LVCodeCompletion)completion {
    return [self createEngine:SDK_TYPE_LVRTC rtcAppId:appId rtcAppKey:appKey imAppId:appId imAppKey:appKey isTestEnv:isTestEnv completion:completion];
}

+ (instancetype)createEngine:(SDKType)type
                    rtcAppId:(NSString *)rtcAppId
                   rtcAppKey:(NSString *)rtcAppKey
                     imAppId:(NSString *)imAppId
                    imAppKey:(NSString *)imAppKey
                   isTestEnv:(BOOL)isTestEnv
                  completion:(LVCodeCompletion)completion {
    
    LVCEngine *rtm = [self shared];
    rtm.engine = [LinkVRTCEngine createEngine:type appId:rtcAppId appKey:rtcAppKey isTestEnv:isTestEnv completion:completion];
    
    if (rtm.imSDK) {
        [rtm.imSDK setDebugMode:isTestEnv];
    } else {
        rtm.imSDK = [LVIMSDK sharedInstance];
        [rtm.imSDK initWithAppId:imAppId secret:imAppKey];
        [rtm.imSDK setDebugMode:isTestEnv];
        [rtm.imSDK start];
    }
    
    return rtm;
}

- (void)setLogOpen:(BOOL)isOpen {
    [self.imSDK setLogVisible:isOpen];
    [self.engine setLogOpen:isOpen];
}

#pragma mark - RTC
// 推流参数配置
- (void)setVideoConfig:(VideoConfigLevel)configLevel { [self.engine setVideoConfig:configLevel]; }
- (void)loginRoom:(NSString *)uid roomId:(NSString *)roomId isHost:(BOOL)isHost delegate:(id<RoomCallbackProtocl> _Nullable)delegate {
    
    [self setRoomMsgDelegate:delegate];
    [self.imSDK setChatroomReceiveMessageDelegate:self];
    int result = [self.imSDK joinChatroom:roomId context:nil callback:^(int ecode, int rcode, int64_t lvsgid, int64_t smsgid, int64_t stime, LVIMMessage *msg) {
        if (ecode == 0 || ecode == 200) {
            [self.engine loginRoom:uid roomId:roomId isHost:isHost delegate:delegate];
        } else {
            dispatch_async(dispatch_get_main_queue(), ^{
                if ([self.delegate respondsToSelector:@selector(onRoomDisconnect:roomId:)]) {
                    [self.delegate onRoomDisconnect:ecode roomId:roomId];
                }
            });
        }
    }];
    
    if (result != 0) {
        if ([self.delegate respondsToSelector:@selector(onRoomDisconnect:roomId:)]) {
            [self.delegate onRoomDisconnect:result roomId:roomId];
        }
    }
}

/// 开始采集视频流
- (void)startCapture {
    [self.engine startCapture];
}
/// 停止采集视频流
- (void)stopCapture {
    [self.engine stopCapture];
}

- (void)logoutRoom {
    NSString *roomId = self.engine.roomId;
    [self.engine logoutRoom];
    [self.imSDK leaveChatroom:roomId context:nil callback:nil];
    [self.imSDK setChatroomReceiveMessageDelegate:nil];
}

- (UIView *)startPreview:(UIView *)view {
    return [self.engine startPreview:view];
}
- (void)startPublishing {
    [self.engine startPublishing];
}
- (UIView *)startPlayingStream:(NSString *)uid inView:(UIView *)view {
    return [self.engine startPlayingStream:uid inView:view];
}
- (void)stopPlayingStream:(NSString *)uid {
    [self.engine stopPlayingStream:uid];
}
// 转换前后置摄像头
- (void)useFrontCamera:(BOOL)isFront {
    [self.engine useFrontCamera:isFront];
}
// 获取连麦SDK的名称
- (NSString *)getSolutionName {
    return [self.engine getSolutionName];
}
// 获取支持连麦数
- (int)getSupportLine {
    return [self.engine getSupportLine];
}
// 获取SDK类型 3为自研，4为zego
- (SDKType)getSDKType {
    return [self.engine getSDKType];
}
// 设置是否静音推流
- (void)enableMic:(BOOL)isMute {
    [self.engine enableMic:isMute];
}
// 释放引擎
- (void)unInitSDK {
    [self.engine unInitSDK];
}
- (int)getMicVolume {
    return [self.engine getMicVolume];
}

/// 设置是否允许 SDK 自动根据设备的方向调整视频的输出方向
/// @param enable 打开和关闭（默认是 YES）
-(void)enableVideoAutoRotation:(BOOL)enable {
    [self.engine enableVideoAutoRotation:enable];
}

/**
 * 设置美颜滤镜级别
 * @param beautyLevel 滤镜美颜级别 取值范围[0,1]，设置-1关闭
 */
- (void)setBeautyLevel:(CGFloat)beautyLevel {
    [self.engine setBeautyLevel:beautyLevel];
}
/**
 * 设置滤镜明亮度
 * @param brightLevel 滤镜明亮度 取值范围[0,1]，设置-1关闭
 */
- (void)setBrightLevel:(CGFloat)brightLevel {
    [self.engine setBrightLevel:brightLevel];
}
/**
 * 设置滤镜饱和度
 * @param toneLevel 滤镜饱和度 取值范围[0,1]，设置-1关闭
 */
- (void)setToneLevel:(CGFloat)toneLevel {
    [self.engine setToneLevel:toneLevel];
}

#pragma mark - IM
- (int)loginIM:(NSString *)uid delegate:(id<LVIMModuleEventDelegate>)delegate {
    self.uid = uid;
    [self setIMAuthEventDelegate:delegate];
    return [self.imSDK login:uid];
}

// 设置IM鉴权监听
- (void)setIMAuthEventDelegate:(id<LVIMModuleEventDelegate>)delegate {
    [self.imSDK setModuleEventDelegate:delegate];
}

// 设置全局消息监听
- (void)setIMGlobalReceiveMessageDelegate:(id<LVIMReceiveMessageDelegate>)delegate {
    [self.imSDK setGlobalReceiveMessageDelegate:delegate];
}

- (void)setRoomMsgDelegate:(id<RoomCallbackProtocl>)delegate {
    _roomMsgDelegate = delegate;
    
    [self.engine performSelector:@selector(setDelegate:) withObject:delegate];
}

- (LVIMMessage *)sendPrivateMessage:(LVIM_MESSAGE_SUBTYPE)subType
                       tid:(NSString *)tid
                      type:(NSString *)type
                   content:(NSString *)content
                  complete:(LVIM_SEND_MESSAGE_CALLBACK)complete {
    return [self sendPrivateMessage:subType tid:tid type:type content:content pushTitle:nil pushContent:nil extend3:nil extend4:nil targetAppID:nil targetAppUID:nil complete:complete];
}

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
                 complete:(LVIM_SEND_MESSAGE_CALLBACK)complete {
    
    LVIMMessage *msg = [LVIMMessage new];
    int result = [msg buildPrivateRequest:subType fid:self.uid tid:tid type:type ?: @"" content:content pushTitle:pushTitle pushContent:pushContent extend3:extend3 extend4:extend4 targetAppID:targetAppID targetAppUID:targetAppUID];
    
    if (result != 0) {
        if (complete) {
            complete(ERROR_BUILD_MSG, 0, 0, 0, 0, nil);
        }
        return nil;
    }
    
    [self.imSDK sendMessage:msg context:nil callback:complete];
    return msg;
}

/**
 * 发送房间消息
 * @param roomId   房间Id
 * @param content 消息内容
 * @param complete   发送结果监听
 */
- (LVIMMessage *)sendRoomMessage:(NSString *)roomId content:(NSString *)content complete:(LVIM_SEND_MESSAGE_CALLBACK)complete {
    return [self sendRoomMessage:roomId content:content msgType:@"msgTye" complete:complete];
}

/**
 * 发送房间消息
 * @param roomId   房间Id
 * @param content 消息内容
 * @param msgType 消息的自定义扩展类型，可作为消息标识，但不能传空或者空串
 * @param complete   发送结果监听
 */
- (LVIMMessage *)sendRoomMessage:(NSString *)roomId content:(NSString *)content msgType:(NSString *)msgType complete:(LVIM_SEND_MESSAGE_CALLBACK)complete {
    
    LVIMMessage *msg = [LVIMMessage new];
    int result = [msg buildChatroomRequest:self.uid tid:roomId type:msgType content:content];
    
    if (result != 0) {
        if (complete) {
            complete(ERROR_BUILD_MSG, 0, 0, 0, 0, nil);
        }
        return nil;
    }
    
    [self.imSDK sendMessage:msg context:nil callback:complete];
    return msg;
}

#pragma mark - LVIMReceiveMessageDelegate 房间消息
- (BOOL)onIMReceiveMessageFilter:(int32_t)diytype fromid:(const char *)fromid toid:(const char *)toid msgtype:(const char *)msgtype content:(const char *)content waitings:(int)waitings packetSize:(int)packetSize waitLength:(int)waitLength bufferSize:(int)bufferSize {
    NSString *to = [NSString stringWithUTF8String:toid];
    return ![to isEqualToString:self.engine.roomId];
}

- (int)onIMReceiveMessageHandler:(NSString *)owner immsg:(LVIMMessage *)immsg waitings:(int)waitings packetSize:(int)packetSize waitLength:(int)waitLength bufferSize:(int)bufferSize {
    
    if ([self.roomMsgDelegate respondsToSelector:@selector(onRoomMessageReceive:)]) {
        return [self.roomMsgDelegate onRoomMessageReceive:immsg];
    }
    
    return 0;
}

@end

