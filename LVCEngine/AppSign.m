//
//  AppSign.m
//  LinkVRTMEngine
//
//  Created by Wing on 2021/6/3.
//

#import "AppSign.h"

@implementation AppSign

+ (NSString *)your_app_id {
    NSAssert(false, @"请访问【https://doc-zh.linkv.sg/platform/info/quick_start】获取您的appId和appSecret");
    return nil;
}

+ (NSString *)your_app_key {
    NSAssert(false, @"请访问【https://doc-zh.linkv.sg/platform/info/quick_start】获取您的appId和appSecret");
    return nil;
}

+ (void)queryIMToken:(NSString *)uid complete:(void(^)(NSString *imToken))complete {
    if (!complete) return;
    
    NSMutableURLRequest *requestM = [NSMutableURLRequest requestWithURL:[NSURL URLWithString:@"https://catchu-im-api.fusionv.com/api/rest/getWebimTestToken"]];
    [requestM setValue:@"1234" forHTTPHeaderField:@"userid"];
    
    [[[NSURLSession sharedSession] dataTaskWithRequest:requestM completionHandler:^(NSData * _Nullable data, NSURLResponse * _Nullable response, NSError * _Nullable error) {
        if (error || !data) {
            complete(@"hamiabe5024f32affc83ea5f5d71b38d");
            return;
        }
        
        NSDictionary *dict = [NSJSONSerialization JSONObjectWithData:data options:0 error:&error];
        if (error || !dict || ![dict isKindOfClass:[NSDictionary class]] || !dict[@"data"]) {
            complete(@"hamiabe5024f32affc83ea5f5d71b38d");
            return;
        }
        
        complete(dict[@"data"]);
    }] resume];
}

@end
