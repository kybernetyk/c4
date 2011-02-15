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

typedef struct fs_atlas_quad_t
{
	fs_renderinfo_t ri;
	rect_t src_rect;
	fs_tex2d_id tex_id;
	
	bool is_dynamic;
} fs_atlas_quad_t;


extern fs_atlas_quad_t *fs_atlas_quad_new(void);
extern bool fs_atlas_quad_load(const char *filename, fs_atlas_quad_t *quad_out);
extern void fs_atlas_quad_render(fs_atlas_quad_t *quad);
extern void fs_atlas_quad_free(fs_atlas_quad_t *quad);