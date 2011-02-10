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

typedef struct particle_emitter_t
{
	renderinfo_t ri;
	void *pe;				//pointer to obj-c particle emitter object (TODO: port down to C)
} particle_emitter_t;

extern bool particle_emitter_load(const char *filename, particle_emitter_t *pe);
extern void particle_emitter_free(particle_emitter_t *pe);

extern void particle_emitter_update(particle_emitter_t *pe, float delta);
extern void particle_emitter_render(particle_emitter_t *pe);

extern void particle_emitter_start(particle_emitter_t *pe);
extern void particle_emitter_reset(particle_emitter_t *pe);
extern void particle_emitter_stop(particle_emitter_t *pe);

extern bool particle_emitter_is_active(particle_emitter_t *pe);
extern bool particle_emitter_should_handle(particle_emitter_t *pe);

extern double particle_emitter_get_duration(particle_emitter_t *pe);
extern void particle_emitter_set_duration(particle_emitter_t *pe, double dur);
