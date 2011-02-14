/*
 *  render_system.c
 *  c4
 *
 *  Created by jrk on 14/2/11.
 *  Copyright 2011 Jaroslaw Szpilewski. All rights reserved.
 *
 */

#include "render_system.h"
#include <stdlib.h>
#include <stdio.h>
#include <memory.h>
#include "elite.h"

void render_system_init(le_render_system_t *rs, le_entity_manager_t *mgr)
{
	memset(rs->qry_resp_cache,0x00,sizeof(le_entity_t *)*512);
	rs->e_manager = mgr;
}

void render_system_shutdown(le_render_system_t *rs)
{
	
}

void render_system_update(le_render_system_t *rs, double dt)
{
	
}

void render_system_render(le_render_system_t *rs)
{
	memset(rs->qry_resp_cache,0x00,sizeof(le_entity_t *)*512);
	
	component_family_id_t qry[] = {
		COMP_FAMILY_RENDERABLE,
		COMP_FAMILY_POSITION
	};
	
	size_t res = em_get_entities_with_components(rs->e_manager, qry, 2, rs->qry_resp_cache, 512);
	
	le_entity_t *current_entity = NULL;
	le_component_t *current_ren = NULL;
	
	le_quad_t *quad = NULL;
	le_atlas_quad_t *aquad = NULL;
	comp_position_t *pos = NULL;
	comp_text_t *text = NULL;
	le_particle_emitter_t *pe = NULL;
	
	for (size_t i = 0; i < res; i++)
	{
		current_entity = rs->qry_resp_cache[i];
		current_ren = entity_get_component(current_entity, COMP_FAMILY_RENDERABLE);
		pos = entity_get_component(current_entity, COMP_FAMILY_POSITION)->user_data;
		
		switch (current_ren->subid) 
		{
			case REN_SUB_QUAD:
				quad = current_ren->user_data;
				quad->ri.pos = pos->pos;
				quad->ri.zval = pos->z;
				quad->ri.rot_a = pos->rot;
				quad_render(quad);
				break;

			case REN_SUB_ATLAS_QUAD:
				aquad = current_ren->user_data;
				aquad->ri.pos = pos->pos;
				aquad->ri.zval = pos->z;
				aquad->ri.rot_a = pos->rot;
				atlas_quad_render(aquad);
				break;
			case REN_SUB_TEXT:
				text = current_ren->user_data;
				text->font->ri.pos = pos->pos;
				text->font->ri.zval = pos->z;
				text->font->ri.rot_a = pos->rot;
				font_render(text->font, text->string);
				break;
			case REN_SUB_PEMITTER:
				pe = current_ren->user_data;
				pe->ri.pos = pos->pos;
				pe->ri.zval = pos->z;
				particle_emitter_render(pe);
				break;
				
			
			default:
				break;
		}
	}
}

