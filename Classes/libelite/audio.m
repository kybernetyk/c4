/*
 *  audio.c
 *  c4
 *
 *  Created by jrk on 13/2/11.
 *  Copyright 2011 Jaroslaw Szpilewski. All rights reserved.
 *
 */

//note the code for resource caching is here. cocosdension does its own caching though so we won't use our code
//till we replace cocos dension with something different :]

#include "audio.h"
#include "sys_config.h"

#include <stdio.h>
#include <string.h>

#import "CocosDenshion.h"
#import "SimpleAudioEngine.h"
#import "hash.h"

static audio_id current_sound_id = 0;
static audio_id current_music_id = 0;

typedef struct audio_cache_element
{
	unsigned int hash;				//32bit hash value for the filename
	int retain_count;
	
	char filename[255];		//the audiofile filename. cocos denshion needs it for reference
} audio_cache_element;

static audio_cache_element *sound_cache = NULL;
static audio_cache_element *music_cache = NULL;
static float *sound_delays = NULL;

bool audio_init(void)
{
	[SimpleAudioEngine sharedEngine];
	
	
	current_sound_id = 0;
	current_music_id = 0;
	
	if (!sound_cache)
		sound_cache = calloc(g_sysconfig.audio_cache_size, sizeof(audio_cache_element));
	if (!music_cache)
		music_cache = calloc(g_sysconfig.audio_cache_size, sizeof(audio_cache_element));
	if (!sound_delays)
		sound_delays = calloc(g_sysconfig.audio_cache_size, sizeof(float));
	
	audio_set_sound_vol(g_sysconfig.sound_vol);
	audio_set_music_vol(g_sysconfig.music_vol);
	
	return true;
}

void audio_empty_caches(void)
{
	for (int i = 0; i < g_sysconfig.audio_cache_size; i++)
	{
		if (sound_cache[i].retain_count > 0)
		{
			sound_cache[i].retain_count = 1;
			audio_sound_release(i);
		}
	}

	for (int i = 0; i < g_sysconfig.audio_cache_size; i++)
	{
		if (music_cache[i].retain_count > 0)
		{
			music_cache[i].retain_count = 1;
			audio_music_release(i);
		}
	}
}

void audio_shutdown(void)
{
	if (sound_cache)
	{
		free(sound_cache);
		sound_cache = NULL;
	}
	
	if (music_cache)
	{
		free(music_cache);
		music_cache = NULL;
	}
	
	if (sound_delays)
	{
		free(sound_delays);
		sound_delays = NULL;
	}
}

audio_id audio_sound_load(const char *filename)
{
	NSString *fn = [NSString stringWithCString: filename encoding: NSASCIIStringEncoding];
	[[SimpleAudioEngine sharedEngine] preloadEffect: fn];
	
	unsigned int testhash = murmur_hash_2(filename, strlen(filename), 0);
	
	for (int i = 0; i < g_sysconfig.audio_cache_size; i++)
	{
		if (sound_cache[i].hash == testhash)
		{	
			sound_cache[i].retain_count++;
			return i;
		}
	}
	
	audio_id the_id = current_sound_id++;
	sound_cache[the_id].hash = testhash;
	sound_cache[the_id].retain_count = 1;
	sprintf(sound_cache[the_id].filename, "%s", filename);
	
	return the_id;
}

void audio_sound_release(audio_id sound)
{
	if (sound_cache[sound].retain_count == 0)
		return;
	
	sound_cache[sound].retain_count--;
	
	if (sound_cache[sound].retain_count <= 0)
	{
		NSString *fn = [NSString stringWithCString: sound_cache[sound].filename encoding: NSASCIIStringEncoding];
		[[SimpleAudioEngine sharedEngine] unloadEffect: fn];
		sound_cache[sound].filename[0] = 0;
		sound_cache[sound].hash = 0;
	}
}

void audio_sound_play(audio_id sound)
{
	NSString *fn = [NSString stringWithCString: sound_cache[sound].filename encoding: NSASCIIStringEncoding];
	[[SimpleAudioEngine sharedEngine] playEffect: fn];
}

audio_id audio_music_load(const char *filename)
{
	NSString *fn = [NSString stringWithCString: filename encoding: NSASCIIStringEncoding];
	[[SimpleAudioEngine sharedEngine] preloadBackgroundMusic: fn];

	unsigned int testhash = murmur_hash_2(filename, strlen(filename), 0);
	
	for (int i = 0; i < g_sysconfig.audio_cache_size; i++)
	{
		if (music_cache[i].hash == testhash)
		{	
			music_cache[i].retain_count++;
			return i;
		}
	}
	
	audio_id the_id = current_music_id++;
	music_cache[the_id].hash = testhash;
	music_cache[the_id].retain_count = 1;
	sprintf(music_cache[the_id].filename, "%s", filename);
	
	return the_id;
}

void audio_music_release(audio_id music)
{
	if (music_cache[music].retain_count == 0)
		return;

	music_cache[music].retain_count--;
	
	if (music_cache[music].retain_count <= 0)
	{
		//NSString *fn = [NSString stringWithCString: music_cache[music].filename encoding: NSASCIIStringEncoding];
		//no unloading in denshion for music? oO
		
		music_cache[music].filename[0] = 0;
		music_cache[music].hash = 0;
	}
}

void audio_music_play(audio_id music)
{
	NSString *fn = [NSString stringWithCString: music_cache[music].filename encoding: NSASCIIStringEncoding];
	[[SimpleAudioEngine sharedEngine] playBackgroundMusic: fn loop: YES];
}

void audio_music_pause(audio_id sound)
{
//	NSString *fn = [NSString stringWithCString: music_cache[music].filename encoding: NSASCIIStringEncoding];
	[[SimpleAudioEngine sharedEngine] pauseBackgroundMusic];
}

void audio_music_resume(audio_id music)
{
	[[SimpleAudioEngine sharedEngine] resumeBackgroundMusic];
}

void audio_music_stop(audio_id music)
{
	[[SimpleAudioEngine sharedEngine] stopBackgroundMusic];
}


void audio_set_sound_vol(float sfx_vol)
{
	[[SimpleAudioEngine sharedEngine] setEffectsVolume: sfx_vol];	
}

float audio_get_sound_vol(void)
{
	return [[SimpleAudioEngine sharedEngine] effectsVolume];
}

void audio_set_music_vol(float mus_vol)
{
	[[SimpleAudioEngine sharedEngine] setBackgroundMusicVolume: mus_vol];
}

float audio_get_music_vol(void)
{
	return [[SimpleAudioEngine sharedEngine] backgroundMusicVolume];
}
