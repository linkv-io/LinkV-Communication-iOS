//
//  LinkVRTCEngine.h
//  iOS_Zego
//
//  Created by Wing on 2020/8/4.
//  Copyright © 2020 Wing. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#import "VideoQuality.h"

typedef NS_ENUM(NSInteger, SDKType) {
    SDK_TYPE_LVRTC = 3,
    SDK_TYPE_ZEGO  = 4,
};

typedef NS_ENUM(NSInteger, VideoConfigLevel) {
    Video_Config_180P = 0,
    Video_Config_270P,
    Video_Config_360P,
    Video_Config_540P,
    Video_Config_720P,
    Video_Config_1080P,
};

typedef NS_ENUM(NSInteger, VideoRotation) {
    VideoRotation_0          = 0,
    VideoRotation_90         = 90,
    VideoRotation_180        = 180,
    VideoRotation_270        = 270,
};

/**
 音视频录制支持模式
 */
typedef enum : NSUInteger {
    /**
        仅录制音频
     */
    LVRecorderType_AUDIO = 0,
    /**
        仅录制视频
     */
    LVRecorderType_VIDEO,
    /**
        音频和视频同时录制
     */
    LVRecorderType_AUDIO_AND_VIDEO,
    
} LVRecorderType;

typedef void (^LVCodeCompletion)(NSInteger code);

@protocol RTCCallbackDelegate <NSObject>

/// 有成员推流加入回调
/// @param uid 用户 id
- (void)onAddRemoterUser:(NSString *_Nullable)uid;

/// 有成员停止推流回调
/// @param uid 用户 id
- (void)onRemoteLeave:(NSString *_Nullable)uid;

/// 加入房间回调
/// @param roomId 房间 ID
- (void)onRoomConnected:(NSString *_Nullable)roomId;

@optional

/// 被踢出房间或断开房间链接
/// @param errorCode 参见Zego或LinkV错误码
/// @param roomId  房间id
- (void)onRoomDisconnect:(int)errorCode roomId:(NSString *_Nullable)roomId;

/// 推流状态变化
/// @param code 错误码
- (void)onPublishStateUpdate:(int)code;

/// 播放状态更新
/// @param code 错误码
/// @param userId 用户 ID
- (void)onPlayStateUpdate:(int)code userId:(nonnull NSString *)userId;

/// 推流质量变化
/// @param uid 用户 ID
/// @param quality 质量
- (void)onPublishStreamQualityUpdate:(NSString *_Nullable)uid quality:(VideoQuality *_Nullable)quality;

/// 视频 分辨率 变化
/// @param uid 用户 id
/// @param width 宽度
/// @param height 高度
- (void)onVideoSizeChanged:(NSString *_Nullable)uid width:(int)width height:(int)height;

/// 远端播放质量变化通知
/// @param uid 用户 id
/// @param quality 质量
- (void)onRemoteQualityUpdate:(NSString *_Nullable)uid quality:(VideoQuality *_Nullable)quality;

/// 收到视频首帧回调通知事件
/// @param userId 视频发送者用户 ID
/// @param streamId 流 ID，默认为空字符串，如果使用自定义视频流时为自定义视频流对应的流名称
- (void)OnReceivedFirstVideoFrame:(NSString *_Nullable)userId streamId:(NSString *_Nullable)streamId;


/// 收到视频首帧回调通知事件
/// @param userId 视频发送者用户 ID
/// @param streamId 流 ID，默认为空字符串，如果使用自定义视频流时为自定义视频流对应的流名称
- (void)OnReceivedFirstAudioFrame:(NSString *_Nullable)userId streamId:(NSString *_Nullable)streamId;

@end

@protocol RTCProtocol <NSObject>

/// 初始化 SDK
- (void)initSDK:(NSString *_Nullable)appId appKey:(NSString *_Nullable)appKey isTestEnv:(BOOL)isTestEnv completion:(LVCodeCompletion _Nullable )completion;

/// 设置是否开启日志
- (void)setLogOpen:(BOOL)isOpen;

/// 推流参数配置
- (void)setVideoConfig:(VideoConfigLevel)configLevel;

/// 登录房间
- (void)loginRoom:(NSString *_Nullable)uid roomId:(NSString *_Nullable)roomId isHost:(BOOL)isHost                     delegate:(id<RTCCallbackDelegate>_Nullable)delegate;

/// 退出房间
- (void)logoutRoom;

#pragma mark 视频控制相关方法

/// 开启摄像头
- (void)startCapture;

/// 停止摄像头
- (void)stopCapture;

/// 渲染视图
- (UIView *_Nullable)startPreview:(UIView *_Nonnull)view;

/// 发布音视频数据（推流）
- (void)startPublishing;

/// 停止发布音视频数据
- (void)stopPublishing;

/// 播放流
- (UIView *_Nullable)startPlayingStream:(NSString *_Nullable)uid inView:(UIView *_Nonnull)view;

/// 停止播放流
- (void)stopPlayingStream:(NSString *_Nonnull)uid;

/// 转换前后置摄像头
- (void)useFrontCamera:(BOOL)isFront;

/// 开启或关闭麦克风
- (void)enableMic:(bool)enbale;

/** 控制所有远端音频流音量, 加入房间成功后调用
 *  @param volume 音量大小（0，100）
 */
- (void)setPlayVolume:(int)volume;

/** 控制远端音频流音量, 加入房间成功后调用
 * @param volume 音量大小（0，100）
 * @param userId 被控制的用户ID
 */
- (void)setPlayVolume:(int)volume userId:(NSString *_Nullable)userId;

#pragma mark - 视频录制
/// @param userId 用户 ID
/// @param path 文件路径
/// @param type 音视频录制类型
/// @return 0 : 录制成功， 其他 : 录制失败
- (int)startRecorder:(NSString *_Nonnull)userId path:(NSString *_Nonnull)path type:(LVRecorderType)type;


/// 停止录制
/// @param userId 音视频用户 ID
/// @return 0 : 停止录制成功，其他 : 停止录制失败
- (int)stopRecorder:(NSString *_Nonnull)userId;

#pragma mark 其它相关方法

/// 获取连麦SDK的名称
- (NSString *_Nonnull)getSolutionName;

/// 获取支持连麦数
- (int)getSupportLine;

/// 获取SDK类型 3为自研，4为zego
- (SDKType)getSDKType;

/// 释放引擎
- (void)unInitSDK;

/// 获取音量
- (int)getMicVolume;

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

/** 设置是否允许 SDK 自动根据设备的方向调整视频的输出方向
 *  @param enable 打开和关闭（默认是 YES）
 */
- (void)enableVideoAutoRotation:(BOOL)enable;

/** 设置手动管理视频输出方向，该方法需要先调用 enableVideoAutoRotateAdapter，将自动旋转方法关闭才能生效
 *  @param rotation 视频输出方向，（默认是 VideoRotation_0）
 */
- (void)setOutputVideoRotation:(VideoRotation)rotation;

@end

@interface LinkVRTCEngine : NSObject<RTCProtocol>

+ (instancetype _Nullable )createEngine:(SDKType)type
                                  appId:(NSString *_Nullable)appId
                                 appKey:(NSString *_Nullable)appKey
                   isTestEnv:(BOOL)isTestEnv
                             completion:(LVCodeCompletion _Nullable )completion;

@property (nonatomic, copy, nullable) NSString *roomId;
@property (nonatomic, copy, nullable) NSString *uid;

@end
