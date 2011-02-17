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
	fs_font_t *font;
} cd_text_t;

typedef struct cd_quad_t
{
	fs_quad_t *quad;
} cd_quad_t;

typedef struct cd_atlas_quad_t
{
	fs_atlas_quad_t *atlas_quad;
} cd_atlas_quad_t;

typedef struct cd_pemitter_t
{
	fs_particle_emitter_t *pemitter;
} cd_pemitter_t;

extern cd_quad_t *comp_quad_init(le_component_header_t *comp, const char *filename);
extern cd_atlas_quad_t *comp_atlas_quad_init(le_component_header_t *comp, const char *filename, rect_t src);
extern cd_pemitter_t *comp_pe_init(le_component_header_t *comp, const char *filename);

extern cd_text_t *comp_text_init(le_component_header_t *comp, const char *filename, const char *text);
extern void comp_text_set_text(le_component_header_t *comp, const char *text);