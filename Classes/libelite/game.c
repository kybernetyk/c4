/*
 *  game.c
 *  c4
 *
 *  Created by jrk on 8/2/11.
 *  Copyright 2011 Jaroslaw Szpilewski. All rights reserved.
 *
 */

#include "game.h"

#include <stdio.h>
#include <stdbool.h>

#include "elite.h"


double TICKS_PER_SECOND;
double SKIP_TICKS;
double MAX_FRAMESKIP = 5;
double FIXED_DELTA;
double next_game_tick;
unsigned int loops;
int paused;

static timer_t timer;
static scene_t current_scene;
static scene_t next_scene;

static bool next_scene_queued = false;

static char fps_str[255];
static fs_font_t fps_font;

void game_set_next_scene(scene_t scene)
{
	next_scene = scene;
	next_scene_queued = true;
}

bool game_init(scene_t initial_scene)
{
	TICKS_PER_SECOND = g_sysconfig.desired_fps;
	SKIP_TICKS = (1000.0 / TICKS_PER_SECOND);
	MAX_FRAMESKIP = 5;
	FIXED_DELTA = (1.0/TICKS_PER_SECOND);
	paused = 0;
		
	timer_update(&timer);
	
	fs_font_load("impact20.fnt", &fps_font);
	fps_font.ri.pos = vec2d_make(0.0, g_sysconfig.screen_h);
	fps_font.ri.anchor_point = vec2d_make(0.0, 1.0);
	
	current_scene = initial_scene;
	current_scene.init_func(&current_scene);
	next_scene_queued = false;

	next_game_tick = timer_get_tick_count();
	timer.delta = 0.0;
	return true;
}

void game_tick(void)
{
	if (next_scene_queued)
	{
		next_scene_queued = false;
		current_scene.free_func(&current_scene);

		current_scene = next_scene;
		current_scene.init_func(&current_scene);
		
		next_game_tick = timer_get_tick_count();
		timer_update(&timer);
		timer.delta = 0.0;
	}
	
	timer_update(&timer);
	timer_get_fps(&timer);
	sprintf(fps_str, "fps: %.2f", timer.fps);

	//delta based loop (don't use)
//	fs_input_update();
//	current_scene.pre_frame_func(&current_scene);
//	current_scene.update_func(&current_scene, timer.delta);
	

	/* do while instead of just while will remove the jerkiness if we get 0 loops.
	 on the other hand it won't allow for eg. 30fps gameplay on a 60fps device :( 
	 
	 while(t > ...) instead of (while timer_get_tick_count() > ...) seems to work ... time will tell
	 */
	
	loops = 0;
	double t = timer_get_tick_count();
	while (t > next_game_tick && loops < MAX_FRAMESKIP)
	{
		fs_input_update();
		current_scene.pre_frame_func(&current_scene);
		current_scene.update_func(&current_scene, FIXED_DELTA);
		next_game_tick += SKIP_TICKS;
		loops++;
	}
}

void game_render(void)
{
/*#ifdef __ALLOW_RENDER_TO_TEXTURE__
	RenderDevice::sharedInstance()->setRenderTargetBackingTexture();
	RenderDevice::sharedInstance()->beginRender();
	current_scene->render();
	current_scene->frameDone();
	RenderDevice::sharedInstance()->endRender();
	
	RenderDevice::sharedInstance()->setRenderTargetScreen();
	RenderDevice::sharedInstance()->beginRender();
	glTranslatef( (0.5 * SCREEN_W),  (0.5 * SCREEN_H), 0);
	//glRotatef(45.0, 0, 0, 1.0);
	glTranslatef( -(0.5 * SCREEN_W),  -(0.5 * SCREEN_H), 0);
	
	RenderDevice::sharedInstance()->renderBackingTextureToScreen();
	RenderDevice::sharedInstance()->endRender();	
#else
	RenderDevice::sharedInstance()->beginRender();
	current_scene->render();
	current_scene->frameDone();
	RenderDevice::sharedInstance()->endRender();
#endif
*/	
	fs_renderer_begin_frame();
	current_scene.render_func(&current_scene);
	fs_font_render(&fps_font, fps_str);
	
	current_scene.post_frame_func(&current_scene);
	
	fs_renderer_end_frame();
}

void game_end(void)
{
	//current scene end
	current_scene.free_func(&current_scene);
	fs_font_free(&fps_font);
	fs_renderer_shutdown();
	fs_audio_shutdown();
}
