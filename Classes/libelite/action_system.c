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
#include <stdio.h>

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

static void step_action(action_header_t *header, double dt)
{
	header->timestamp += dt;
	if (header->duration - header->timestamp <= 0.0)
		header->finished = true;
}


static void handle_move_to(action_header_t *actn_hdr, le_entity_t *ent, double dt)
{
	cd_position_t *pos = entity_get_component(ent, COMP_FAMILY_POSITION);
	if (!pos)
		return;
	
	cd_actn_move_to_t *actn = actn_hdr->action;
	
	if (actn_hdr->duration == 0.0)
	{
		pos->pos = actn->dest;
		return;
	}
	
	if (!actn_hdr->initialized)
	{
		vec2d_t d;
		d.x = actn->dest.x - pos->pos.x;
		d.y = actn->dest.y - pos->pos.y;
		
		actn->_ups_x = d.x / actn_hdr->duration;
		actn->_ups_y = d.y / actn_hdr->duration;
		actn_hdr->initialized = true;
	}
	
	pos->pos.x += actn->_ups_x * dt;
	pos->pos.y += actn->_ups_y * dt;
}

static void handle_action_container(cd_action_container_t *cont, le_entity_t *ent, double dt)
{
	action_header_t *cur_hdr = NULL;
	
	for (int i = 0; i < NUM_ACTIONS_PER_CONTAINER; i++)
	{
		cur_hdr = cont->headers[i];
		if (!cur_hdr)
			continue;

		step_action(cur_hdr, dt);
		
		switch (cur_hdr->type) 
		{
			case ACTN_SUB_MOVETO:
				handle_move_to(cur_hdr, ent, dt);
				break;
			default:
				break;
		}

		if (cur_hdr->finished)
		{
			if (cur_hdr->on_complete_action_header)
			{
				action_header_t *p = cur_hdr->on_complete_action_header;
				
				if (cur_hdr->action_deallocator)
					cur_hdr->action_deallocator(cur_hdr->action);
				free(cur_hdr);
				cont->headers[i] = p;
			}
			else
			{
				if (cur_hdr->action_deallocator)
					cur_hdr->action_deallocator(cur_hdr->action);
				free(cur_hdr);
				cont->headers[i] = NULL;
			}
		}
	}
}
void action_system_update(le_action_system_t *sys, double dt)
{
	memset(sys->qry_resp_cache,0x00,sizeof(le_entity_t *)*g_sysconfig.entity_pool_size);

	size_t res = em_get_entities_with_component(sys->e_manager,COMP_FAMILY_ACTION_CONTAINER, sys->qry_resp_cache, g_sysconfig.entity_pool_size);
	
	le_entity_t *current_entity = NULL;
	le_component_header_t *comp_hdr = NULL;
	cd_action_container_t *act_cont = NULL;

	for (size_t i = 0; i < res; i++)
	{
		current_entity = sys->qry_resp_cache[i];
		comp_hdr = entity_get_component_header(current_entity, COMP_FAMILY_ACTION_CONTAINER);
		act_cont = comp_get(comp_hdr);
		
		handle_action_container(act_cont, current_entity, dt);
		
	}		
	
}

static void free_action_container(void *data)
{
	cd_action_container_t *c = data;
	for (int i = 0; i < NUM_ACTIONS_PER_CONTAINER; i++)
	{
		if (c->headers[i])
		{
			if (c->headers[i]->action_deallocator)
				c->headers[i]->action_deallocator(c->headers[i]->action);
			free(c->headers[i]);
		}
		free(c->headers);
	}
}

action_header_t *action_system_add_action_to_entity(le_action_system_t *sys, le_entity_t *ent)
{
	cd_action_container_t *cont = entity_get_component(ent, COMP_FAMILY_ACTION_CONTAINER);
	if (!cont)
	{
		le_component_header_t *comp_hdr = entity_add_component(ent, COMP_FAMILY_ACTION_CONTAINER);
		comp_hdr->component = malloc(sizeof(cd_action_container_t));
		comp_hdr->component_deallocator = free_action_container;
		memset(comp_hdr->component,0x00,sizeof(cd_action_container_t));
		cont = comp_hdr->component;
	}
	
	action_header_t *a_hdr = NULL;
	for (int i = 0; i < NUM_ACTIONS_PER_CONTAINER; i++)
	{
		if (!cont->headers[i])
		{
			cont->headers[i] = malloc(sizeof(action_header_t));
			memset(cont->headers[i], 0x00, sizeof(action_header_t));
			a_hdr = cont->headers[i];
			break;
		}
	}
	
	if (!a_hdr)
	{
		printf("couldn't find empty header slot ...\n");
		abort();
		return NULL;
	}
	
	return a_hdr;
}

cd_actn_move_to_t *action_move_to_init(action_header_t *act_hdr, vec2d_t dest)
{
	cd_actn_move_to_t *act = malloc(sizeof(cd_actn_move_to_t));
	
	act->dest = dest;
	act->_ups_x = act->_ups_y = 0.0;

	act_hdr->initialized = false;
	act_hdr->finished = false;
	act_hdr->duration = 0.0;
	act_hdr->timestamp = 0.0;
	act_hdr->type = ACTN_SUB_MOVETO;
	act_hdr->action_deallocator = free;
	act_hdr->action = act;
	
	return act;
}

void action_append_action(action_header_t *first, action_header_t *to_append)
{
	action_header_t *last = first;
	while (1)
	{
		if (last->on_complete_action_header)
			last = last->on_complete_action_header;
		else
			break;
	}
	last->on_complete_action_header = to_append;
}

//
//cd_actn_move_to_t *action_move_to_init(le_component_header_t *comp, vec2d_t dest)
//{
//	cd_actn_move_to_t *ret = malloc(sizeof(cd_actn_move_to_t));
//	ret->dest = dest;
//	ret->_ups_x = ret->_ups_y = 0.0;
//	ret->header.initialized = false;
//	ret->header.finished = false;
//	ret->header.duration = 0.0;
//	ret->header.timestamp = 0.0;
//
//	comp->subid = ACTN_SUB_MOVETO;
//	comp->component = ret;
//	comp->component_deallocator = free;
//
//	
//	return ret;
//}
//
//le_component_header_t *action_system_add_action_to_entity(le_action_system_t *sys, le_entity_t *ent)
//{
//	le_component_header_t *ret = entity_add_component(ent, COMP_FAMILY_ACTION_CONTAINER);
//
//	
//	return ret;
//}