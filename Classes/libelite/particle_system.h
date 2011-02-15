/*
 *  particle_system.h
 *  c4
 *
 *  Created by jrk on 14/2/11.
 *  Copyright 2011 Jaroslaw Szpilewski. All rights reserved.
 *
 */

#pragma once
#include "elite.h"

typedef struct le_particle_system_t
{
	le_entity_t **qry_resp_cache;
	le_entity_manager_t *e_manager;

} le_particle_system_t;

extern void particle_system_init(le_particle_system_t *ps, le_entity_manager_t *mgr);
extern void particle_system_shutdown(le_particle_system_t *ps);

extern void particle_system_update(le_particle_system_t *ps, double dt);

