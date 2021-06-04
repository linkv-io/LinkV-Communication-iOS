//
//  LVRTCEngine.h
//  LVRTCEngine
//
//  Copyright © 2021年 LinkV. All rights reserved.
//
#import <Foundation/Foundation.h>
#import "LVRTCEngineDefines.h"

NS_ASSUME_NONNULL_BEGIN

@class LVRTCEngine;
@class LVRTCEngineDelegate;

#pragma mark -
#pragma mark - LVRTCEngineDelegate

LV_EXPORT_CLASS

/// 音视频引擎代理回调方法
@protocol LVRTCEngineDelegate <NSObject>
@required

/// 房间重连成功通知，SDK 在网络断开和恢复时会自动进行重连，重连结果通过该回调通知 APP
- (void)OnRoomReconnected;

/// 加入房间成功的回调
/// @param code 加入房间是否成功错误码
/// @param users 用户列表，如果房间中有用户存在，该列表会自动生效
- (void)OnEnterRoomComplete:(LVErrorCode)code users:(nullable NSArray<LVUser*>*)users;

/// 退出房间完成，异常退出和用户主动调用 logoutRoom 方法会触发该回调方法
- (void)OnExitRoomComplete;

/// 房间断开的通知，网络异常时 SDK 会自动进行重连，90 秒如果重连失败会触发该方法回调
/// @param code 错误码
- (void)OnRoomDisconnected:(LVErrorCode)code;

/// 有成员加入的通知
/// @param user 成员列表
- (void)OnAddRemoter:(LVUser *)user;

/// 有成员离开的通知
/// @param uid 离开用户 ID
- (void)OnDeleteRemoter:(NSString*)uid;

/// 远端混流完成的回调，用户可以通过调用混乱方法在远端进行音视频混流，参考 mixStream
/// @param code 混流结果
- (void)OnMixComplete:(LVErrorCode)code;

/// 单路音频流回调
/// @param uid 该路音频的用户 ID
/// @param audio_data 音频数据
/// @param bits_per_sample PCM 位深
/// @param sample_rate 采样率
/// @param number_of_channels 通道数
/// @param number_of_frames 帧数量
- (void)OnAudioData:(NSString *)uid
         audio_data:(const void*)audio_data
    bits_per_sample:(int)bits_per_sample
        sample_rate:(int)sample_rate
 number_of_channels:(size_t)number_of_channels
   number_of_frames:(size_t)number_of_frames;

/// 混音录音数据回调，该方法需要打开录音功能才会触发回调 setAudioRecordFlag:
/// @param data pcm 数据
/// @param samples 数据长度
/// @param nchannel 通道数据
/// @param samplesPerChannel 每声道采样数
/// @param flag 类型
- (void)OnAudioMixStream:(const int16_t *)data samples:(int)samples nchannel:(int)nchannel samplesPerChannel:(int)samplesPerChannel flag:(LVAudioRecordType)flag;


/// 发布资源质量状态变化的通知，SDK 内部会定时统计推流的质量信息，APP 可以实现该方法获取推流的视频质量信息统计，回调的频率可以通过 +setPublishQualityMonitorCycle 进行设置
/// @param quality 质量信息
- (void)OnPublishQualityUpdate:(LVVideoStatistic *)quality;

/// 播放质量变化的通知，SDK 内部会定时统计拉流质量信息，APP 可以实现该方法获取拉流的视频质量信息统计，回调的频率可以通过 +setPlayQualityMonitorCycle 进行设置
/// @param quality 质量信息
/// @param userId 用户 ID
- (void)OnPlayQualityUpate:(LVVideoStatistic *)quality userId:(NSString*)userId;

/// 发布资源状态变更的通知，调用 startPublishing 之后会触发该方法回调，发布资源状态的变更会通过该方法进行通知
/// @param code 状态码
- (void)OnPublishStateUpdate:(LVErrorCode)code;

/// 播放状态变化的通知，调用 startPlayingStream: 方法之后，播放状态的变更会通过该方法进行通知
/// @param code 状态码
/// @param userId 用户 ID
- (void)OnPlayStateUpdate:(LVErrorCode)code userId:(NSString*)userId;

