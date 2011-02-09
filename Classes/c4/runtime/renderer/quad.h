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
#include "renderinfo.h"
#include "texture.h"

typedef struct quad_t
{
	renderinfo_t renderinfo;
	tex2d_id tex_id;
} quad_t;

extern GLuint quad_load(const char *filename, quad_t *quad_out);
extern void quad_render(quad_t *quad);
extern void quad_free(quad_t *quad);