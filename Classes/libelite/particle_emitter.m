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

bool particle_emitter_load(const char *filename, le_particle_emitter_t *pe)
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

void particle_emitter_render(le_particle_emitter_t *pe)
{
	glPushMatrix();
	glTranslatef(0.0, 0.0, pe->ri.zval);		//the pe system will do further transforms
	[(ParticleEmitter *)pe->pe renderParticles];
	glPopMatrix();
}

void particle_emitter_update(le_particle_emitter_t *pe, float delta)
{
	Vector2f pos;
	pos.x = pe->ri.pos.x;
	pos.y = pe->ri.pos.y;

	[(ParticleEmitter *)pe->pe setSourcePosition: pos];
	[(ParticleEmitter *)pe->pe updateWithDelta: delta];
}


void particle_emitter_free(le_particle_emitter_t *pe)
{
	if (pe->pe)
	{
		[(ParticleEmitter *)pe->pe release];
		pe->pe = nil;
	}
}


void particle_emitter_start(le_particle_emitter_t *pe)
{
	[(ParticleEmitter *)pe->pe startParticleEmitter];
}

void particle_emitter_reset(le_particle_emitter_t *pe)
{
	[(ParticleEmitter *)pe->pe reset];
}

void particle_emitter_stop(le_particle_emitter_t *pe)
{
	[(ParticleEmitter *)pe->pe stopParticleEmitter];
}

bool particle_emitter_is_active(le_particle_emitter_t *pe)
{
	return [(ParticleEmitter *)pe->pe active];	
}

bool particle_emitter_should_handle(le_particle_emitter_t *pe)
{
	if (![(ParticleEmitter *)pe->pe active] && [(ParticleEmitter *)pe->pe particleCount] == 0)
		return false;
	return true;
}

double particle_emitter_get_duration(le_particle_emitter_t *pe)
{
	return [(ParticleEmitter *)pe->pe duration];
}

void particle_emitter_set_duration(le_particle_emitter_t *pe, double dur)
{
	[(ParticleEmitter *)pe->pe setDuration: dur];
}
