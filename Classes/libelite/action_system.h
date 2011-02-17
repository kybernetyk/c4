/*
 *  action_system.h
 *  c4
 *
 *  Created by jrk on 17/2/11.
 *  Copyright 2011 Jaroslaw Szpilewski. All rights reserved.
 *
 */

#pragma once
#include "elite.h"

#define ACTN_SUB_MOVETO 0x01

typedef struct cd_actn_move_to_t
{
	vec2d_t dest;
		
	double _ups_x;
	double _ups_y;
	
	double duration;
	bool finished;
	
} cd_actn_move_to_t;

typedef struct le_action_system_t
{
	le_entity_t **qry_resp_cache;
	le_entity_manager_t *e_manager;
} le_action_system_t;

extern void action_system_init(le_action_system_t *sys, le_entity_manager_t *mgr);
extern void action_system_shutdown(le_action_system_t *sys);

extern void action_system_update(le_action_system_t *sys, double dt);

extern le_component_t *action_system_add_action_to_entity(le_action_system_t *sys, le_entity_t *ent);

extern cd_actn_move_to_t *action_move_to_init(le_component_t *comp, vec2d_t dest);
