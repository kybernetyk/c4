/*
 *  particle_system.c
 *  c4
 *
 *  Created by jrk on 14/2/11.
 *  Copyright 2011 Jaroslaw Szpilewski. All rights reserved.
 *
 */

#include "particle_system.h"
#include <memory.h>

void particle_system_init(le_particle_system_t *ps, le_entity_manager_t *mgr)
{
	memset(ps->qry_resp_cache,0x00,sizeof(le_entity_t *)*512);
	ps->e_manager = mgr;
}

void particle_system_shutdown(le_particle_system_t *s)
{
	
}

void particle_system_update(le_particle_system_t *ps, double dt)
{
	memset(ps->qry_resp_cache,0x00,sizeof(le_entity_t *)*512);
	
	component_family_id_t qry[] = {
		COMP_FAMILY_RENDERABLE,
		COMP_FAMILY_POSITION
	};
	
	size_t res = em_get_entities_with_components(ps->e_manager, qry, 2, ps->qry_resp_cache, 512);
	
	le_entity_t *current_entity = NULL;
	le_component_t *current_ren = NULL;
	
	cd_position_t *pos;
	le_particle_emitter_t *pe;

	for (size_t i = 0; i < res; i++)
	{
		current_entity = ps->qry_resp_cache[i];
		current_ren = entity_get_component(current_entity, COMP_FAMILY_RENDERABLE);
		
		if (current_ren->subid == REN_SUB_PEMITTER)
		{
			pe = current_ren->comp_data; 
			if (particle_emitter_should_handle(pe))
			{
				pos = entity_get_component(current_entity, COMP_FAMILY_POSITION)->comp_data;
					
				pe->ri.pos = pos->pos;
				particle_emitter_update(pe, dt);
				
				if (!particle_emitter_should_handle(pe))		//handling can change state during update()
				{
					//mark for removal
				}
			}
		}

	}
}
