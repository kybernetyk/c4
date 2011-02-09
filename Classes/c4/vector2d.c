/*
 *  vector_2d.c
 *  c4
 *
 *  Created by jrk on 8/2/11.
 *  Copyright 2011 flux forge. All rights reserved.
 *
 */

#include "vector2d.h"

vec2d_t vec2d_make (double x, double y)
{
	return (vec2d_t){
		.x = x,
		.y = y
	};
}

size2d_t size2d_make (double w, double h)
{
	return (size2d_t){
		.w = w,
		.h = h
	};
}


rect_t rect_make(double x, double y, double w, double h)
{
	return (rect_t){
		.x = x,
		.y = y,
		.w = w,
		.h = h
	};
}