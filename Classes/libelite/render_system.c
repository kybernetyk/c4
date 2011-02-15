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

static int z_comp(const void *ep1, const void *ep2)
{
	/* and now for some casting magic [we're getting pointers to pointers as simple pointers] ... stupid warnings/errors */
//	le_entity_t *e1 = *(le_entity_t**)ep1;
//	le_entity_t *e2 = *(le_entity_t**)ep2;
	
	cd_position_t *p1, *p2;
	p1 = entity_get_component(*(le_entity_t**)ep1, COMP_FAMILY_POSITION)->comp_data;
	p2 = entity_get_component(*(le_entity_t**)ep2, COMP_FAMILY_POSITION)->comp_data;
	
	if (p1->z < p2->z)
		return -1;
	if (p1->z > p2->z)
		return 1;
	
	return 0;
}

void render_system_render(le_render_system_t *rs)
{
	//qry and sort only if something has changed
	if (rs->e_manager->is_dirty)
	{
		memset(rs->qry_resp_cache,0x00,sizeof(le_entity_t *)*512);
		component_family_id_t qry[] = {
			COMP_FAMILY_RENDERABLE,
			COMP_FAMILY_POSITION
		};
		rs->resp_size = em_get_entities_with_components(rs->e_manager, qry, 2, rs->qry_resp_cache, 512);
		qsort(rs->qry_resp_cache, rs->resp_size, sizeof(le_entity_t *),z_comp);
	}
	
//	for (int i = 0; i < res; i++)
//	{
//		current_entity = rs->qry_resp_cache[i];
//		printf("%p = z: %f\n", current_entity, comp_get_userdata(entity_get_component(current_entity, COMP_FAMILY_POSITION),cd_position_t)->z  );		
//	}
//
//	printf("\n\n");
	
//	for (int i = 0; i < res; i++)
//	{
//		current_entity = rs->qry_resp_cache[i];
//		printf("%p = z: %f\n", current_entity, comp_get_userdata(entity_get_component(current_entity, COMP_FAMILY_POSITION),cd_position_t)->z  );		
//	}


	le_entity_t *current_entity = NULL;
	le_component_t *current_ren = NULL;
	
	le_quad_t *quad = NULL;
	le_atlas_quad_t *aquad = NULL;
	cd_position_t *pos = NULL;
	cd_text_t *text = NULL;
	le_particle_emitter_t *pe = NULL;
	
	for (size_t i = 0; i < rs->resp_size; i++)
	{
		current_entity = rs->qry_resp_cache[i];
		current_ren = entity_get_component(current_entity, COMP_FAMILY_RENDERABLE);
		pos = entity_get_component(current_entity, COMP_FAMILY_POSITION)->comp_data;
		
		switch (current_ren->subid) 
		{
			case REN_SUB_QUAD:
				quad = current_ren->comp_data;
				quad->ri.pos = pos->pos;
				quad->ri.zval = pos->z;
				quad->ri.rot_a = pos->rot;
				quad_render(quad);
				break;

			case REN_SUB_ATLAS_QUAD:
				aquad = current_ren->comp_data;
				aquad->ri.pos = pos->pos;
				aquad->ri.zval = pos->z;
				aquad->ri.rot_a = pos->rot;
				atlas_quad_render(aquad);
				break;
			case REN_SUB_TEXT:
				text = current_ren->comp_data;
				text->font->ri.pos = pos->pos;
				text->font->ri.zval = pos->z;
				text->font->ri.rot_a = pos->rot;
				font_render(text->font, text->string);
				break;
			case REN_SUB_PEMITTER:
				pe = current_ren->comp_data;
				pe->ri.pos = pos->pos;
				pe->ri.zval = pos->z;
				particle_emitter_render(pe);
				break;
				
			
			default:
				break;
		}
	}
}

