/*
 *  game.c
 *  c4
 *
 *  Created by jrk on 8/2/11.
 *  Copyright 2011 Jaroslaw Szpilewski. All rights reserved.
 *
 */

#include "game.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include "elite.h"


static double TICKS_PER_SECOND;
static double SKIP_TICKS;
static double MAX_FRAMESKIP = 5;
static double FIXED_DELTA;
static double next_game_tick;
static unsigned int loops;

//static int game_paused = 0;

static timer_t timer;


#define S_STACK_SIZE 32

#define S_NONE 0x00
#define S_SET 0x01
#define S_PUSHED 0x02
#define S_POPPED 0x03

static int scene_queue_state = S_NONE;

static char fps_str[255];
static fs_font_t fps_font;

static int scene_stack_pointer = 0;
static int push_counter = 0;
static int pop_counter = 0;
static scene_t scene_stack[S_STACK_SIZE];
static scene_t push_stack[S_STACK_SIZE];
static scene_t next_scene;
static scene_t *current_scene;

//void game_set_paused(bool paused)
//{
//	if (paused)
//		game_paused++;
//	else
//		game_paused--;
//	
//	if (game_paused <= 0)
//	{
//		game_paused = 0;
//		next_game_tick = timer_get_tick_count();
//		timer_update(&timer);
//		timer_update(&timer);
//	}
//}

#pragma mark -
#pragma mark focus handler
void game_did_become_active(void)
{
	next_game_tick = timer_get_tick_count();
	timer_update(&timer);
	timer_update(&timer);
}

extern void game_did_enter_background(void)
{
	
}


#pragma mark -
#pragma mark scene stack
void game_set_next_scene(scene_t scene)
{
	next_scene = scene;
	scene_queue_state = S_SET;
}

void game_push_scene(scene_t scene)
{
	//assert(scene_queue_state == S_NONE);
	//make multiple pushes/pops per frame possible in the future ... for now we will just explode
	
	
	scene_stack_pointer++;
	if (scene_stack_pointer >= S_STACK_SIZE)
	{
		printf("You fucked up the stack!\n");
		abort();
		return;
	}
	scene_stack[scene_stack_pointer] = scene;

	
	scene_t *p = &scene_stack[scene_stack_pointer];
	p->init_func(p);
	printf("pushing scene %p\n", p);
	
	
	

	scene_queue_state = S_PUSHED;
}

void game_pop_scene(void)
{
	//assert(scene_queue_state == S_NONE);
	//make multiple pushes/pops per frame possible in the future ... for now we will just explode
	printf("popping scene ...\n");
	
	scene_queue_state = S_POPPED;
	pop_counter++;
}

#pragma mark -
#pragma mark game lifetime
bool game_init(scene_t initial_scene)
{
	TICKS_PER_SECOND = g_sysconfig.desired_fps;
	SKIP_TICKS = (1000.0 / TICKS_PER_SECOND);
	MAX_FRAMESKIP = 5;
	FIXED_DELTA = (1.0/TICKS_PER_SECOND);
//	game_paused = 0;
		
	timer_update(&timer);
	timer_update(&timer);
	
	fs_font_load("impact20.fnt", &fps_font);
	fps_font.ri.pos = vec2d_make(0.0, g_sysconfig.screen_h);
	fps_font.ri.anchor_point = vec2d_make(0.0, 1.0);
	
	scene_stack_pointer = 0;
	scene_stack[scene_stack_pointer] = initial_scene;
	current_scene = &scene_stack[scene_stack_pointer];
	scene_stack[scene_stack_pointer].init_func(&scene_stack[scene_stack_pointer]);
	scene_queue_state = S_NONE;


	next_game_tick = timer_get_tick_count();
	timer.delta = 0.0;
	return true;
}

void game_tick(void)
{
	if (scene_queue_state != S_NONE)
	{
		switch (scene_queue_state)
		{
			case S_SET:
				current_scene->free_func(current_scene);
				scene_stack[scene_stack_pointer] = next_scene;
				current_scene = &scene_stack[scene_stack_pointer];
				current_scene->init_func(current_scene);
				break;
			case S_PUSHED:
				current_scene = &scene_stack[scene_stack_pointer];
				break;
			case S_POPPED:
				for (int i = 0; i < pop_counter; i++)
				{
					scene_stack_pointer--;
					if (scene_stack_pointer < 0)
					{
						printf("You fucked up the stack!\n");
						abort();
						return;
					}
					current_scene->free_func(current_scene);
					current_scene = &scene_stack[scene_stack_pointer];
				}
				pop_counter = 0;
				break;
			default:
				abort();
				break;
		}
		scene_queue_state = S_NONE;
	
		printf("current scene is: %p ...\n", current_scene);
		next_game_tick = timer_get_tick_count();
		timer_update(&timer);
		timer_update(&timer);
	}
	
	timer_update(&timer);
	timer_get_fps(&timer);
	sprintf(fps_str, "fps: %.2f", timer.fps);

	//delta based loop (don't use)
	fs_input_update();
	current_scene->pre_frame_func(current_scene);
	current_scene->update_func(current_scene, timer.delta);
	

	/* do while instead of just while will remove the jerkiness if we get 0 loops.
	 on the other hand it won't allow for eg. 30fps gameplay on a 60fps device :( 
	 
	 while(t > ...) instead of (while timer_get_tick_count() > ...) seems to work ... time will tell
	 */

	/*current_scene->pre_frame_func(current_scene);
	
	loops = 0;
	double t = timer_get_tick_count();
	while (t > next_game_tick && loops < MAX_FRAMESKIP)
	{
		fs_input_update();
	
		current_scene->update_func(current_scene, FIXED_DELTA);
		next_game_tick += SKIP_TICKS;
		loops++;
	}*/
//	printf("%i\n", loops);
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
	current_scene->render_func(current_scene);
	fs_font_render(&fps_font, fps_str);
	
	
	current_scene->post_frame_func(current_scene);
	fs_renderer_end_frame();
}

void game_end(void)
{
	//current scene end
	current_scene->free_func(current_scene);
	fs_font_free(&fps_font);
	fs_renderer_shutdown();
	fs_audio_shutdown();
}