/// 远端用户音量变化的通知，该回调通需要调用打开音量变化监听方法才会生效 startSoundLevelMonitor，该回调频率可以调用 startSoundLevelMonitor:
/// @param soundLevels 远端用户音量信息
- (void)OnAudioVolumeUpdate:(NSArray<LVAudioVolume *> *)soundLevels;

@optional
/// 是否需要在视频帧上附加其他媒体信息
/// @param timestamp 当前要发送数据帧的时间戳
/// @return 请返回要随当前视频帧发送的附加数据 （字符串最长长度 24 个字符）
-(NSString *)OnMediaSideInfoInPublishVideoFrame:(NSUInteger)timestamp;

/// 收到远端视频数据回调，如果为 SDK 设置了渲染视图，SDK 内部会自动将该视频帧渲染出来
/// @param pixelBuffer 已解码的视频数据
/// @param uid 用户 ID
/// @param sei 随视频数据携带的附加数据信息
/// @return 建议返回当前渲染完成的时间戳，用以统计渲染延迟
/// @discussion 注：如果用户实现该接口，则 SDK 内部的远端视频渲染会自动实现，渲染完全由外部进行实现
- (int64_t)OnDrawFrame:(CVPixelBufferRef)pixelBuffer
                   uid:(NSString *)uid
                   sei:(NSString *)sei;

/// SDK 内部采集数据回调
/// @param videoFrame 视频原始数据
/// @return 处理后的视频数据，SDK 内部不会调用 CFRetain 增加引用计数， 会调用 CFRelease 减少引用计数， 视频开发者创建的视频对象
-(CMSampleBufferRef)OnCaptureVideoFrame:(CMSampleBufferRef)videoFrame;

/// 用户被踢出房间
/// @param reason 被踢原因
/// @param roomId 房间号
- (void)OnKickOff:(NSInteger)reason roomId:(NSString *)roomId;

/// 混音播放器当前播放时长
/// @param time_ms 播放时长
- (void)AudioMixerCurrentPlayingTime:(int)time_ms;

/// 混音播放器当前音频播放结束
- (void)AudioMixerPlayerDidFinished;


/// 收到视频首帧回调通知事件
/// @param userId 视频发送者用户 ID
/// @param streamId 流 ID，默认为空字符串，如果使用自定义视频流时为自定义视频流对应的流名称
- (void)OnReceivedFirstVideoFrame:(NSString *)userId streamId:(NSString *)streamId;


/// 收到音频首帧回调通知事件
/// @param userId 音视频发送者用户 ID
/// @param streamId 流 ID，默认为空字符串，如果使用自定义视频流时为自定义视频流对应的流名称
- (void)OnReceivedFirstAudioFrame:(NSString *)userId streamId:(NSString *)streamId;

@end


#pragma mark -
#pragma mark - LVRTCEngine

LV_EXPORT_CLASS

/// RTC 引擎服务主类，音视频基本功能通过该类提供
@interface LVRTCEngine : NSObject

/// 不能直接使用，请通过 sharedInstance 获取该实例
/// @warning 不能直接使用
+(instancetype) alloc __attribute__((unavailable("alloc not available, call sharedInstance instead")));

/// 不能直接使用，请通过 sharedInstance 获取该实例
/// @warning 不能直接使用
-(instancetype) init __attribute__((unavailable("init not available, call sharedInstance instead")));

/// 不能直接使用，请通过 sharedInstance 获取该实例
/// @warning 不能直接使用
+(instancetype) new __attribute__((unavailable("new not available, call sharedInstance instead")));

/// LVRTCEngine SDK 版本号（1.0.1）
/// @return 版本号
/// @discussion 开发者使用本 API 获取 SDK 版本号
+ (NSString *)versionName;

/// LVRTCEngine SDK 版本号，数字（10001）
/// @return 版本号
/// @discussion 开发者使用本 API 获取 SDK 版本号
+ (NSInteger)versionNumber;

/// SDK 内部版本号，汇报给 linkv 时可以提供该版本号
+ (NSString *)buildVersion;

/// 初始化 SDK
+ (void)initSdk;

/// 卸载 SDK，销毁单例对象资源
+ (void)unInitSDK;

/// 设置 ISO 国家码, 辅助调度信息.
/// @param iso iso 国家码参数
+ (void)setISOCountryCode:(NSString *)iso;

/// 设置推流时数据统计信息回调的频率，回调方法请看 OnPublishQualityUpdate 回调
/// @param sec 回调频率（1 / sec）
+ (void)setPublishQualityMonitorCycle:(int)sec;

