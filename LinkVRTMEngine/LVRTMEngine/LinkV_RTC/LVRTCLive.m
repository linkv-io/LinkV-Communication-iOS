//
//  LVRTCLive.m
//  iOS_Zego
//
//  Created by Wing on 2020/8/4.
//  Copyright © 2020 Wing. All rights reserved.
//


#import "LVRTCLive.h"
#import <LinkV/LinkV.h>

#ifdef DEBUG
#define DLog(...) NSLog(__VA_ARGS__)
#else
#define DLog(...) /* */
#endif

@interface LVRTCLive ()<LVRTCEngineDelegate>

@property (nonatomic, strong) NSMutableDictionary *playDictM;

@end

@implementation LVRTCLive

// 推流参数配置
 - (void)initSDK:(NSString *)appId appKey:(NSString *)appKey isTestEnv:(BOOL)isTestEnv completion:(LVCodeCompletion)completion {
     
     [LVRTCEngine initSdk];
     [LVRTCEngine setUseTestEnv:isTestEnv];
     [LVRTCEngine setUseInternationalEnv:YES];
     [[LVRTCEngine sharedInstance] auth:appId skStr:appKey userId:@"" completion:^(LVErrorCode code) {
         if (completion) {
             completion(code);
         }
     }];
}

- (void)setLogOpen:(BOOL)isOpen {
    LVLoggingSeverity log = isOpen ? kLVLoggingSeverityVerbose : kLVLoggingSeverityNone;
    [LVRTCEngine setLogLevel:log];
}

- (void)setVideoConfig:(VideoConfigLevel)configLevel {
    [[LVRTCEngine sharedInstance] setAVConfig:[[LVAVConfig alloc] initWithVideoProfile:configLevel]];
}

- (void)loginRoom:(NSString *)uid roomId:(NSString *)roomId isHost:(BOOL)isHost                     delegate:(id<RTCCallbackDelegate>)delegate{
    self.roomId = roomId;
    self.uid = uid;
    self.delegate = delegate;
    
    [[LVRTCEngine sharedInstance] loginRoom:uid roomId:roomId isHost:isHost isOnlyAudio:NO delegate:self];
}

- (void)logoutRoom {
    if (!self.roomId) return;
    
    [[LVRTCEngine sharedInstance] stopPublishing];
    
    NSArray *uids = self.playDictM.allKeys;
    for (NSString *uid in uids) {
        [self stopPlayingStream:uid];
    }
    
    [[LVRTCEngine sharedInstance] logoutRoom:^(LVErrorCode code) {
        
    }];
    self.uid = nil;
}

- (UIView *)startPreview:(UIView *)view {
    
    LVRTCDisplayView *displayView = [[LVRTCDisplayView alloc] initWithFrame:view.bounds];
    displayView.userInteractionEnabled = NO;
    [view insertSubview:displayView atIndex:0];
    displayView.userInteractionEnabled = NO;
    displayView.uid = self.uid;
    displayView.viewContentMode = LVViewContentModeScaleAspectFill;
    [[LVRTCEngine sharedInstance] addDisplayView:displayView];
    [[LVRTCEngine sharedInstance] startCapture];
    
    if (!self.uid) return nil;
    self.playDictM[self.uid] = displayView;
    
    return displayView;
}

- (void)startPublishing {
    [[LVRTCEngine sharedInstance] startPublishing];
}

- (UIView *)startPlayingStream:(NSString *)uid inView:(UIView *)view {
    if (!uid || !view) return nil;
    LVRTCDisplayView *displayView = [[LVRTCDisplayView alloc] initWithFrame:view.bounds];
    displayView.userInteractionEnabled = NO;
    [view insertSubview:displayView atIndex:0];
    displayView.uid = uid;
    displayView.viewContentMode = LVViewContentModeScaleAspectFill;
    [[LVRTCEngine sharedInstance] addDisplayView:displayView];
    [[LVRTCEngine sharedInstance] startPlayingStream:uid];
    
    self.playDictM[uid] = displayView;
    
    return displayView;
}

- (void)stopPlayingStream:(NSString *)uid {
    if (!uid) return;
    [[LVRTCEngine sharedInstance] stopPlayingStream:uid];
    
    LVRTCDisplayView *displayView = self.playDictM[uid];
    [[LVRTCEngine sharedInstance] removeDisplayView:displayView];
    [displayView removeFromSuperview];
    
    [self.playDictM removeObjectForKey:uid];
}

- (void)startCapture {
    [[LVRTCEngine sharedInstance] startCapture];
    
    NSLog(@"[Wing] build_version = %@", [LVRTCEngine buildVersion]);
}

- (void)stopCapture {
    [[LVRTCEngine sharedInstance] stopCapture];
}

