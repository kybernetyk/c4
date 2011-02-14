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

typedef struct le_quad_t
{
	renderinfo_t ri;
	tex2d_id tex_id;
	
	bool is_dynamic;
} le_quad_t;

extern le_quad_t *quad_new(void);
extern bool quad_load(const char *filename, le_quad_t *quad_out);
extern void quad_render(le_quad_t *quad);
extern void quad_free(le_quad_t *quad);