/// 设置拉流时数据统计信息回调的频率，回调方法请看 OnPlayQualityUpate 回调
/// @param sec 回调频率（1 / sec）
+ (void)setPlayQualityMonitorCycle:(int)sec;

/// 设置日志打印级别（SDK 日志保存在缓存目录的 cmliveroom 目录下）
/// @param level 日志打印级别
+ (void)setLogLevel:(LVLoggingSeverity)level;

/// 是否使用测试环境
/// @param env YES 为使用测试环境，NO 为不使用测试环境（默认为 NO）
+ (void)setUseTestEnv:(BOOL)env;

/// 设置调试的 signal  IP 地址
/// @param ip signal IP 地址
+ (void)setDebugServerIp:(NSString *)ip;

/// 设置接收到视频数据时解码的像素格式类型
/// @param osType 像素格式类型（CVPixelFormatType）
+ (bool)setDecoderPixelType:(OSType)osType;

/// 设置视频数据编码的像素格式类型
/// @param osType 像素格式类型（CVPixelFormatType）
+ (bool)setEncoderPixelType:(OSType)osType;

/// 返回 LVRTCEngine 实例对象
+ (instancetype)sharedInstance;


/// SDK 鉴权
/// @param appId 向 LiveMe 申请的 appId
/// @param skStr  SDK 签名字符串
/// @param userId 用户 ID
/// @param completion 鉴权完成回调
- (void)auth:(NSString *)appId
       skStr:(NSString *)skStr
      userId:(NSString *)userId
  completion:(LVServiceCompletion)completion;


/// 设置视频编码参数
/// @param config 编码参数配置
/// @discussion 注：如果后面设置的视频采集分辨率大于之前设置的分辨率，摄像头可能会进行重庆操作
- (void)setAVConfig:(LVAVConfig *)config;

/// 获取当前视频编码参数
/// @return 视频编码参数
- (LVAVConfig *)getAVConfig;


/// 加入音视频房间（音视频会议）
/// @param userId 用户 ID
/// @param roomId 房间 ID
/// @param isHost 是否是主持人
/// @param isOnlyAudio 是否以音频模式加入
/// @param delegate 房间回调代理
- (void)loginRoom:(NSString *)userId
           roomId:(NSString *)roomId
           isHost:(bool)isHost
      isOnlyAudio:(bool)isOnlyAudio
         delegate:(id<LVRTCEngineDelegate>)delegate;

/// 退出房间
/// @param completion 离开房间结果回调
- (void)logoutRoom:(LVServiceCompletion)completion;

/// 开始远端视频混流（将多路视频流混合在一个视频流中），混流成功与否通过 OnMixComplete 回调告知
/// @param config 混乱配置
- (bool)mixStream:(LVMixStreamConfig*)config;

/// 停止混流
- (void)stopMixStream;

/// 发布音视频数据（推流），如果登录房间时 isOnlyAudio 为 YES 则只发送音频数据，视频数据不发送，推流状态的变更通过 OnPublishStateUpdate: 回调
- (bool)startPublishing;

/// 停止发布音视频数据
- (void)stopPublishing;

/// 开始播放一路音视频流，播放状态的变更通过 OnPlayStateUpdate:userId: 回调
/// @param userId 要播放的音视频流用户 ID
- (void)startPlayingStream:(NSString *)userId;

/// 停止播放一路音视频流
/// @param userId 要停止播放的音视频流用户 ID
- (void)stopPlayingStream:(NSString *)userId;

/// 房间之间 PK（跨房间连麦功能）
/// @param roomId 房间 ID
- (void)linkRoom:(NSString *)roomId;

/// 取消跨房间连麦功能
/// @param roomId 房间 ID
- (void)unlinkRoom:(NSString *)roomId;

/// 房间之间 PK（跨房间连麦功能）
/// @param roomId 房间 ID
/// @param linkUserId PK 对象的 userId
- (void)linkRoom:(NSString *)roomId linkUserId:(NSString *)linkUserId;

/// 取消跨房间连麦功能
/// @param roomId 房间 ID
/// @param unlinkUserId 取消 pk 对象的 userId
- (void)unlinkRoom:(NSString *)roomId unlinkUserId:(NSString *)unlinkUserId;


