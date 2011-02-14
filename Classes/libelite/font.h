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

typedef struct le_font_t
{
	renderinfo_t ri;
	bm_font fnt;
	tex2d_id tex_id;
	
	bool is_dynamic;
} le_font_t;

extern le_font_t *font_new(void);
extern bool font_load(const char *filename, le_font_t *font_out);
extern void font_render(le_font_t *fnt, const char *text);
extern void font_free(le_font_t *fnt);