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


static int TICKS_PER_SECOND;
static int SKIP_TICKS;
static int MAX_FRAMESKIP= 5;
static float FIXED_DELTA;
static unsigned int next_game_tick;
static int loops;
static int paused;
static timer_t timer;
static scene_t current_scene;
static scene_t next_scene;

static bool next_scene_queued = false;

static char fps_str[255];
static le_font_t fps_font;

void game_set_next_scene(scene_t scene)
{
	next_scene = scene;
	next_scene_queued = true;
}

bool game_init(scene_t initial_scene)
{
	TICKS_PER_SECOND = g_sysconfig.desired_fps;
	SKIP_TICKS = 1000 / TICKS_PER_SECOND;
	MAX_FRAMESKIP = 5;
	FIXED_DELTA = (1.0/TICKS_PER_SECOND);
	paused = 0;

		
	next_game_tick = timer_get_tick_count();
	timer_update(&timer);
	
	font_load("impact20.fnt", &fps_font);
	fps_font.ri.pos = vec2d_make(0.0, g_sysconfig.screen_h);
	fps_font.ri.anchor_point = vec2d_make(0.0, 1.0);
	
	current_scene = initial_scene;
	current_scene.init_func(&current_scene);
	next_scene_queued = false;

	return true;
}

void game_tick(void)
{
	if (next_scene_queued)
	{
		next_scene_queued = false;
		current_scene.free_func(&current_scene);

		current_scene = next_scene;
		next_game_tick = timer_get_tick_count();
		current_scene.init_func(&current_scene);
	}
	
	timer_update(&timer);
	timer_get_fps(&timer);
	sprintf(fps_str, "fps: %.2f", timer.fps);
	
	loops = 0;
	while (timer_get_tick_count() > next_game_tick && loops < MAX_FRAMESKIP)
	{
		input_update();
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
	renderer_begin_frame();
	current_scene.render_func(&current_scene);
	
	font_render(&fps_font, fps_str);
	
	renderer_end_frame();
}

void game_end(void)
{
	//current scene end
	current_scene.free_func(&current_scene);
	font_free(&fps_font);
	renderer_shutdown();
	audio_shutdown();
}