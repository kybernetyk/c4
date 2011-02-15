/*
 *  quad.c
 *  c4
 *
 *  Created by jrk on 8/2/11.
 *  Copyright 2011 Jaroslaw Szpilewski. All rights reserved.
 *
 */
#include <stdlib.h>
#include <stdio.h>
#include "renderinfo.h"
#include "quad.h"

fs_quad_t *quad_new(void)
{
	fs_quad_t *ret = calloc(1, sizeof(fs_quad_t));
	ret->is_dynamic = true;
	
	return ret;
}

bool fs_quad_load(const char *filename, fs_quad_t *quad_out)
{
	quad_out->tex_id = fs_tex2d_load(filename);
	if (quad_out->tex_id < 0)
	{
		printf("couldn't load quad %s ...\n", filename);
		abort();
		return false;
	}
	
	fs_tex2d_t *tex = fs_tex2d_get_tex_by_id(quad_out->tex_id);
	
	quad_out->ri.alpha = 1.0;
	quad_out->ri.rot_a = 0.0;
	quad_out->ri.scale = vec2d_make(1.0, 1.0);
	quad_out->ri.size = size2d_make(tex->w,tex->h);
	quad_out->ri.anchor_point = vec2d_make(0.5, 0.5);

	return true;
}

void fs_quad_free(fs_quad_t *quad)
{
	fs_tex2d_release(quad->tex_id);
	quad->tex_id = -1;
	
	if (quad->is_dynamic)
		free(quad);
}


void fs_quad_render(fs_quad_t *quad)
{
	fs_tex2d_t *tex = fs_tex2d_get_tex_by_id(quad->tex_id);
	if (!tex)
		return;
	
	glPushMatrix();
	fs_renderinfo_transform(&quad->ri);
	
	GLfloat w = quad->ri.size.w;
	GLfloat h = quad->ri.size.h;
	GLfloat alpha = quad->ri.alpha;
	
	GLfloat		coordinates[] = { 0.0f,	1.0,
		1.0,	1.0,
		0.0f,	0.0f,
		1.0,	0.0f };
	GLfloat		vertices[] = 
	{	
		0,			0,			0,
		w,	0,			0,
		0,			h,	0,
		w,			h,	0
	};
	
	GLfloat colors[] = 
	{
		1.0,1.0,1.0,alpha,
		1.0,1.0,1.0,alpha,
		1.0,1.0,1.0,alpha,
		1.0,1.0,1.0,alpha,
	};
	glColorPointer(4, GL_FLOAT, 0, colors);

	
	fs_tex2d_bind(fs_tex2d_get_tex_by_id(quad->tex_id)->name);
	
	
	glVertexPointer(3, GL_FLOAT, 0, vertices);
	glTexCoordPointer(2, GL_FLOAT, 0, coordinates);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	
	
	glPopMatrix();
}