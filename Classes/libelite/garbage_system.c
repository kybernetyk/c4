/*
 *  garbage_system.c
 *  c4
 *
 *  Created by jrk on 15/2/11.
 *  Copyright 2011 Jaroslaw Szpilewski. All rights reserved.
 *
 */

#include "garbage_system.h"
#include <memory.h>

void garbage_system_init(le_garbage_system_t *gs, le_entity_manager_t *mgr)
{
	memset(gs->qry_resp_cache,0x00,sizeof(le_entity_t *)*512);
	gs->e_manager = mgr;
}

void garbage_system_shutdown(le_garbage_system_t *gs)
{
	
}

void garbage_system_collect(le_garbage_system_t *gs)
{
	memset(gs->qry_resp_cache,0x00,sizeof(le_entity_t *)*512);
	
	
	size_t res = em_get_entities_with_component(gs->e_manager,COMP_FAMILY_GARBAGE, gs->qry_resp_cache, 512);
	
	le_entity_t *current_entity = NULL;
	for (size_t i = 0; i < res; i++)
	{
		current_entity = gs->qry_resp_cache[i];
		em_remove_entity(gs->e_manager, current_entity);
	}		
}

