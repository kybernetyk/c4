/*
 *  vector_2d.h
 *  c4
 *
 *  Created by jrk on 8/2/11.
 *  Copyright 2011 flux forge. All rights reserved.
 *
 */
#pragma once

typedef struct vector2d_t
{
	double x;
	double y;
} vec2d_t;

extern vec2d_t vec2d_make(double x, double y);

typedef struct size2d_t
{
	double w;
	double h;
} size2d_t;

extern size2d_t size2d_make(double w, double h);

typedef struct rect_t
{
	double x;
	double y;

	double w;
	double h;
} rect_t;

extern rect_t rect_make(double x, double y, double w, double h);