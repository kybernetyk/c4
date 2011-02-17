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
	cd_quad_t *q = data;
	fs_quad_free(q->quad);
	free(data);
}

cd_quad_t *comp_quad_init(le_component_header_t *comp, const char *filename)
{
	fs_quad_t *q = quad_new();
	
	if (!fs_quad_load(filename, q))
	{
		fs_quad_free(q);
		abort();
		return NULL;
	}
	cd_quad_t *ret = malloc(sizeof(cd_quad_t));
	ret->quad = q;
	
	comp->component = ret;
	comp->component_deallocator = comp_quad_free;
	comp->family = COMP_FAMILY_RENDERABLE;
	comp->subid = REN_SUB_QUAD;
#ifdef COMP_INCLUDES_NAME 
	sprintf(comp->name, "%s", "COMP_FAMILY_RENDERABLE.REN_SUB_QUAD");
#endif
	return ret;
}

#pragma mark atlas quad
static void comp_atlas_quad_free(void *data)
{
	cd_atlas_quad_t *aq = data;
	
	fs_atlas_quad_free(aq->atlas_quad);
	free(data);
}

cd_atlas_quad_t *comp_atlas_quad_init(le_component_header_t *comp, const char *filename, rect_t src)
{
	fs_atlas_quad_t *aq = fs_atlas_quad_new();
	if (!fs_atlas_quad_load(filename, aq))
	{
		fs_atlas_quad_free(aq);
		abort();
		return NULL;
	}

	cd_atlas_quad_t *ret = malloc(sizeof(cd_atlas_quad_t));
	ret->atlas_quad = aq;
	
	aq->src_rect = src;
	aq->ri.size = size2d_make(src.w, src.h);

	comp->component = ret;
	comp->component_deallocator = comp_atlas_quad_free;
	comp->family = COMP_FAMILY_RENDERABLE;
	comp->subid = REN_SUB_ATLAS_QUAD;

#ifdef COMP_INCLUDES_NAME 	
	sprintf(comp->name, "%s", "COMP_FAMILY_RENDERABLE.REN_SUB_ATLAS_QUAD");
#endif
	
	return ret;
}

#pragma mark font
static void comp_text_free(void *data)
{
	fs_font_free(((cd_text_t*)data)->font);
	free(((cd_text_t*)data)->string);
	free(data);
}

cd_text_t *comp_text_init(le_component_header_t *comp, const char *filename, const char *text)
{
	fs_font_t *fnt = fs_font_new();
	
	if (!fs_font_load(filename, fnt))
	{
		fs_font_free(fnt);
		abort();
		return NULL;
	}

	cd_text_t *ret = malloc(sizeof(cd_text_t));
	ret->font = fnt;
	ret->string = malloc(strlen(text)+1);
	sprintf(ret->string,"%s", text);
		
	comp->component = ret;
	comp->component_deallocator = comp_text_free;
	comp->family = COMP_FAMILY_RENDERABLE;
	comp->subid = REN_SUB_TEXT;
#ifdef COMP_INCLUDES_NAME 
	sprintf(comp->name, "%s", "COMP_FAMILY_RENDERABLE.REN_SUB_TEXT");
#endif
	return ret;
}

void comp_text_set_text(le_component_header_t *comp, const char *text)
{
	cd_text_t *ctxt = comp->component;
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
static void comp_pe_free(void *data)
{
	cd_pemitter_t *pe = data;
	fs_particle_emitter_free(pe->pemitter);
	free(data);
}

cd_pemitter_t *comp_pe_init(le_component_header_t *comp, const char *filename)
{
	fs_particle_emitter_t *pe = particle_emitter_new();
	
	if (!fs_particle_emitter_load(filename, pe))
	{
		fs_particle_emitter_free(pe);
		abort();
		return NULL;
	}
	cd_pemitter_t *ret = malloc(sizeof(cd_pemitter_t));
	ret->pemitter = pe;
	
	comp->component = ret;
	comp->component_deallocator = comp_pe_free;
	comp->family = COMP_FAMILY_RENDERABLE;
	comp->subid = REN_SUB_PEMITTER;
#ifdef COMP_INCLUDES_NAME 
	sprintf(comp->name, "%s", "COMP_FAMILY_RENDERABLE.REN_SUB_PEMITTER");
#endif
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
 component->user_data = malloc(sizeof(fs_quad_t));
 component->comp_data_deallocator = (void(*)(void*))quad_free;
 
 qaddr_t
 }*/