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

typedef struct le_particle_emitter_t
{
	renderinfo_t ri;
	void *pe;				//pointer to obj-c particle emitter object (TODO: port down to C)
	
	bool is_dynamic;
} le_particle_emitter_t;

extern le_particle_emitter_t *particle_emitter_new(void);
extern bool particle_emitter_load(const char *filename, le_particle_emitter_t *pe);
extern void particle_emitter_free(le_particle_emitter_t *pe);

extern void particle_emitter_update(le_particle_emitter_t *pe, float delta);
extern void particle_emitter_render(le_particle_emitter_t *pe);

extern void particle_emitter_start(le_particle_emitter_t *pe);
extern void particle_emitter_reset(le_particle_emitter_t *pe);
extern void particle_emitter_stop(le_particle_emitter_t *pe);

extern bool particle_emitter_is_active(le_particle_emitter_t *pe);
extern bool particle_emitter_should_handle(le_particle_emitter_t *pe);

extern double particle_emitter_get_duration(le_particle_emitter_t *pe);
extern void particle_emitter_set_duration(le_particle_emitter_t *pe, double dur);
