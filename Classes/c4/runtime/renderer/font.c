/*
 *  font.c
 *  c4
 *
 *  Created by jrk on 9/2/11.
 *  Copyright 2011 Jaroslaw Szpilewski. All rights reserved.
 *
 */

#include "font.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <memory.h>
#include "renderinfo.h"
#include "util.h"

bool font_load(const char *filename, font_t *font_out)
{
	const char *fn = path_for_file(filename);
	
	if (!bm_loadfont(fn, &font_out->fnt))
	{
		printf("couldn't load font from %s ...\n", fn);
		abort();
		return false;
	}
	
	font_out->tex_id = tex2d_load(font_out->fnt.tex_filename);
	if (font_out->tex_id < 0)
	{
		printf("couldn't load font texture %s ...\n", font_out->fnt.tex_filename);
		abort();
		return false;
	}

	tex2d_t *tex = tex2d_get_tex_by_id(font_out->tex_id);
	font_out->renderinfo.alpha = 1.0;
	font_out->renderinfo.rot_a = 0.0;
	font_out->renderinfo.scale = vec2d_make(1.0, 1.0);
	font_out->renderinfo.size = size2d_make(tex->w,tex->h);
	font_out->renderinfo.anchor_point = vec2d_make(0.5, 0.5);
	return true;
}


static void font_transform(font_t *fnt, const char *text)
{
	int w = bm_width(&fnt->fnt, text);
	float h = fnt->fnt.line_h*.75; //bm_height(&font, text);

	
	glTranslatef(fnt->renderinfo.pos.x, fnt->renderinfo.pos.y, fnt->renderinfo.zval);
	
	if (fnt->renderinfo.rot_a != 0.0f )
		glRotatef( fnt->renderinfo.rot_a, 0.0f, 0.0f, 1.0f );
	
	if (fnt->renderinfo.scale.x != 1.0 || fnt->renderinfo.scale.y != 1.0)
		glScalef( fnt->renderinfo.scale.x, fnt->renderinfo.scale.y, 1.0f );
	
	glTranslatef(- (fnt->renderinfo.anchor_point.x * w),h - (fnt->renderinfo.anchor_point.y * h), 0);
	
}

void font_render(font_t *fnt, const char *text)
{
	tex2d_t *texture = tex2d_get_tex_by_id(fnt->tex_id);
	if (!texture)
		return;
	
	glPushMatrix();
	font_transform(fnt, text);
	
	GLfloat alpha = fnt->renderinfo.alpha;
	int l = strlen(text);
	
	tex2d_bind(texture->name);

	GLfloat colors[] = 
	{
		1.0,1.0,1.0,alpha,
		1.0,1.0,1.0,alpha,
		1.0,1.0,1.0,alpha,
		1.0,1.0,1.0,alpha,
	};
	glColorPointer(4, GL_FLOAT, 0, colors);
	
	//IF RENDER ERRORS OCCUR
	//EXCHANGE FLOAT FOR DOUBLE!
	float tx,ty,tw,th;
	
	
	bm_char *pchar = NULL;
	for (int i = 0; i < l; i++)
	{
		pchar = &fnt->fnt.chars[ text[i] ];
		
		tx = (float)pchar->x / (float)texture->w;
		ty = (float)pchar->y / (float)texture->h;
		tw = (float)pchar->w / (float)texture->w;
		th = (float)pchar->h / (float)texture->h;
		GLfloat		vertices[] = 
		{	
			0,			0,			0,
			pchar->w,	0,			0,
			0,			pchar->h,	0,
			pchar->w,	pchar->h,	0
		};
		
		GLfloat		coordinates[] = { tx,	ty + th,
			tx + tw,	ty + th,
			tx,	ty,
			tx + tw,	ty };
		
		glTranslatef(0, -pchar->h, 0.0);
		glTranslatef(pchar->x_offset, -pchar->y_offset, 0.0);			
		
		glVertexPointer(3, GL_FLOAT, 0, vertices);
		glTexCoordPointer(2, GL_FLOAT, 0, coordinates);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		
		glTranslatef(-pchar->x_offset, pchar->y_offset, 0.0);			
		glTranslatef(0, pchar->h, 0.0);
		
		
		glTranslatef(pchar->x_advance-pchar->x_offset, 0, 0.0);			
	}
	

	glPopMatrix();
}

void font_free(font_t *fnt)
{
	tex2d_release(fnt->tex_id);
	fnt->tex_id = -1;
}