/// 开始视频采集
-(void)startCapture;

/// 停止视频采集
-(void)stopCapture;

/// 切换摄像头
/// @param position 摄像头位置
-(void)switchCamera:(LVRTCCameraPosition)position;

/// 为音视频设置预览视图，请先加入房间后再进行设置，SDK 内部会自动将视图和接收到的音视频数据进行绑定操作
/// @param view 视图信息
/// @discussion 如果不传 uid 则该视图默认为当前登录用户
-(void)addDisplayView:(LVRTCDisplayView *)view;

/// 移除预览视图
/// @param view 视图信息
-(void)removeDisplayView:(LVRTCDisplayView *)view;

/// 开启或关闭麦克风
/// @param enbale true 开启，false 关闭（默认为 true）
/// @return true 成功，false 失败
/// @discussion 推流时可调用本 API 进行参数配置
- (bool)enableMic:(bool)enbale;

/// 扬声器和听筒切换功能
/// @param enable true 视同扬声器播放音频，false 使用听筒播放音频。（默认 true）
/// @return true 成功，false 失败
- (bool)enableSpeakerphone:(bool)enable;

/// 声音数据回调参数设置，支持多种录制模式，录制结果通过 OnAudioMixStream: samples:nchannel:nchannel flag: 回调
/// @param flag 参考 CMAudioRecodType, 默认 CMAudioRecodTypeNone 标识不回调
- (void)setAudioRecordFlag:(LVAudioRecordType)flag;

/// 开启音量变化监听，并设置音量变化监听回调频率，成功后回调 OnAudioVolumeUpdate:
/// @param timeInMS 单位毫秒（默认回调频率为 100 ms 回调一次）
- (void)startSoundLevelMonitor:(unsigned int)timeInMS;

/// 停止音量变化的监听
- (void)stopSoundLevelMonitor;

/// 获取用户的音量信息
/// @param userId 用户 ID
/// @return 用户 ID 对应的音量数据
- (int)getSoundLevelByUserId:(NSString *)userId;

/// 设置是否允许 SDK 自动根据设备的方向调整视频的输出方向
/// @param enable 打开和关闭（默认是 YES）
-(void)enableVideoAutoRotation:(BOOL)enable;


/// 设置手动管理视频输出方向，该方法需要先调用 enableVideoAutoRotateAdapter，将自动旋转方法关闭才能生效
/// @param rotation 视频输出方向，（默认是 LVVideoRotation_0）
-(void)setOutputVideoRotation:(LVVideoRotation)rotation;

/// 开始播放音乐文件及混音（声音伴奏）
/// @param filePath 文件路径
/// @param mode LVAudioMixingMode，设置混音模式
/// @param loop 指定音频文件循环播放的次数，正整数：循环的次数 -1：无限循环
/// @return 0 ： 方法调用成功，!0：失败
-(int)startAudioMixing:(NSString*)filePath mode:(LVAudioMixingMode)mode loop:(int)loop;

/// 动态调整混音模式
/// @param mode 混音模式参数
-(void)setAudioMixingMode:(LVAudioMixingMode)mode;

/// 停止播放音乐文件及混音。请在房间内调用该方法。
/// @return 0：方法调用成功 !0：方法调用失败
-(int)stopAudioMixing;

/// 暂停播放音乐文件及混音。请在房间内调用该方法。
/// @return 0：方法调用成功 !0：方法调用失败
-(int)pauseAudioMixing;

/// 恢复播放音乐文件及混音。请在房间内调用该方法。
/// @return 0：方法调用成功 !0：方法调用失败
-(int)resumeAudioMixing;

/// 获取当前伴奏音量，请在伴奏开始后调用
/// @return -1：伴奏未开始 0～100：当前伴奏音量
-(int)getAudioMixingVolume;

/// 调节音乐文件的播放音量。请在房间内调用该方法。
/// @param volume 音量：0～100
/// @return 0：方法调用成功 !0：方法调用失败
-(int)adjustAudioMixingVolume:(int)volume;

/// 获取当前伴奏文件总时长，单位毫秒。请在房间内调用该方法。
/// @return 文件长度
-(long)getAudioMixingTotalLength;

/// 获取音乐文件的播放进度。单位为毫秒。请在房间内调用该方法。
/// @return <0：方法调用失败 >=0：方法调用成功并返回伴奏播放进度
-(long)getAudioMixingCurrentPosition;

