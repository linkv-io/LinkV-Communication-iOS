//
//  LiveVC.h
//  iOS_Zego
//
//  Created by Wing on 2020/8/3.
//  Copyright © 2020 Wing. All rights reserved.
//

#import <UIKit/UIKit.h>

NS_ASSUME_NONNULL_BEGIN

@interface LiveVC : UIViewController

@property (nonatomic, strong) NSString *roomId;
@property (nonatomic, assign) BOOL isHost;
@property (nonatomic, copy) NSString *uid;

@end

NS_ASSUME_NONNULL_END
