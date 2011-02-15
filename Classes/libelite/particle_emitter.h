/*
 *  particle_emitter.h
 *  c4
 *
 *  Created by jrk on 10/2/11.
 *  Copyright 2011 Jaroslaw Szpilewski. All rights reserved.
 *
 */
#pragma once
#include <stdbool.h>

#include "renderinfo.h"
#include "texture.h"

//TODO: rewrite the particle emitter in C so we can be portable to non apple platforms!
//right now this file is just a stub to call obj-c code from C ...

typedef struct fs_particle_emitter_t
{
	fs_renderinfo_t ri;
	void *pe;				//pointer to obj-c particle emitter object (TODO: port down to C)
	
	bool is_dynamic;
} fs_particle_emitter_t;

extern fs_particle_emitter_t *particle_emitter_new(void);
extern bool fs_particle_emitter_load(const char *filename, fs_particle_emitter_t *pe);
extern void fs_particle_emitter_free(fs_particle_emitter_t *pe);

extern void fs_particle_emitter_update(fs_particle_emitter_t *pe, float delta);
extern void fs_particle_emitter_render(fs_particle_emitter_t *pe);

extern void fs_particle_emitter_start(fs_particle_emitter_t *pe);
extern void fs_particle_emitter_reset(fs_particle_emitter_t *pe);
extern void fs_particle_emitter_stop(fs_particle_emitter_t *pe);

extern bool fs_particle_emitter_is_active(fs_particle_emitter_t *pe);
extern bool fs_particle_emitter_should_handle(fs_particle_emitter_t *pe);

extern double fs_particle_emitter_get_duration(fs_particle_emitter_t *pe);
extern void fs_particle_emitter_set_duration(fs_particle_emitter_t *pe, double dur);
