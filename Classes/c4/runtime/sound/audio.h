/*
 *  audio.h
 *  c4
 *
 *  Created by jrk on 13/2/11.
 *  Copyright 2011 Jaroslaw Szpilewski. All rights reserved.
 *
 */

#pragma once
#include <stdbool.h>

typedef int audio_id;

//
extern bool audio_init(void);
extern void audio_empty_caches(void);
extern void audio_shutdown(void);

//
extern audio_id audio_sound_load(const char *filename);
extern void audio_sound_play(audio_id sound);
extern void audio_sound_release(audio_id sound);

//
extern audio_id audio_music_load(const char *filename);
extern void audio_music_play(audio_id music);
extern void audio_music_pause(audio_id music);
extern void audio_music_resume(audio_id music);
extern void audio_music_stop(audio_id music);
extern void audio_music_release(audio_id music);

//
extern void audio_set_sound_vol(float sfx_vol);
extern float audio_get_sound_vol(void);

extern void audio_set_music_vol(float mus_vol);
extern float audio_get_music_vol(void);
