/*
 *  renderer.c
 *  c4
 *
 *  Created by jrk on 8/2/11.
 *  Copyright 2011 Jaroslaw Szpilewski. All rights reserved.
 *
 */

#include "renderer.h"
#include "ogl.h"


static struct camera_t
{
	double x;
	double y;
	double rot_a;
} camera;

static struct viewport_t
{
	double w_pixels;
	double h_pixels;
	
	double w_units;
	double h_units;
	
	double x_conv;		//conversion factors for world<->screen space conversion
	double y_conv;
	double scale;
} viewport;

static void setup_viewport_and_projection(int vp_w_pixels, int vp_h_pixels, float vp_w_units, float vp_h_units)
{
	viewport.w_pixels = vp_w_pixels;
	viewport.h_pixels = vp_h_pixels;
	viewport.w_units = vp_w_units;
	viewport.h_units = vp_h_units;
	viewport.x_conv = (double)vp_w_units/(double)vp_w_pixels;
	viewport.y_conv = (double)vp_h_units/(double)vp_h_pixels;
	
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	glViewport(0,0,vp_w_pixels, vp_h_pixels);
	
	glEnable(GL_TEXTURE_2D); //enable texture mapping
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f); //Black Background

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_ALPHA_TEST);
	
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );//GL_ONE_MINUS_SRC_ALPHA);      // Enable Alpha Blending (disable alpha testing)
	glEnable(GL_BLEND);
	
	glShadeModel(GL_SMOOTH); //enables smooth color shading
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity(); //Reset projection matrix
	
	//glOrthof(-viewport_width_in_meters/2.0 , viewport_width_in_meters/2.0 , -viewport_height_in_meters/2.0, viewport_height_in_meters/2.0 , -10.0 , 10.0 );
	
	glOrthof(0, vp_w_units, 0, vp_h_units, -10.0 , 10.0 );
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	
	glEnableClientState( GL_VERTEX_ARRAY);
	glEnableClientState( GL_TEXTURE_COORD_ARRAY );
	glEnableClientState( GL_COLOR_ARRAY);
	
	glEnable( GL_TEXTURE_2D);
}


bool renderer_init(double scale)
{
	viewport.scale = scale;
	
	double screen_size_x = SCREEN_W * scale;
	double screen_size_y = SCREEN_H * scale;	//change to 280 for a 40px high empty strip [eg for an ad banner]
	
	double viewport_size_x = SCREEN_W;// / pixeltometerratio;//viewport_size_x / xyratio;
	double viewport_size_y = SCREEN_H;	
	
	
	setup_viewport_and_projection(screen_size_x, screen_size_y, viewport_size_x, viewport_size_y);
	camera.rot_a = 0.0;
	camera.x = viewport_size_x/2.0;
	camera.y = viewport_size_y/2.0;

	return true;
}

void renderer_begin_frame(void)
{
	glClearColor(0.0f, 0.0f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	
	glLoadIdentity();
	
	
	glTranslatef( viewport.w_units/2.0, viewport.h_units/2.0, 0);
	
	if (camera.rot_a != 0.0)
		glRotatef(camera.rot_a, 0.0, 0.0, 1.0);
	
	glTranslatef( -camera.x , -camera.y, 0.0);
	
}

void renderer_end_frame(void)
{
	
}

void renderer_release(void)
{
	
}

#pragma mark -
#pragma mark coord conv
vec2d_t renderer_screen_to_world(vec2d_t vec)
{
#if __IPHONE_OS_VERSION_MIN_REQUIRED >= 30000
#else
	vec.y = SCREEN_H * viewport.scale - (float)vec.y;
#endif
	
	vec2d_t ret;
	ret.x = vec.x * viewport.x_conv + camera.x - viewport.w_units/2; //+ camera offset etc
	ret.y = vec.y * viewport.y_conv + camera.y - viewport.h_units/2;
	
	return ret;
}