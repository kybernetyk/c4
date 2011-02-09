/*
 *  atlas_quad.h
 *  c4
 *
 *  Created by jrk on 9/2/11.
 *  Copyright 2011 flux forge. All rights reserved.
 *
 */

#pragma once
#include "renderinfo.h"
#include "texture.h"

typedef struct atlas_quad_t
{
	renderinfo_t renderinfo;
	rect_t src_rect;
	tex2d_id tex_id;
} atlas_quad_t;

extern GLuint atlas_quad_load(const char *filename, atlas_quad_t *quad_out);
extern void atlas_quad_render(atlas_quad_t *quad);
extern void atlas_quad_free(atlas_quad_t *quad);