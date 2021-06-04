//
//  VideoQuality.h
//  iOS_Zego
//
//  Created by Wing on 2020/8/4.
//  Copyright © 2020 Wing. All rights reserved.
//

#import <UIKit/UIKit.h>

NS_ASSUME_NONNULL_BEGIN

@interface VideoQuality : NSObject

/// 视频总发送包数量
@property (nonatomic, assign) int       videoSentPackets;
/// 视频输入帧率（视频帧率）
@property (nonatomic, assign) int       videoFps;
/// 视频码率
@property (nonatomic, assign) int       videoBitratebps;
/// 音频码率
@property (nonatomic, assign) int       audioBitratebps;
/// 音频包 RTT，单位 ms
@property (nonatomic, assign) int       audioRtt;
/// 视频包 RTT，单位 ms
@property (nonatomic, assign) int       videoRtt;
/// 累计视频丢包百分比
@property (nonatomic, assign) int       videoLostPercent;

/// 累计音频丢包百分比
@property (nonatomic, assign) int       audioLostPercent;

#pragma mark - Only LinkV
/** 以下参数只有在SDK未LinkV的时候才有值  */
/// 视频编码帧数量
@property (nonatomic, assign) int       videoEncodeFrames;

/// 视频总发送字节数
@property (nonatomic, assign) long long videosentKbytes;

/// 视频丢包总数
@property (nonatomic, assign) int       videoLostPackets;

/// 音频丢包总数
@property (nonatomic, assign) int       audioLostPackets;

/// 当前可用的发送带宽
@property (nonatomic, assign) int       availableSendBandwidth;

/// 视频编码耗时（单位毫米），仅对视频发送方有效
@property (nonatomic, assign) int       videoAvgEncodeCostMs;

/// 推流：编码视频宽，拉流：输入视频帧宽
@property (nonatomic, assign) int       frameWidth;

/// 推流：编码视频高，拉流：输入视频帧高
@property (nonatomic, assign) int       frameHeight;

@end

NS_ASSUME_NONNULL_END
