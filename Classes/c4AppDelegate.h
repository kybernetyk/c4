//
//  c4AppDelegate.h
//  c4
//
//  Created by jrk on 8/2/11.
//  Copyright 2011 Jaroslaw Szpilewski. All rights reserved.
//

#import <UIKit/UIKit.h>

@class c4ViewController;

@interface c4AppDelegate : NSObject <UIApplicationDelegate> {
    UIWindow *window;
    c4ViewController *viewController;
}

@property (nonatomic, retain) IBOutlet UIWindow *window;
@property (nonatomic, retain) IBOutlet c4ViewController *viewController;

@end

