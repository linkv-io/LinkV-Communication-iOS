//
//  LiveVC.m
//  iOS_Zego
//
//  Created by Wing on 2020/8/3.
//  Copyright © 2020 Wing. All rights reserved.
//

#import "LiveVC.h"
#import "LinkVRTCEngine.h"
#import "LVRTMEngine.h"
#import "MBProgressHUD+DS.h"

#ifdef DEBUG
#define DLog(...) NSLog(__VA_ARGS__)
#else
#define DLog(...) /* */
#endif

@interface LiveVC ()<RoomCallbackProtocl>

@property (nonatomic, strong) LVRTMEngine *engine;
@property (weak, nonatomic) IBOutlet UIView *meView;
@property (weak, nonatomic) IBOutlet UIView *otherView;
@property (weak, nonatomic) IBOutlet UILabel *meLabel;
@property (weak, nonatomic) IBOutlet UILabel *otherLabel;

@property (nonatomic, copy) NSString *publishSizeStr;
@property (nonatomic, copy) NSString *playSizeStr;
@property (nonatomic, copy) NSString *otherUid;

@end

@implementation LiveVC

- (void)viewDidLoad {
    [super viewDidLoad];

    [self initSDK];
    [self loginRoom];
    
    self.title = [NSString stringWithFormat:@"%@ %@", [self.engine getSolutionName], self.roomId];
}

- (void)loginRoom {
    [self.engine loginRoom:self.uid roomId:self.roomId isHost:self.isHost delegate:self];
    
    [MBProgressHUD showMessage:@"登录房间..."];
}

- (void)initSDK {
    self.engine = [LVRTMEngine shared];
}

#pragma mark - RTCCallbackDelegate
- (void)onAddRemoterUser:(NSString *)uid {
    self.otherUid = uid;
    [self.engine startPlayingStream:uid inView:self.otherView];
    
    self.otherLabel.hidden = NO;
}

- (void)onRemoteLeave:(NSString *)uid {
    [self.engine stopPlayingStream:uid];
    self.otherUid = nil;
    self.playSizeStr = nil;
    
    self.otherLabel.hidden = YES;
}

/**
 * 被踢出房间或断开房间链接
 * @param errorCode 错误码，参见Zego或LinkV错误码
 * @param roomId 房间id
 */
- (void)onRoomDisconnect:(int)errorCode roomId:(NSString *)roomId {
    DLog(@"[Wing] onExitRoom roomId = %@ errorCode = %@", roomId, @(errorCode));
    
    [MBProgressHUD showError:@"断开房间"];
}

- (void)onRoomConnected:(NSString *)roomId {
    DLog(@"[Wing] onLoginRoomSucceed thread %@", [NSThread currentThread]);
    
    if (self.isHost) {
        [self.engine startPreview:self.meView];
        [self.engine startPublishing];
    }
    
    [MBProgressHUD hideHUD];
}

- (void)onVideoSizeChanged:(NSString *)uid width:(int)width height:(int)height {
    if ([uid isEqualToString:self.uid]) {
        self.publishSizeStr = [NSString stringWithFormat:@"%@\n%@x%@",uid, @(width), @(height)];
    } else {
        self.playSizeStr = [NSString stringWithFormat:@"%@\n%@x%@",uid, @(width), @(height)];
    }
}

- (void)onPublishStreamQualityUpdate:(NSString *)uid quality:(VideoQuality *)quality {
    if (self.publishSizeStr) {
        self.meLabel.text = [NSString stringWithFormat:@"%@ / %@", self.publishSizeStr, @(quality.videoFps)];
    }
}

- (void)onRemoteQualityUpdate:(NSString *)uid quality:(VideoQuality *)quality {
    if (self.playSizeStr) {
        self.otherLabel.text = [NSString stringWithFormat:@"%@ / %@", self.playSizeStr, @(quality.videoFps)];
    }
}

#pragma mark - LVIMReceiveMessageDelegate
- (int)onRoomMessageReceive:(LVIMMessage *)msg {
    NSString *content = [[NSString alloc] initWithData:msg.mContent encoding:NSUTF8StringEncoding];
    NSLog(@"[Wing] onRoomMessageReceive content = %@", content);
    
    dispatch_async(dispatch_get_main_queue(), ^{
        [MBProgressHUD showSuccess:content toView:self.view];
    });
    
    return 0;
}

#pragma mark - BtnAction
- (IBAction)sendRoomMsgBtnClick:(UIButton *)sender {
    NSLog(@"[Wing] sendRoomMessage start");
    
    [self.engine sendRoomMessage:self.roomId content:@"Test room msg" complete:^(int ecode, int rcode, int64_t lvsgid, int64_t smsgid, int64_t stime, LVIMMessage *msg) {
        NSLog(@"[Wing] send room msg end ecode = %@ rcode = %@", @(ecode), @(rcode));
    }];
}

#pragma mark - Dealloc
- (void)dealloc {
    NSLog(@"[Wing] %@ dealloc🍀🍀🍀🍀", self);
    
    [self.engine logoutRoom];
}

@end
