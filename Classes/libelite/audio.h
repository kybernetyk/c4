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

typedef int fs_audio_id;

//
extern bool fs_audio_init(void);
extern void fs_audio_empty_caches(void);
extern void fs_audio_shutdown(void);

//
extern fs_audio_id fs_audio_sound_load(const char *filename);
extern void fs_audio_sound_play(fs_audio_id sound);
extern void fs_audio_sound_release(fs_audio_id sound);

//
extern fs_audio_id fs_audio_music_load(const char *filename);
extern void fs_audio_music_play(fs_audio_id music);
extern void fs_audio_music_pause(fs_audio_id music);
extern void fs_audio_music_resume(fs_audio_id music);
extern void fs_audio_music_stop(fs_audio_id music);
extern void fs_audio_music_release(fs_audio_id music);

//
extern void fs_audio_set_sound_vol(float sfx_vol);
extern float fs_audio_get_sound_vol(void);

extern void fs_audio_set_music_vol(float mus_vol);
extern float fs_audio_get_music_vol(void);