// 转换前后置摄像头
- (void)useFrontCamera:(BOOL)isFront {
    [[LVRTCEngine sharedInstance] switchCamera:isFront ? LVRTCCameraPositionFront : LVRTCCameraPositionBack];
}

// 获取连麦SDK的名称
- (NSString *)getSolutionName { return @"LVRTCLive";}

// 获取支持连麦数
- (int)getSupportLine { return 8; }

// 获取SDK类型 3为自研，4为zego
- (SDKType)getSDKType { return SDK_TYPE_LVRTC; }

// 设置是否静音推流
- (void)enableMic:(BOOL)enbale {
    [[LVRTCEngine sharedInstance] enableMic:!enbale];
}

// 释放引擎
- (void)unInitSDK {
    [LVRTCEngine unInitSDK];
}

/// 停止发布音视频数据
- (void)stopPublishing{
    [[LVRTCEngine sharedInstance] stopPublishing];
}

- (int)getMicVolume { return [[LVRTCEngine sharedInstance] getSoundLevelByUserId:self.uid]; }
/**
 * 设置美颜滤镜级别
 * @param beautyLevel 滤镜美颜级别 取值范围[0,1]，设置-1关闭
 */
- (void)setBeautyLevel:(CGFloat)beautyLevel {
    [[LVRTCEngine sharedInstance].getLVBeautyManager setBeautyLevel:beautyLevel];
}
/**
 * 设置滤镜明亮度
 * @param brightLevel 滤镜明亮度 取值范围[0,1]，设置-1关闭
 */
- (void)setBrightLevel:(CGFloat)brightLevel {
    [[LVRTCEngine sharedInstance].getLVBeautyManager setBrightLevel:brightLevel];
}

/**
 * 设置滤镜饱和度
 * @param toneLevel 滤镜饱和度 取值范围[0,1]，设置-1关闭
 */
- (void)setToneLevel:(CGFloat)toneLevel {
    [[LVRTCEngine sharedInstance].getLVBeautyManager setToneLevel:toneLevel];
}

- (void)setPlayVolume:(int)volume {
    [[LVRTCEngine sharedInstance] setPlayVolume:volume];
}


- (void)setPlayVolume:(int)volume userId:(NSString *)userId {
    [[LVRTCEngine sharedInstance] setPlayVolume:volume userId:userId];
}


-(void)enableVideoAutoRotation:(BOOL)enable {
    [[LVRTCEngine sharedInstance] enableVideoAutoRotation:enable];
}

-(void)setOutputVideoRotation:(VideoRotation)rotation {
    [[LVRTCEngine sharedInstance] setOutputVideoRotation:rotation];
}

#pragma mark - LVRTCEngineDelegate
/// 加入房间成功的回调
/// @param code 加入房间是否成功错误码
/// @param users 用户列表，如果房间中有用户存在，该列表会自动生效
- (void)OnEnterRoomComplete:(LVErrorCode)code users:(nullable NSArray<LVUser *> *)users {
    
    DLog(@"[Wing] OnEnterRoomComplete code = %@", @(code));
    if (code == LVErrorCodeSuccess) {
        if ([self.delegate respondsToSelector:@selector(onRoomConnected:)]) {
            dispatch_async(dispatch_get_main_queue(), ^{
                [self.delegate onRoomConnected:self.roomId];
            });
        }
        for (LVUser *user in users) {
            if ([user.userId isEqualToString:self.uid]) continue;
            
            if ([self.delegate respondsToSelector:@selector(onAddRemoterUser:)]) {
                dispatch_async(dispatch_get_main_queue(), ^{
                    [self.delegate onAddRemoterUser:user.userId];
                });
            }
        }
    } else {
        if ([self.delegate respondsToSelector:@selector(onRoomDisconnect:roomId:)]) {
            dispatch_async(dispatch_get_main_queue(), ^{
                [self.delegate onRoomDisconnect:code roomId:self.roomId];
            });
        }
    }
}

- (void)OnAddRemoter:(LVUser *)user {
    DLog(@"[Wing] OnAddRemoter uid = %@", user.userId);
    if ([self.delegate respondsToSelector:@selector(onAddRemoterUser:)]) {
        dispatch_async(dispatch_get_main_queue(), ^{
            [self.delegate onAddRemoterUser:user.userId];
        });
    }
}

- (void)OnDeleteRemoter:(NSString *)uid {
    DLog(@"[Wing] OnDeleteRemoter uid = %@", uid);
    
    if ([self.delegate respondsToSelector:@selector(onRemoteLeave:)]) {
        dispatch_async(dispatch_get_main_queue(), ^{
            [self.delegate onRemoteLeave:uid];
        });
    }
}

