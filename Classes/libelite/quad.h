/*
 *  quad.h
 *  c4
 *
 *  Created by jrk on 8/2/11.
 *  Copyright 2011 Jaroslaw Szpilewski. All rights reserved.
 *
 */

/*
 simple textured quad
 renders a texture onto a quad.
*/

#pragma once
#include <stdbool.h>

#include "renderinfo.h"
#include "texture.h"

typedef struct fs_quad_t
{
	fs_renderinfo_t ri;
	fs_tex2d_id tex_id;
	
	bool is_dynamic;
} fs_quad_t;

extern fs_quad_t *quad_new(void);
extern bool fs_quad_load(const char *filename, fs_quad_t *quad_out);
extern void fs_quad_render(fs_quad_t *quad);
extern void fs_quad_free(fs_quad_t *quad);