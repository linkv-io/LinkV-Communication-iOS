//
//  MBProgressHUD+NJ.m
//  NJWisdomCard
//
//  Created by apple on 15/8/25.
//  Copyright (c) 2015年 Weconex. All rights reserved.
//
#import "MBProgressHUD.h"
@implementation MBProgressHUD (DS)

/**
 *  显示信息
 *
 *  @param text 信息内容
 *  @param icon 图标
 *  @param view 显示的视图
 */
+ (void)show:(NSString *)text icon:(NSString *)icon view:(UIView *)view
{
    if (view == nil) view = [[UIApplication sharedApplication].windows lastObject];
    
    if ([[[UIApplication sharedApplication].windows lastObject] isKindOfClass:NSClassFromString(@"UITextEffectsWindow")]) {
        view =  [[UIApplication sharedApplication].windows firstObject];
    }
    
    
    // 快速显示一个提示信息
    MBProgressHUD *hud = [MBProgressHUD showHUDAddedTo:view animated:YES];
    hud.labelText = text;
    // 设置图片
    hud.customView = [[UIImageView alloc] initWithImage:[UIImage imageNamed:[NSString stringWithFormat:@"MBProgressHUD.bundle/%@", icon]]];
    // 再设置模式
    hud.mode = MBProgressHUDModeCustomView;
    
    hud.userInteractionEnabled = true;
    
    // 隐藏时候从父控件中移除
    hud.removeFromSuperViewOnHide = YES;
    
    // 1秒之后再消失
    [hud hide:YES afterDelay:0.75];
}

/**
 *  显示成功信息
 *
 *  @param success 信息内容
 */
+ (void)showSuccess:(NSString *)success
{
    [MBProgressHUD hideHUD];
    [self showSuccess:success toView:nil];
}

/**
 *  显示成功信息
 *
 *  @param success 信息内容
 *  @param view    显示信息的视图
 */
+ (void)showSuccess:(NSString *)success toView:(UIView *)view
{
    [self show:success icon:@"success.png" view:view];
}

/**
 *  显示错误信息
 *
 */
+ (void)showError:(NSString *)error
{
    [MBProgressHUD hideHUD];
    [self showError:error toView:nil];
}

/**
 *  显示错误信息
 *
 *  @param error 错误信息内容
 *  @param view  需要显示信息的视图
 */

+ (void)showError:(NSString *)error toView:(UIView *)view{
    [self show:error icon:@"error.png" view:view];
}

/**
 *  显示错误信息
 *
 *  @param message 信息内容
 *
 *  @return 直接返回一个MBProgressHUD，需要手动关闭
 */
+ (MBProgressHUD *)showMessage:(NSString *)message
{
    [MBProgressHUD hideHUD];
    return [self showMessage:message toView:nil];
}

+ (MBProgressHUD *)showMessageDim:(NSString *)message dim:(BOOL)dim
{
    [MBProgressHUD hideHUD];
    return [self showMessage:message toView:nil andDim:dim];
}

/**
 *  显示一些信息
 *
 *  @param message 信息内容
 *  @param view    需要显示信息的视图
 *
 *  @return 直接返回一个MBProgressHUD，需要手动关闭
 */
+ (MBProgressHUD *)showMessage:(NSString *)message toView:(UIView *)view {
    if (view == nil) view = [[UIApplication sharedApplication].windows lastObject];
    
    if ([[[UIApplication sharedApplication].windows lastObject] isKindOfClass:NSClassFromString(@"UITextEffectsWindow")]) {
        view =  [[UIApplication sharedApplication].windows firstObject];
    }
    
    // 快速显示一个提示信息
    MBProgressHUD *hud = [MBProgressHUD showHUDAddedTo:view animated:YES];
    hud.labelText = message;
    hud.userInteractionEnabled = true;
    // 隐藏时候从父控件中移除
    hud.removeFromSuperViewOnHide = YES;
    // YES代表需要蒙版效果
    hud.dimBackground = NO;
    return hud;
}


+ (MBProgressHUD *)showMessage:(NSString *)message toView:(UIView *)view andDim:(BOOL)dim {
    if (view == nil) view = [[UIApplication sharedApplication].windows lastObject];
    
    if ([[[UIApplication sharedApplication].windows lastObject] isKindOfClass:NSClassFromString(@"UITextEffectsWindow")]) {
        view =  [[UIApplication sharedApplication].windows firstObject];
    }
    // 快速显示一个提示信息
    MBProgressHUD *hud = [MBProgressHUD showHUDAddedTo:view animated:YES];
    hud.labelText = message;
    hud.userInteractionEnabled = true;
    // 隐藏时候从父控件中移除
    hud.removeFromSuperViewOnHide = YES;
    // YES代表需要蒙版效果
    hud.dimBackground = dim;
    return hud;
}


+ (MBProgressHUD *)showMessageEnabled:(NSString *)message
{
    return [self showMessageEnabled:message toView:nil];
}

+ (MBProgressHUD *)showMessageEnabled:(NSString *)message toView:(UIView *)view {
    if (view == nil) view = [[UIApplication sharedApplication].windows lastObject];
    // 快速显示一个提示信息
    MBProgressHUD *hud = [MBProgressHUD showHUDAddedTo:view animated:YES];
    hud.labelText = message;
    hud.userInteractionEnabled = YES;
    // 隐藏时候从父控件中移除
    hud.removeFromSuperViewOnHide = YES;
    // YES代表需要蒙版效果
    hud.dimBackground = NO;
    return hud;
}




/**
 *  手动关闭MBProgressHUD
 */
+ (void)hideHUD
{
    [self hideHUDForView:nil];
}

/**
 *  手动关闭MBProgressHUD
 *
 *  @param view    显示MBProgressHUD的视图
 */
+ (void)hideHUDForView:(UIView *)view
{
    if (view == nil) view = [[UIApplication sharedApplication].windows lastObject];
    
    if ([[[UIApplication sharedApplication].windows lastObject] isKindOfClass:NSClassFromString(@"UITextEffectsWindow")]) {
        view =  [[UIApplication sharedApplication].windows firstObject];
    }
    [self hideHUDForView:view animated:YES];
}

@end
