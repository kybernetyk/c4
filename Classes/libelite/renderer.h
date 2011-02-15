/*
 *  renderer.h
 *  c4
 *
 *  Created by jrk on 8/2/11.
 *  Copyright 2011 Jaroslaw Szpilewski. All rights reserved.
 *
 */
#pragma once
#include <stdbool.h>

#include "vec2d.h"

extern bool fs_renderer_init(void);
extern void fs_renderer_begin_frame(void);
extern void fs_renderer_end_frame(void);
extern void fs_renderer_shutdown(void);

extern vec2d_t fs_renderer_screen_to_world(vec2d_t vec);
