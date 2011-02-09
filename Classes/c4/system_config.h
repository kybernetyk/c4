/*
 *  system_config.h
 *  c4
 *
 *  Created by jrk on 8/2/11.
 *  Copyright 2011 flux forge. All rights reserved.
 *
 */

#define C4_VERSION_STR "0.0.12a"

#define PORTRAIT 0x01
#define LANDSCAPE 0x02

//device orientation mode
//#define ORIENTATION_LANDSCAPE
#define ORIENTATION_PORTRAIT

#define DESIRED_FPS 60.0

//screen size
#ifdef ORIENTATION_PORTRAIT
	#define SCREEN_W 320.0
	#define SCREEN_H 480.0
#endif
#ifdef ORIENTATION_LANDSCAPE
	#define SCREEN_W 480.0
	#define SCREEN_H 320.0
#endif

