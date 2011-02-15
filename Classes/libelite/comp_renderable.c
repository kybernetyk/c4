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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
	
	comp->comp_data = q;
	comp->comp_data_deallocator = comp_quad_free;
	comp->family = COMP_FAMILY_RENDERABLE;
	comp->subid = REN_SUB_QUAD;
	sprintf(comp->name, "%s", "COMP_FAMILY_RENDERABLE.REN_SUB_QUAD");
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
	aq->ri.size = size2d_make(src.w, src.h);

	comp->comp_data = aq;
	comp->comp_data_deallocator = comp_atlas_quad_free;
	comp->family = COMP_FAMILY_RENDERABLE;
	comp->subid = REN_SUB_ATLAS_QUAD;
	sprintf(comp->name, "%s", "COMP_FAMILY_RENDERABLE.REN_SUB_ATLAS_QUAD");
	
	return aq;
}

#pragma mark font
void comp_text_free(void *data)
{
	font_free(((cd_text_t*)data)->font);
	free(((cd_text_t*)data)->string);
	free(data);
}

cd_text_t *comp_text_init(le_component_t *comp, const char *filename, const char *text)
{
	le_font_t *fnt = font_new();
	
	if (!font_load(filename, fnt))
	{
		font_free(fnt);
		return NULL;
	}

	cd_text_t *ret = malloc(sizeof(cd_text_t));
	ret->font = fnt;
	ret->string = malloc(strlen(text)+1);
	sprintf(ret->string,"%s", text);
		
	comp->comp_data = ret;
	comp->comp_data_deallocator = comp_text_free;
	comp->family = COMP_FAMILY_RENDERABLE;
	comp->subid = REN_SUB_TEXT;
	sprintf(comp->name, "%s", "COMP_FAMILY_RENDERABLE.REN_SUB_TEXT");
	return ret;
}

void comp_text_set_text(le_component_t *comp, const char *text)
{
	cd_text_t *ctxt = comp->comp_data;
	if (ctxt->string)
	{
		if (strlen(ctxt->string) > strlen(text))
		{	
			sprintf(ctxt->string, "%s", text);
			return;
		}
		free(ctxt->string);
	}
	
	ctxt->string = malloc(strlen(text)+1);
	sprintf(ctxt->string, "%s", text);
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
	
	comp->comp_data = ret;
	comp->comp_data_deallocator = comp_pe_free;
	comp->family = COMP_FAMILY_RENDERABLE;
	comp->subid = REN_SUB_PEMITTER;
	sprintf(comp->name, "%s", "COMP_FAMILY_RENDERABLE.REN_SUB_PEMITTER");
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
 component->comp_data_deallocator = (void(*)(void*))quad_free;
 
 qaddr_t
 }*/