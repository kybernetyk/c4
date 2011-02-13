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
#include "vector2d.h"

extern bool input_init(void);
extern void input_update(void);
extern void input_shutdown(void);

extern void input_set_touch_active(bool b);
extern bool input_is_touch_active(void);

extern void input_set_touch_up_received(bool b);
extern bool input_touch_up_received(void);

extern void input_set_touch_location(vec2d_t vec);
extern void input_get_touch_location(vec2d_t *outvec);			//current location of the touch

extern void input_set_initial_touch_location(vec2d_t vec);
extern void input_get_initial_touch_location(vec2d_t *outvec);	//first registered location for touch
