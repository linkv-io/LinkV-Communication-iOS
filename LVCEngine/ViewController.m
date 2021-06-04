//
//  ViewController.m
//  iOS_Zego
//
//  Created by Wing on 2020/8/3.
//  Copyright © 2020 Wing. All rights reserved.
//

#import "ViewController.h"
#import "LiveVC.h"
#import "LVCEngine.h"
#import "MBProgressHUD+DS.h"
#import <AdSupport/ASIdentifierManager.h>
#import "AppSign.h"

@interface ViewController ()<LVIMModuleEventDelegate, RTCCallbackDelegate, LVIMReceiveMessageDelegate>

@property (weak, nonatomic) IBOutlet UITextField *roomField;

@property (nonatomic, strong) LVCEngine *engine;
@property (nonatomic, copy) NSString *uid;

@property (nonatomic, assign) BOOL isIMAuthed;
@property (nonatomic, assign) BOOL isRTCAuthed;

@property (nonatomic, assign) BOOL isQueryingIMToken;

@end

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];

    self.roomField.text = @"1524";
    self.uid = [[[[ASIdentifierManager sharedManager] advertisingIdentifier] UUIDString] stringByReplacingOccurrencesOfString:@"-" withString:@""];
    
//    self.uid = [NSString stringWithFormat:@"%d", arc4random_uniform(10000)];
    NSLog(@"[Wing] start loginRoom uid = %@", self.uid);
    
    [self initSDK];
}

- (void)initSDK {
    __weak typeof(self) weakSelf = self;
    self.engine = [LVCEngine createEngineWithAppId:[AppSign your_app_id] appKey:[AppSign your_app_key] isTestEnv:NO completion:^(NSInteger code) {
        [weakSelf authComplete:code];
    }];
    
    if (!self.isIMAuthed) {
        [self.engine setIMGlobalReceiveMessageDelegate:self];
        [self.engine loginIM:self.uid delegate:self];
    }
}

- (IBAction)startLiveBtnClick:(UIButton *)sender {
    if (!self.roomField.hasText) return;
    
    if (!self.isIMAuthed || !self.isRTCAuthed) {
        [self initSDK];
        return;
    }
    
    LiveVC *liveVC = [LiveVC new];
    liveVC.isHost = YES;
    liveVC.roomId = self.roomField.text;
    liveVC.uid = self.uid;
    [self.navigationController pushViewController:liveVC animated:YES];
}

- (IBAction)watchLiveBtnClick:(UIButton *)sender {
    if (!self.roomField.hasText) return;
    
    if (!self.isIMAuthed || !self.isRTCAuthed) {
        [self initSDK];
        return;
    }
    
    LiveVC *liveVC = [LiveVC new];
    liveVC.isHost = NO;
    liveVC.roomId = self.roomField.text;
    liveVC.uid = self.uid;
    [self.navigationController pushViewController:liveVC animated:YES];
}

- (void)touchesBegan:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event {
    [self.view endEditing:YES];
}

#pragma mark - LVIMModuleEventDelegate
/**
 @brief 请求token的事件
 **/
-(void)onQueryIMToken {
    NSLog(@"[Wing] onQueryIMToken thread = %@", [NSThread currentThread]);
    
    // 正式接入应该是：你的客户端请求服务端获取IM Token， 你的服务端请求IM的服务端获取IM Token
    if (self.isQueryingIMToken) return;
    self.isQueryingIMToken = YES;
    __weak typeof(self) weakSelf = self;
    [AppSign queryIMToken:self.uid complete:^(NSString * _Nonnull imToken) {
        [[LVIMSDK sharedInstance] setIMToken:weakSelf.uid token:imToken];
        weakSelf.isQueryingIMToken = NO;
    }];
}

/**
 @brief 连接成功事件(暂不提供)
 @param context 状态信息
 **/
-(void)onIMConnected:(void*)context {
    NSLog(@"[Wing] context = %@", context);
}

/**
 @brief 连接断开事件(暂不提供)
 **/
-(void)onIMLosted {
    NSLog(@"[Wing] onIMLosted");
}

/**
 @brief 认证成功事件
 @param uid 用户ID
 @param token token (IM)
 @param unReadMsgSize 未读消息数量(粗略)
 **/
-(void)onIMAuthSuccessed:(NSString*)uid token:(NSString*)token unReadMsgSize:(int)unReadMsgSize {
    NSLog(@"[Wing] onIMAuthSuccessed uid = %@, token = %@", uid, token);
    
    self.isIMAuthed = YES;
    dispatch_async(dispatch_get_main_queue(), ^{
        [MBProgressHUD showSuccess:@"onIMAuthSuccessed"];
    });
}

/**
 @brief 认证失败事件
 @param uid 用户ID
 @param token 认证码(im)
 @param ecode 错误码
 @param rcode 响应码
 @param expired 是否是过期了
 **/
-(void)onIMAuthFailed:(NSString*)uid token:(NSString*)token ecode:(int)ecode rcode:(int)rcode expired:(BOOL)expired {
    NSLog(@"[Wing] onIMAuthFailed uid = %@, token = %@, errorCode = %@, rCode = %@", uid, token, @(ecode), @(rcode));
    
    dispatch_async(dispatch_get_main_queue(), ^{
        [MBProgressHUD showError:@"onIMAuthFailed" toView:self.view];
    });
}

/**
 @brief 认证过期
 @param uid 用户ID
 @param token 认证码(im)
 @param owner 所属者
 **/
-(void)onIMTokenExpired:(NSString*)uid token:(NSString*)token owner:(NSString*)owner {
    NSLog(@"[Wing] onIMTokenExpired uid = %@, token = %@", uid, token);
    
    dispatch_async(dispatch_get_main_queue(), ^{
        [MBProgressHUD showError:@"onIMTokenExpired" toView:self.view];
    });
}

#pragma mark - RTCCallbackDelegate
- (void)authComplete:(NSInteger)errorCode {
    NSLog(@"[Wing] authComplete errorCode = %@", @(errorCode));
    
    self.isRTCAuthed = errorCode == 0;
    [MBProgressHUD showSuccess:@"RTC Authed"];
}

#pragma mark - LVIMReceiveMessageDelegate 私信消息
- (BOOL)onIMReceiveMessageFilter:(int32_t)diytype fromid:(const char *)fromid toid:(const char *)toid msgtype:(const char *)msgtype content:(const char *)content waitings:(int)waitings packetSize:(int)packetSize waitLength:(int)waitLength bufferSize:(int)bufferSize {
    return NO;
}

- (int)onIMReceiveMessageHandler:(NSString *)owner immsg:(LVIMMessage *)immsg waitings:(int)waitings packetSize:(int)packetSize waitLength:(int)waitLength bufferSize:(int)bufferSize {
    
    NSString *content = [[NSString alloc] initWithData:immsg.mContent encoding:NSUTF8StringEncoding];
    NSLog(@"[Wing] onIMReceiveMessageHandler private content = %@", content);
    dispatch_async(dispatch_get_main_queue(), ^{
        [MBProgressHUD showSuccess:content];
    });
    
    return 0;
}


@end
