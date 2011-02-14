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
	LE_PORTRAIT,
	LE_LANDSCAPE
} device_orientation_e;

typedef struct sys_config_t
{
	//gfx
	double screen_w;						//viewport width in pixels
	double screen_h;						//viewport height in pixels
	double screen_scale;					//debug scaling
	double desired_fps;						//the desired fps rate the game should run on
	device_orientation_e orientation;		//device orientation lock
	int texture_cache_size;					//size of the texture cache (in elements)
	
	//sfx
	int audio_cache_size;					//size of audio resource cache (in elements)
	double sound_vol;				//default sfx volume
	double music_vol;				//default music volume
	
	//entity
	int entity_pool_size;
	int components_per_entity;
	
} sys_config_t;
extern sys_config_t g_sysconfig;

extern bool sys_config_read_config_file(sys_config_t *cfg, const char *filename);
