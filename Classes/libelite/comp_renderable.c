/*
 *  renderable.c
 *  c4
 *
 *  Created by jrk on 14/2/11.
 *  Copyright 2011 Jaroslaw Szpilewski. All rights reserved.
 *
 */

#include "comp_renderable.h"
#include "elite.h"
#include "render_system.h"

#pragma mark quad
static void comp_quad_free(void *data)
{
	quad_free(data);
}

le_quad_t *comp_quad_init(le_component_t *comp, const char *filename)
{
	le_quad_t *q = quad_new();
	
	if (!quad_load(filename, q))
	{
		quad_free(q);
		return NULL;
	}
	
	comp->user_data = q;
	comp->user_data_deallocator = comp_quad_free;
	comp->family = COMP_FAMILY_RENDERABLE;
	comp->subid = REN_SUB_QUAD;
	
	return q;
}

#pragma mark atlas quad
static void comp_atlas_quad_free(void *data)
{
	atlas_quad_free(data);
}

le_atlas_quad_t *comp_atlas_quad_init(le_component_t *comp, const char *filename, rect_t src)
{
	le_atlas_quad_t *aq = atlas_quad_new();
	
	if (!atlas_quad_load(filename, aq))
	{
		atlas_quad_free(aq);
		return NULL;
	}

	aq->src_rect = src;
	
	comp->user_data = aq;
	comp->user_data_deallocator = comp_atlas_quad_free;
	comp->family = COMP_FAMILY_RENDERABLE;
	comp->subid = REN_SUB_ATLAS_QUAD;
	
	
	return aq;
}

#pragma mark font
void comp_font_free(void *data)
{
	font_free(data);
}

le_font_t *comp_font_init(le_component_t *comp, const char *filename)
{
	le_font_t *ret = font_new();
	
	if (!font_load(filename, ret))
	{
		font_free(ret);
		return NULL;
	}
	
	comp->user_data = ret;
	comp->user_data_deallocator = comp_font_free;
	comp->family = COMP_FAMILY_RENDERABLE;
	comp->subid = REN_SUB_FONT;
	
	return ret;
}


#pragma mark particle emitter
void comp_pe_free(void *data)
{
	particle_emitter_free(data);
}

le_particle_emitter_t *comp_pe_init(le_component_t *comp, const char *filename)
{
	le_particle_emitter_t *ret = particle_emitter_new();
	
	if (!particle_emitter_load(filename, ret))
	{
		particle_emitter_free(ret);
		return NULL;
	}
	
	comp->user_data = ret;
	comp->user_data_deallocator = comp_pe_free;
	comp->family = COMP_FAMILY_RENDERABLE;
	comp->subid = REN_SUB_PEMITTER;
	
	return ret;
}

/*
 entity_t *e = em_new_ent();
 component_t *c = em_add_component(e,RENDERABLE);
 comp_renderable_init(c, REN_QUAD, "filename.png");
 
 c = em_add_component(e,POSITION);
 comp_position_init(c, (vec2d_t){0,0});
 vec2d_t v = comp_position_get_data(c);
 vec2d_t v = comp_get_userdata(c, vec2d_t);
 
 void render_system_make_quad(le_component_t *component)
 {
 component->user_data = malloc(sizeof(le_quad_t));
 component->user_data_deallocator = (void(*)(void*))quad_free;
 
 qaddr_t
 }*/