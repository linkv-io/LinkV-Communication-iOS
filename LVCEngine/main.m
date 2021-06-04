//
//  main.m
//  LinkVRTMEngine
//
//  Created by Wing on 2021/6/3.
//

#import <UIKit/UIKit.h>
#import "LVAppDelegate.h"

int main(int argc, char * argv[]) {
    NSString * appDelegateClassName;
    @autoreleasepool {
        // Setup code that might create autoreleased objects goes here.
        appDelegateClassName = NSStringFromClass([LVAppDelegate class]);
    }
    return UIApplicationMain(argc, argv, nil, appDelegateClassName);
}
