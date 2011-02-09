/*
 *  atlas_quad.c
 *  c4
 *
 *  Created by jrk on 9/2/11.
 *  Copyright 2011 Jaroslaw Szpilewski. All rights reserved.
 *
 */

#include "atlas_quad.h"
#include <stdlib.h>
#include <stdio.h>
#include "renderinfo.h"
#include "quad.h"

bool atlas_quad_load(const char *filename, atlas_quad_t *quad_out)
{
	quad_out->tex_id = tex2d_load(filename);
	if (quad_out->tex_id < 0)
	{
		printf("couldn't load quad %s ...\n", filename);
		abort();
		return false;
	}
	
	tex2d_t *tex = tex2d_get_tex_by_id(quad_out->tex_id);
	
	quad_out->renderinfo.alpha = 1.0;
	quad_out->renderinfo.rot_a = 0.0;
	quad_out->renderinfo.scale = vec2d_make(1.0, 1.0);
	quad_out->renderinfo.size = size2d_make(tex->w,tex->h);
	quad_out->renderinfo.anchor_point = vec2d_make(0.5, 0.5);
	quad_out->src_rect = rect_make(0.0, 0.0, tex->w, tex->h);
	
	return true;
	
}

void atlas_quad_free(atlas_quad_t *quad)
{
	tex2d_release(quad->tex_id);
	quad->tex_id = -1;
}

void atlas_quad_render(atlas_quad_t *quad)
{
	tex2d_t *tex = tex2d_get_tex_by_id(quad->tex_id);
	if (!tex)
		return;
	
	glPushMatrix();
	renderinfo_transform(&quad->renderinfo);
	
	GLfloat w = quad->renderinfo.size.w;
	GLfloat h = quad->renderinfo.size.h;
	GLfloat alpha = quad->renderinfo.alpha;
	
	rect_t src;
	src.x = quad->src_rect.x / tex->w;
	src.y = quad->src_rect.y / tex->h;

	src.w = quad->src_rect.w / tex->w;
	src.h = quad->src_rect.h / tex->h;
	
	
	GLfloat		coordinates[] = 
	{ 	
		src.x,			src.y + src.h,
		src.x + src.w,	src.y + src.h,
		src.x,			src.y,
		src.x + src.w,	src.y 
	};

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
	
	
	tex2d_bind(tex->name);
	
	
	glVertexPointer(3, GL_FLOAT, 0, vertices);
	glTexCoordPointer(2, GL_FLOAT, 0, coordinates);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	
	
	glPopMatrix();
}