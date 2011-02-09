/*
 *  renderer.h
 *  c4
 *
 *  Created by jrk on 8/2/11.
 *  Copyright 2011 Jaroslaw Szpilewski. All rights reserved.
 *
 */
#pragma once
#include "vector2d.h"

extern int renderer_init(double scale);
extern void renderer_begin_frame(void);
extern void renderer_end_frame(void);
extern void renderer_release(void);

extern vec2d_t renderer_screen_to_world(vec2d_t vec);
