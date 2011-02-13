/*
 *  sys_config.h
 *  libelite

 	Config system for the lib. Important: FILL THE GLOBAL STRUCTURE BEFORE CALLING ANY OTHER LE METHOD!
 
 *
 *  Created by jrk on 11/2/11.
 *  Copyright 2011 Jaroslaw Szpilewski. All rights reserved.
 *
 */


#pragma once
#include <stdbool.h>

typedef enum device_orientation_e
{
	PORTRAIT,
	LANDSCAPE
} device_orientation_e;

typedef struct sys_config_t
{
	//gfx
	double screen_w;
	double screen_h;
	double desired_fps;
	device_orientation_e orientation;
	int texture_cache_size;
	
	//sfx
	int audio_cache_size;
	double default_sound_vol;
	double default_music_vol;
	
	
} sys_config_t;
extern sys_config_t g_sysconfig;

extern bool sys_config_read_config_file(sys_config_t *cfg, const char *filename);
