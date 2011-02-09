/*
 *  renderinfo.h
 *  c4
 *
 *  Created by jrk on 8/2/11.
 *  Copyright 2011 Jaroslaw Szpilewski. All rights reserved.
 *
 */
#pragma once
//generic render info for stuff that needs to be rendered
#include "vector2d.h"

typedef struct renderinfo_t
{
	vec2d_t pos;			//position on screen
	size2d_t size;			//size of the underlying resource [how big shall the object be on the screen]
	double zval;			//z coordinate (used for z ordering)
	double alpha;			//alpha 0.0 = transparent, 1.0 = opaque
	double rot_a;			//rotation angle
	vec2d_t scale;			//scale in x,y
	vec2d_t anchor_point;	//anchor point 0,0 = lower left, 0.5,0.5 = center, 1.0,1.0 = upper right
} renderinfo_t;

/* set ups render matrix for the given renderinfo */
extern void renderinfo_transform(renderinfo_t *ri);