/*
 *  render_system.h
 *  c4
 *
 *  Created by jrk on 14/2/11.
 *  Copyright 2011 Jaroslaw Szpilewski. All rights reserved.
 *
 */

#pragma once
#include "elite.h"

#define REN_SUB_QUAD 0x01
#define REN_SUB_ATLAS_QUAD 0x02
#define REN_SUB_FONT 0x03
#define REN_SUB_PEMITTER 0x04

typedef struct le_render_system_t
{
	le_entity_t *qry_resp_cache[512];
	
} le_render_system_t;

extern void render_system_init(le_render_system_t *rs);
extern void render_system_shutdown(le_render_system_t *rs);

extern void render_system_update(le_render_system_t *rs, double dt);
extern void render_system_render(le_render_system_t *rs);

extern void render_system_make_quad(le_component_t *component);