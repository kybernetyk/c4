/*
 *  particle_emitter.c
 *  c4
 *
 *  Created by jrk on 10/2/11.
 *  Copyright 2011 Jaroslaw Szpilewski. All rights reserved.
 *
 */

#include "particle_emitter.h"
#include <stdlib.h>
#include <stdio.h>
#include "renderinfo.h"
#import "ParticleEmitter.h"

fs_particle_emitter_t *particle_emitter_new(void)
{
	fs_particle_emitter_t *ret = calloc(1, sizeof(fs_particle_emitter_t));
	ret->is_dynamic = true;
	
	return ret;
}

bool fs_particle_emitter_load(const char *filename, fs_particle_emitter_t *pe)
{
	ParticleEmitter *pm = [[ParticleEmitter alloc] initParticleEmitterWithFile: [NSString stringWithCString: filename
																								   encoding: NSASCIIStringEncoding]];
	if (!pm)
		return false;
	
	pe->pe = pm;
	pe->ri.alpha = 1.0;
	pe->ri.rot_a = 0.0;
	pe->ri.scale = vec2d_make(1.0, 1.0);
	pe->ri.zval = 0.0;
	pe->ri.size = size2d_make(0,0);
	pe->ri.anchor_point = vec2d_make(0.5, 0.5);
	
	return true;
}

void fs_particle_emitter_render(fs_particle_emitter_t *pe)
{
	glPushMatrix();
	glTranslatef(0.0, 0.0, pe->ri.zval);		//the pe system will do further transforms
	[(ParticleEmitter *)pe->pe renderParticles];
	glPopMatrix();
}

void fs_particle_emitter_update(fs_particle_emitter_t *pe, float delta)
{
	Vector2f pos;
	pos.x = pe->ri.pos.x;
	pos.y = pe->ri.pos.y;

	[(ParticleEmitter *)pe->pe setSourcePosition: pos];
	[(ParticleEmitter *)pe->pe updateWithDelta: delta];
}


void fs_particle_emitter_free(fs_particle_emitter_t *pe)
{
	if (pe->pe)
	{
		[(ParticleEmitter *)pe->pe release];
		pe->pe = nil;
	}
	
	if (pe->is_dynamic)
		free(pe);
}


void fs_particle_emitter_start(fs_particle_emitter_t *pe)
{
	[(ParticleEmitter *)pe->pe startParticleEmitter];
}

void fs_particle_emitter_reset(fs_particle_emitter_t *pe)
{
	[(ParticleEmitter *)pe->pe reset];
}

void fs_particle_emitter_stop(fs_particle_emitter_t *pe)
{
	[(ParticleEmitter *)pe->pe stopParticleEmitter];
}

bool fs_particle_emitter_is_active(fs_particle_emitter_t *pe)
{
	return [(ParticleEmitter *)pe->pe active];	
}

bool fs_particle_emitter_should_handle(fs_particle_emitter_t *pe)
{
	if (![(ParticleEmitter *)pe->pe active] && [(ParticleEmitter *)pe->pe particleCount] == 0)
		return false;
	return true;
}

double fs_particle_emitter_get_duration(fs_particle_emitter_t *pe)
{
	return [(ParticleEmitter *)pe->pe duration];
}

void fs_particle_emitter_set_duration(fs_particle_emitter_t *pe, double dur)
{
	[(ParticleEmitter *)pe->pe setDuration: dur];
}