/// 设置音乐文件的播放位置。请在房间内调用该方法。
/// @param pos 整数。进度条位置，单位为毫秒
/// @return 0：方法调用成功 !0：方法调用失败
-(long)setAudioMixingPosition:(int)pos;

/// 设置外置音频数据参数
/// @param audioConfig 音频参数
-(void)setExternalAudioConfig:(LVExternalAudioConfig *)audioConfig;

/// 获取当前外置音频参数
/// @return 外置音频参数
-(LVExternalAudioConfig *)getExternalAudioConfig;

/// 打开和关闭外置音频采集功能，该参数如果打卡会自动将 SDK 内置麦克风功能禁用，由用户向 SDK 输入音频 PCM 数据。请在加入房间之前设置
/// @param enable true: 打卡，false: 关闭
-(void)enableExternalAudioInput:(bool)enable;

/// 发送外置音频数据，注：只有调用 enableExternalAudioInput 打开外部音频输入接口之后才会生效
/// @param audioFrameBuffer 音频数据
/// @param length 音频数据长度（int16_t 数组数据长度）
/// @return 发送成功与否
-(int)sendAudioFrame:(int16_t *)audioFrameBuffer length:(int)length;

/// 外部采集的视频数据
/// @param sampleBuffer 视频数据（必须是解码的视频数据）
/// @see startCapture
- (void)sendVideoFrame:(CMSampleBufferRef)sampleBuffer;

/// 外部采集的视频数据（可以随视频附加其他信息）   注：调用 startCapture 后该方法会被忽略
/// @param sampleBuffer 视频数据（必须是解码的视频数据）
/// @param sei 媒体附加数据，该数据会随视频一起发送到接收端
/// @see startCapture
- (void)sendVideoFrame:(CMSampleBufferRef)sampleBuffer sei:(NSString*)sei;

/// 控制远端音频流音量, 加入房间成功后调用
/// @param volume 音量大小（0，100）
/// @param userId 被控制的用户ID
- (void)setPlayVolume:(int)volume userId:(NSString *)userId;


/// 控制所有远端音频流音量, 加入房间成功后调用
/// @param volume 音量大小（0，100）
- (void)setPlayVolume:(int)volume;

/// 获取美颜参数管理（美颜管理器）
- (LVBeautyManager *)getLVBeautyManager;

/// 设置回声消除音频模式
/// @param mode 模式参数
- (void)setAecMode:(LVAudio3AMode)mode;

/// 设置降噪模式
/// @param mode 模式参数
- (void)setNsMode:(LVAudio3AMode)mode;

/// 设置自动增益模式
/// @param mode 模式参数
- (void)setAgcMode:(LVAudio3AMode)mode;

/// SDK 内部录制功能，目前仅支持视频编码为 H264、音频编码为 Opus 的录制，录制格式为 mkv 文件，无论录制音频还是视频请提供 mkv 后缀文件地址, eg: /tmp/recorder/hello.mkv
/// @param userId 用户 ID
/// @param path 文件路径
/// @param type 音视频录制类型
/// @return 0 : 录制成功， 其他 : 录制失败
- (int)startRecorder:(NSString*)userId path:(NSString *)path type:(LVRecorderType)type;


/// 停止录制
/// @param userId 音视频用户 ID
/// @return 0 : 停止录制成功，其他 : 停止录制失败
- (int)stopRecorder:(NSString*)userId;


@end


@interface LVRTCEngine (DEPRECATED)

/// 开始播放音乐文件及混音（声音伴奏）
/// @param filePath 文件路径
/// @param replace true: 只推动设置的本地音频文件，不传输麦克风收录的音频  false：音频文件内容将会和麦克风采集的音频流进行混音
/// @param loop 指定音频文件循环播放的次数，正整数：循环的次数 -1：无限循环
/// @return 0 ： 方法调用成功，!0：失败
-(int)startAudioMixing:(NSString *)filePath replace:(BOOL)replace loop:(int)loop;

/// 是否使用国际版本，该接口已废弃，SDK 内部会自动调度到国际或国内服务
/// @param env YES 为国际版本，NO 为国内版本
+ (void)setUseInternationalEnv:(BOOL)env;

@end


NS_ASSUME_NONNULL_END
