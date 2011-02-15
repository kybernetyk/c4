/*
 *  input.c
 *  c4
 *
 *  Created by jrk on 11/2/11.
 *  Copyright 2011 Jaroslaw Szpilewski. All rights reserved.
 *
 */

#include "input.h"
#include <stdio.h>

static vec2d_t _touch_location;
static vec2d_t _initial_touch_location;
static bool _is_touch_active;
static bool _is_touchup_active;
static bool _touchup_handled;

static bool _touchdown_handled;


bool fs_input_init(void)
{
	_is_touch_active = false;
	_is_touchup_active = false;
	
	return true;
}

void fs_input_shutdown(void)
{
	
}

#if LE_TARGET_IPHONE
void fs_input_update(void)
{
	if (_touchup_handled)
	{
		_is_touchup_active = false;
		_touchup_handled = false;
	}
	
	if (_touchdown_handled)
	{
		_is_touch_active = false;
		_touchdown_handled = false;
	}
}
#else
void input_update(void)
{
	if (_touchup_handled)
	{
		_is_touchup_active = false;
		_touchup_handled = false;
	}
	
	if (_touchdown_handled)
	{
		_is_touch_active = false;
		_touchdown_handled = false;
	}
	
	int x;
	int y;
	Uint8 i = SDL_GetMouseState(&x, &y);
	
	mx3::vector2D v = {x,y};
	v = mx3::RenderDevice::sharedInstance()->coord_convertScreenToWorld (v);
	
	if ( (i & SDL_BUTTON_LEFT) )
	{
		if (!_is_touch_active)
			setInitialTouchLocation (v);
		
		setTouchActive(true);
		setTouchLocation (v);
		
		
		//				printf("x: %f, y: %f\n",v.x,v.y);
	}
	else
	{
		if (_is_touch_active)
			setTouchUpReceived (true);
		
		setTouchLocation (v);
		setTouchActive (false);
	}
	
	//	_touch_location.x = (float)x/960.0*480.0;
	//	_touch_location.y = (640.0-(float)y)/640.0*320.0;
}
#endif

void fs_input_set_touch_active(bool b)
{
	_touchup_handled = false;
	_is_touch_active = b;
}

bool fs_input_is_touch_active(void)
{
	return _is_touch_active;
}

void fs_input_set_touch_up_received(bool b)
{
	_is_touchup_active = b;
	_touchup_handled = false;
}

bool fs_input_touch_up_received(void)
{
	_touchup_handled = true;
	return _is_touchup_active;
}

void fs_input_set_touch_location(vec2d_t vec)
{
	_touch_location = vec;	
	//printf("touch loc: %f,%f\n",vec.x,vec.y);
}

void fs_input_get_touch_location(vec2d_t *outvec)
{
	*outvec = _touch_location;	
}

void fs_input_set_initial_touch_location(vec2d_t vec)
{
	_initial_touch_location = vec;	
}

void fs_input_get_initial_touch_location(vec2d_t *outvec)
{
	*outvec = _initial_touch_location;	
}


bool fs_input_touch_inside_rect(rect_t *rc)
{
//	printf("touch loc: %f,%f\n",_touch_location.x,_touch_location.y);
	return vec2d_inside_rect(&_touch_location, rc);
}