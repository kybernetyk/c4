/*
 *  particle_system.c
 *  c4
 *
 *  Created by jrk on 14/2/11.
 *  Copyright 2011 Jaroslaw Szpilewski. All rights reserved.
 *
 */

#include "particle_system.h"
#include <stdlib.h>
#include <memory.h>

void particle_system_init(le_particle_system_t *ps, le_entity_manager_t *mgr)
{
	ps->qry_resp_cache = malloc(g_sysconfig.entity_pool_size * sizeof(le_entity_t *));
	memset(ps->qry_resp_cache,0x00,sizeof(le_entity_t *)*g_sysconfig.entity_pool_size);
	ps->e_manager = mgr;
}

void particle_system_shutdown(le_particle_system_t *ps)
{
	free(ps->qry_resp_cache);
}

void particle_system_update(le_particle_system_t *ps, double dt)
{
	memset(ps->qry_resp_cache,0x00,sizeof(le_entity_t *)*g_sysconfig.entity_pool_size);
	
	component_family_id_t qry[] = {
		COMP_FAMILY_RENDERABLE,
		COMP_FAMILY_POSITION
	};
	
	size_t res = em_get_entities_with_components(ps->e_manager, qry, 2, ps->qry_resp_cache, g_sysconfig.entity_pool_size);
	
	le_entity_t *current_entity = NULL;
	le_component_header_t *current_ren = NULL;
	
	cd_position_t *pos;
	cd_pemitter_t *pe;

	for (size_t i = 0; i < res; i++)
	{
		current_entity = ps->qry_resp_cache[i];
		current_ren = entity_get_component_header(current_entity, COMP_FAMILY_RENDERABLE);
		
		if (current_ren->subid == REN_SUB_PEMITTER)
		{
			pe = current_ren->component; 
			if (fs_particle_emitter_should_handle(pe->pemitter))
			{
				pos = entity_get_component_header(current_entity, COMP_FAMILY_POSITION)->component;
					
				pe->pemitter->ri.pos = pos->pos;
				fs_particle_emitter_update(pe->pemitter, dt);
				
				if (!fs_particle_emitter_should_handle(pe->pemitter))		//handling can change state during update()
				{
					//mark for removal
				}
			}
		}

	}
}
