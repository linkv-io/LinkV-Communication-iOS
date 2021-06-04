//
//  AppSign.h
//  LinkVRTMEngine
//
//  Created by Wing on 2021/6/3.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@interface AppSign : NSObject

+ (NSString *)your_app_id;
+ (NSString *)your_app_key;

+ (void)queryIMToken:(NSString *)uid complete:(void(^)(NSString *imToken))complete;

@end

NS_ASSUME_NONNULL_END
