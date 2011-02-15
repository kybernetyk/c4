/*
 *  renderable.h
 *  c4
 *
 *  Created by jrk on 14/2/11.
 *  Copyright 2011 Jaroslaw Szpilewski. All rights reserved.
 *
 */

#pragma once
#include "elite.h"

typedef struct cd_text_t
{
	char *string;
	le_font_t *font;
} cd_text_t;

extern le_quad_t *comp_quad_init(le_component_t *comp, const char *filename);
extern le_atlas_quad_t *comp_atlas_quad_init(le_component_t *comp, const char *filename, rect_t src);
extern le_particle_emitter_t *comp_pe_init(le_component_t *comp, const char *filename);

extern cd_text_t *comp_text_init(le_component_t *comp, const char *filename, const char *text);
extern void comp_text_set_text(le_component_t *comp, const char *text);