- (void)OnKickOff:(NSInteger)reason roomId:(NSString *)roomId {
    DLog(@"[Wing] OnDeleteRemoter uid = %@", roomId);
    
    if ([self.delegate respondsToSelector:@selector(onRoomDisconnect:roomId:)]) {
        dispatch_async(dispatch_get_main_queue(), ^{
            [self.delegate onRoomDisconnect:(int)reason roomId:roomId];
        });
    }
}

- (void)OnRoomDisconnected:(LVErrorCode)code {
    dispatch_async(dispatch_get_main_queue(), ^{
        if ([self.delegate respondsToSelector:@selector(onRoomDisconnect:roomId:)]) {
            [self.delegate onRoomDisconnect:(int)code roomId:self.roomId];
        }
    });
}

- (void)OnRoomReconnected {
    dispatch_async(dispatch_get_main_queue(), ^{
       if ([self.delegate respondsToSelector:@selector(onRoomConnected:)]) {
           [self.delegate onRoomConnected:self.roomId];
       }
    });
}

- (void)OnMixComplete:(LVErrorCode)code {}

- (void)OnPublishQualityUpdate:(LVVideoStatistic *)quality {
//    DLog(@"[Wing] OnPublishQualityUpdate fps = %@", @(quality.videoFps));
    dispatch_async(dispatch_get_main_queue(), ^{
        if ([self.delegate respondsToSelector:@selector(onVideoSizeChanged:width:height:)]) {
            [self.delegate onVideoSizeChanged:self.uid width:quality.frameWidth height:quality.frameHeight];
        }
        if ([self.delegate respondsToSelector:@selector(onPublishStreamQualityUpdate:quality:)]) {
            [self.delegate onPublishStreamQualityUpdate:self.uid quality:quality];
        }
    });
}

- (void)OnPublishStateUpdate:(LVErrorCode)code {
    dispatch_async(dispatch_get_main_queue(), ^{
        if ([self.delegate respondsToSelector:@selector(onPublishStateUpdate:)]) {
            [self.delegate onPublishStateUpdate:(int)code];
        }
    });
}

- (void)OnPlayQualityUpate:(nonnull LVVideoStatistic *)quality userId:(nonnull NSString *)userId {
//    DLog(@"[Wing] onPlayQualityUpdate fps = %@ uid = %@", @(quality.videoFps), userId);
    dispatch_async(dispatch_get_main_queue(), ^{

        if ([self.delegate respondsToSelector:@selector(onVideoSizeChanged:width:height:)] && quality.frameWidth > 0) {
            [self.delegate onVideoSizeChanged:userId width:quality.frameWidth height:quality.frameHeight];
        }
        if ([self.delegate respondsToSelector:@selector(onRemoteQualityUpdate:quality:)]) {
            [self.delegate onRemoteQualityUpdate:userId quality:quality];
        }
    });
}

- (void)OnPlayStateUpdate:(LVErrorCode)code userId:(nonnull NSString *)userId {
    dispatch_async(dispatch_get_main_queue(), ^{
        if ([self.delegate respondsToSelector:@selector(onPlayStateUpdate:userId:)]) {
            [self.delegate onPlayStateUpdate:(int)code userId:userId];
        }
    });
}

- (void)OnExitRoomComplete {
    dispatch_async(dispatch_get_main_queue(), ^{
        if ([self.delegate respondsToSelector:@selector(onRoomDisconnect:roomId:)]) {
            [self.delegate onRoomDisconnect:0 roomId:self.roomId];
        }
    });
}

- (void)OnRoomDisconnect:(LVErrorCode)code {
    dispatch_async(dispatch_get_main_queue(), ^{
        if ([self.delegate respondsToSelector:@selector(onRoomDisconnect:roomId:)]) {
            [self.delegate onRoomDisconnect:0 roomId:self.roomId];
        }
    });
}

- (void)OnAudioData:(NSString *)uid audio_data:(const void *)audio_data bits_per_sample:(int)bits_per_sample sample_rate:(int)sample_rate number_of_channels:(size_t)number_of_channels number_of_frames:(size_t)number_of_frames {
    
}

- (void)OnAudioMixStream:(nonnull const int16_t *)data samples:(int)samples nchannel:(int)nchannel flag:(LVAudioRecordType)flag {
    
}


- (void)OnAudioVolumeUpdate:(nonnull NSArray<LVAudioVolume *> *)soundLevels {
    
}


- (void)OnCaptureSoundLevelUpdate:(nonnull LVAudioVolume *)captureSoundLevel {
    
}

#pragma mark - 懒加载
- (NSMutableDictionary *)playDictM {
    if (!_playDictM) {
        _playDictM = [NSMutableDictionary dictionary];
    }
    return _playDictM;
}

@end
