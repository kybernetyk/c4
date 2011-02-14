/*
 *  renderable.h
 *  c4
 *
 *  Created by jrk on 14/2/11.
 *  Copyright 2011 Jaroslaw Szpilewski. All rights reserved.
 *
 */

#pragma once
#include "elite.h"

extern le_quad_t *comp_quad_init(le_component_t *comp, const char *filename);
extern le_atlas_quad_t *comp_atlas_quad_init(le_component_t *comp, const char *filename, rect_t src);
extern le_font_t *comp_font_init(le_component_t *comp, const char *filename);
extern le_particle_emitter_t *comp_pe_init(le_component_t *comp, const char *filename);