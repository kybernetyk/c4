/*
 *  util.c
 *  c4
 *
 *  Created by jrk on 8/2/11.
 *  Copyright 2011 flux forge. All rights reserved.
 *
 */

#include "util.h"
#include <stdio.h>


const char *path_for_file(const char *filename)
{
	NSString *relPath = [NSString stringWithFormat: @"%s",filename];
	//	NSLog(@"%@",relPath);
	
	
	NSMutableArray *imagePathComponents = [NSMutableArray arrayWithArray:[relPath pathComponents]];
	NSString *file = [imagePathComponents lastObject];
	
	[imagePathComponents removeLastObject];
	NSString *imageDirectory = [NSString pathWithComponents:imagePathComponents];
	
	NSString *ret = [[NSBundle mainBundle] pathForResource:file ofType:nil inDirectory:imageDirectory];	
	if (!ret)
	{
		NSLog(@"%s not found! path_for_file()",filename);
		abort();
	}

	const char *c = [ret cStringUsingEncoding: NSASCIIStringEncoding];
	/*The returned C string is guaranteed to be valid only until either the receiver is freed, or until the current autorelease pool is emptied, whichever occurs first. You should copy the C string or use getCString:maxLength:encoding: if it needs to store the C string beyond this time.
	 
	 from apple's docs - the autoreleasepool is drained after every frame! strdup() this if you intend to keep this around!
	 */
	return c;
}