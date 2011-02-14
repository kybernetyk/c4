/*
 *  comp_position.h
 *  c4
 *
 *  Created by jrk on 14/2/11.
 *  Copyright 2011 Jaroslaw Szpilewski. All rights reserved.
 *
 */

#pragma once
#include "elite.h"

#define POS_SUB_ROTPOS 0x01

typedef struct comp_position_t
{
	vec2d_t pos;
	double rot;
} comp_position_t;


extern comp_position_t *comp_position_init(le_component_t *comp, vec2d_t pos);