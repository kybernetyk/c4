/*
 *  action_system.c
 *  c4
 *
 *  Created by jrk on 17/2/11.
 *  Copyright 2011 Jaroslaw Szpilewski. All rights reserved.
 *
 */

#include "action_system.h"
#include <memory.h>
#include <stdlib.h>

void action_system_init(le_action_system_t *sys, le_entity_manager_t *mgr)
{
	sys->qry_resp_cache = malloc(g_sysconfig.entity_pool_size * sizeof(le_entity_t *));
	memset(sys->qry_resp_cache,0x00,sizeof(le_entity_t *)*g_sysconfig.entity_pool_size);
	sys->e_manager = mgr;
	
}

void action_system_shutdown(le_action_system_t *sys)
{
	free(sys->qry_resp_cache);
}

static void handle_move_to(cd_actn_move_to_t *actn, le_entity_t *ent)
{
	//... blehrg
}

void action_system_update(le_action_system_t *sys, double dt)
{
	memset(sys->qry_resp_cache,0x00,sizeof(le_entity_t *)*g_sysconfig.entity_pool_size);

	size_t res = em_get_entities_with_component(sys->e_manager,COMP_FAMILY_ACTION_CONTAINER, sys->qry_resp_cache, g_sysconfig.entity_pool_size);
	
	le_entity_t *current_entity = NULL;
	le_component_t *current_action = NULL;
	for (size_t i = 0; i < res; i++)
	{
		current_entity = sys->qry_resp_cache[i];
		current_action = entity_get_component(current_entity, COMP_FAMILY_ACTION_CONTAINER);
		
		switch (current_action->subid) 
		{
			case ACTN_SUB_MOVETO:
				handle_move_to(current_action->comp_data, current_entity);
				break;
			default:
				break;
		}

	}		
	
}

cd_actn_move_to_t *action_move_to_init(le_component_t *comp, vec2d_t dest)
{
	cd_actn_move_to_t *ret = malloc(sizeof(cd_actn_move_to_t));
	ret->dest = dest;
	ret->_ups_x = ret->_ups_y = 0.0;

	comp->subid = ACTN_SUB_MOVETO;
	comp->comp_data = ret;
	comp->comp_data_deallocator = free;
	
	return ret;
}

le_component_t *action_system_add_action_to_entity(le_action_system_t *sys, le_entity_t *ent)
{
	le_component_t *ret = entity_add_component(ent, COMP_FAMILY_ACTION_CONTAINER);

	
	return ret;
}