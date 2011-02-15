/*
 *  garbage_system.h
 *  c4
 *
 *  Created by jrk on 15/2/11.
 *  Copyright 2011 Jaroslaw Szpilewski. All rights reserved.
 *
 */

/*
 a simple garbage collector for entities.
 looks for entities with a component of the family COMP_FAMILY_GARBAGE and removes them.
 
 call garbage_system_collect() at the beginning of each frame. add a COMP_FAMILY_GARBAGE to entities you wish to be removed
 */

#pragma once
#include "elite.h"

typedef struct le_garbage_system_t
{
	le_entity_t *qry_resp_cache[512];
	le_entity_manager_t *e_manager;
} le_garbage_system_t;

extern void garbage_system_init(le_garbage_system_t *gs, le_entity_manager_t *mgr);
extern void garbage_system_shutdown(le_garbage_system_t *gs);

extern void garbage_system_collect(le_garbage_system_t *gs);

