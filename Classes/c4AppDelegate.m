//
//  c4AppDelegate.m
//  c4
//
//  Created by jrk on 8/2/11.
//  Copyright 2011 Jaroslaw Szpilewski. All rights reserved.
//

#import "c4AppDelegate.h"
#import "c4ViewController.h"
#include "game.h"

@implementation c4AppDelegate

@synthesize window;
@synthesize viewController;

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions
{
    [self.window addSubview:self.viewController.view];
    return YES;
}

- (void)applicationWillResignActive:(UIApplication *)application
{
	game_did_enter_background();
    [self.viewController stopAnimation];
}

- (void)applicationDidBecomeActive:(UIApplication *)application
{
	game_did_become_active();
    [self.viewController startAnimation];
}

- (void)applicationWillTerminate:(UIApplication *)application
{
    [self.viewController stopAnimation];
}

- (void)applicationDidEnterBackground:(UIApplication *)application
{
    // Handle any background procedures not related to animation here.
}

- (void)applicationWillEnterForeground:(UIApplication *)application
{
    // Handle any foreground procedures not related to animation here.
}

- (void)dealloc
{
    [viewController release];
    [window release];
    
    [super dealloc];
}

@end
