/*
 *  c4_config.h
 *  c4

 	Config system for the framework. Important: FILL THE GLOBAL STRUCTURE BEFORE CALLING ANY OTHER c4 METHOD!
 
 *
 *  Created by jrk on 11/2/11.
 *  Copyright 2011 Jaroslaw Szpilewski. All rights reserved.
 *
 */


#pragma once
#include <stdbool.h>

typedef enum c4_orientation_e
{
	PORTRAIT,
	LANDSCAPE
} c4_orientation_e;

typedef struct c4_config_t
{
	//gfx
	double screen_w;
	double screen_h;
	double desired_fps;
	c4_orientation_e orientation;
	int texture_cache_size;
	
	//sfx
	int audio_cache_size;
	double default_sound_vol;
	double default_music_vol;
	
	
} c4_config_t;
extern c4_config_t g_sysconfig;

extern bool c4_config_read_config_file(c4_config_t *cfg, const char *filename);

//defines for easy accessing the global config
#define ORIENTATION g_sysconfig.orientation
#define DESIRED_FPS g_sysconfig.desired_fps
#define SCREEN_W g_sysconfig.screen_w
#define SCREEN_H g_sysconfig.screen_h

#define MAX_TEXTURES g_sysconfig.texture_cache_size
