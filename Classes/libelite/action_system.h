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

#define NUM_ACTIONS_PER_CONTAINER 32

#define ACTN_SUB_MOVETO 0x01

typedef struct action_header_t
{
	double duration;
	double timestamp;
	bool finished;
	bool initialized;
	
	void *action;	//pointer do action stuct like cd_actn_move_to_t
	void (*action_deallocator)(void *);
	
	struct action_header_t *on_complete_action_header;
	int type;
	
} action_header_t;

typedef struct cd_action_container_t
{
	action_header_t *headers[NUM_ACTIONS_PER_CONTAINER];
} cd_action_container_t;


typedef struct cd_actn_move_to_t
{
	vec2d_t dest;
		
	double _ups_x;
	double _ups_y;
		
} cd_actn_move_to_t;

typedef struct le_action_system_t
{
	le_entity_t **qry_resp_cache;
	le_entity_manager_t *e_manager;
} le_action_system_t;

extern void action_system_init(le_action_system_t *sys, le_entity_manager_t *mgr);
extern void action_system_shutdown(le_action_system_t *sys);

extern void action_system_update(le_action_system_t *sys, double dt);

//appends to_append to first
extern void action_append_action(action_header_t *first, action_header_t *to_append);

extern action_header_t *action_system_add_action_to_entity(le_action_system_t *sys, le_entity_t *ent);
extern cd_actn_move_to_t *action_move_to_init(action_header_t *act_hdr, vec2d_t dest);
