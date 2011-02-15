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

typedef struct fs_font_t
{
	fs_renderinfo_t ri;
	bm_font fnt;
	fs_tex2d_id tex_id;
	
	bool is_dynamic;
} fs_font_t;

extern fs_font_t *fs_font_new(void);
extern bool fs_font_load(const char *filename, fs_font_t *font_out);
extern void fs_font_render(fs_font_t *fnt, const char *text);
extern void fs_font_free(fs_font_t *fnt);