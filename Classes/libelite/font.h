/*
 *  font.h
 *  c4
 *
 *  Created by jrk on 9/2/11.
 *  Copyright 2011 Jaroslaw Szpilewski. All rights reserved.
 *
 */

#pragma once
#include  <stdbool.h>
#include "renderinfo.h"
#include "texture.h"
#include "bm_font.h"

typedef struct font_t
{
	renderinfo_t ri;
	bm_font fnt;
	tex2d_id tex_id;
} font_t;

extern bool font_load(const char *filename, font_t *font_out);
extern void font_render(font_t *fnt, const char *text);
extern void font_free(font_t *fnt);