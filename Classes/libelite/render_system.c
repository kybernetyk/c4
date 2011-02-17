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
	rs->qry_resp_cache = malloc(g_sysconfig.entity_pool_size * sizeof(le_entity_t *));
	memset(rs->qry_resp_cache,0x00,sizeof(le_entity_t *)*g_sysconfig.entity_pool_size);
	rs->e_manager = mgr;
}

void render_system_shutdown(le_render_system_t *rs)
{
	free(rs->qry_resp_cache);
}

static int z_comp(const void *ep1, const void *ep2)
{
	/* and now for some casting magic [we're getting pointers to pointers as simple pointers] ... stupid warnings/errors */
//	le_entity_t *e1 = *(le_entity_t**)ep1;
//	le_entity_t *e2 = *(le_entity_t**)ep2;
	
	cd_position_t *p1, *p2;
	p1 = entity_get_component_header(*(le_entity_t**)ep1, COMP_FAMILY_POSITION)->component;
	p2 = entity_get_component_header(*(le_entity_t**)ep2, COMP_FAMILY_POSITION)->component;
	
	if (p1->z < p2->z)
		return -1;
	if (p1->z > p2->z)
		return 1;

	return 0;
/*	
	le_entity_t *e1 = *(le_entity_t**)ep1;
	le_entity_t *e2 = *(le_entity_t**)ep2;
	
	if (e1->guid < e2->guid)
		return -1;
	if (e1->guid > e2->guid)
		return 1;
	
*/
}

void render_system_render(le_render_system_t *rs)
{
	le_entity_t *current_entity = NULL;
	le_component_header_t *current_ren = NULL;
	
	//qry and sort only if something has changed
	if (rs->e_manager->is_dirty)
	{
		memset(rs->qry_resp_cache,0x00,sizeof(le_entity_t *)*g_sysconfig.entity_pool_size);
		component_family_id_t qry[] = {
			COMP_FAMILY_RENDERABLE,
			COMP_FAMILY_POSITION
		};

		rs->resp_size = em_get_entities_with_components(rs->e_manager, qry, 2, rs->qry_resp_cache, g_sysconfig.entity_pool_size);
		qsort(rs->qry_resp_cache, rs->resp_size, sizeof(le_entity_t *),z_comp);
	}
	
	cd_quad_t *quad = NULL;
	cd_atlas_quad_t *aquad = NULL;
	cd_position_t *pos = NULL;
	cd_text_t *text = NULL;
	cd_pemitter_t *pe = NULL;
	
	for (size_t i = 0; i < rs->resp_size; i++)
	{
		current_entity = rs->qry_resp_cache[i];
		current_ren = entity_get_component_header(current_entity, COMP_FAMILY_RENDERABLE);
		pos = entity_get_component_header(current_entity, COMP_FAMILY_POSITION)->component;
		
		switch (current_ren->subid) 
		{
			case REN_SUB_QUAD:
				quad = comp_get(current_ren);
				quad->quad->ri.pos = pos->pos;
				quad->quad->ri.zval = pos->z;
				quad->quad->ri.rot_a = pos->rot;
				fs_quad_render(quad->quad);
				break;
			case REN_SUB_ATLAS_QUAD:
				aquad = comp_get(current_ren);
				aquad->atlas_quad->ri.pos = pos->pos;
				aquad->atlas_quad->ri.zval = pos->z;
				aquad->atlas_quad->ri.rot_a = pos->rot;
				fs_atlas_quad_render(aquad->atlas_quad);
				break;
			case REN_SUB_TEXT:
				text = comp_get(current_ren);
				text->font->ri.pos = pos->pos;
				text->font->ri.zval = pos->z;
				text->font->ri.rot_a = pos->rot;
				fs_font_render(text->font, text->string);
				break;
			case REN_SUB_PEMITTER:
				pe = comp_get(current_ren);
				pe->pemitter->ri.pos = pos->pos;
				pe->pemitter->ri.zval = pos->z;
				fs_particle_emitter_render(pe->pemitter);
				break;
			default:
				abort();
				break;
		}
	}
}

