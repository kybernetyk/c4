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

GLuint quad_load(const char *filename, quad_t *quad_out)
{
	quad_out->tex_id = tex2d_load(filename);
	if (quad_out->tex_id < 0)
	{
		printf("couldn't load quad %s ...\n", filename);
		abort();
		return 0;
	}
	
	tex2d_t *tex = tex2d_get_tex_by_id(quad_out->tex_id);
	
	quad_out->renderinfo.alpha = 1.0;
	quad_out->renderinfo.rot_a = 0.0;
	quad_out->renderinfo.scale = vec2d_make(1.0, 1.0);
	quad_out->renderinfo.size = size2d_make(tex->w,tex->h);
	quad_out->renderinfo.anchor_point = vec2d_make(0.5, 0.5);

	return tex->name;
}

void quad_free(quad_t *quad)
{
	tex2d_release(quad->tex_id);
	quad->tex_id = -1;
}


void quad_render(quad_t *quad)
{
	tex2d_t *tex = tex2d_get_tex_by_id(quad->tex_id);
	if (!tex)
		return;
	
	glPushMatrix();
	renderinfo_transform(&quad->renderinfo);
	
	GLfloat w = quad->renderinfo.size.w;
	GLfloat h = quad->renderinfo.size.h;
	GLfloat alpha = quad->renderinfo.alpha;
	
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

	
	tex2d_bind(tex2d_get_tex_by_id(quad->tex_id)->name);
	
	
	glVertexPointer(3, GL_FLOAT, 0, vertices);
	glTexCoordPointer(2, GL_FLOAT, 0, coordinates);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	
	
	glPopMatrix();
}