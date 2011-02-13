/*
 *  atlas_quad.h
 *  c4
 *
 *  Created by jrk on 9/2/11.
 *  Copyright 2011 Jaroslaw Szpilewski. All rights reserved.
 *
 */

#pragma once
#include <stdbool.h>

#include "renderinfo.h"
#include "texture.h"

typedef struct le_atlas_quad_t
{
	renderinfo_t ri;
	rect_t src_rect;
	tex2d_id tex_id;
} le_atlas_quad_t;

extern bool atlas_quad_load(const char *filename, le_atlas_quad_t *quad_out);
extern void atlas_quad_render(le_atlas_quad_t *quad);
extern void atlas_quad_free(le_atlas_quad_t *quad);