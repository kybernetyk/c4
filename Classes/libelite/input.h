/*
 *  input.h
 *  c4
 *
 *  Created by jrk on 11/2/11.
 *  Copyright 2011 Jaroslaw Szpilewski. All rights reserved.
 *
 */

#pragma once
#include <stdbool.h>
#include "vec2d.h"

extern bool fs_input_init(void);
extern void fs_input_update(void);
extern void fs_input_shutdown(void);

extern void fs_input_set_touch_active(bool b);
extern bool fs_input_is_touch_active(void);

extern void fs_input_set_touch_up_received(bool b);
extern bool fs_input_touch_up_received(void);

extern void fs_input_set_touch_location(vec2d_t vec);
extern void fs_input_get_touch_location(vec2d_t *outvec);			//current location of the touch

extern void fs_input_set_initial_touch_location(vec2d_t vec);
extern void fs_input_get_initial_touch_location(vec2d_t *outvec);	//first registered location for touch


//intersection tests
extern bool fs_input_touch_inside_rect(rect_t *rc);