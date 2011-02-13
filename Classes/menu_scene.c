/*
 *  menu_scene.c
 *  c4
 *
 *  Created by jrk on 8/2/11.
 *  Copyright 2011 Jaroslaw Szpilewski. All rights reserved.
 *
 */
#include <stdio.h>
#include <math.h>
#include "elite.h"
#include "menu_scene.h"

static le_quad_t p;
static le_quad_t q;
static le_atlas_quad_t r;
static le_font_t f;
static le_particle_emitter_t pe;
static le_particle_emitter_t pe2;

static le_particle_emitter_t pe3;
static le_particle_emitter_t pe4;


static audio_id music;
static audio_id sound;

static int scene_init(scene_t *scene)
{
	quad_load("menu_back.png", &p);
	p.ri.zval = 0;
	p.ri.anchor_point = vec2d_make(0.0, 0.0);
	
	quad_load("minyx_bw.png", &q);
	q.ri.pos.x = g_sysconfig.screen_w/2;
	q.ri.pos.y = g_sysconfig.screen_h/2;
	q.ri.zval = 1.0;
	
	atlas_quad_load("bubbles.png", &r);
	r.ri.pos = vec2d_make(100, 100);
	r.ri.zval = 2.0;
	r.ri.size = size2d_make(41, 41);
	r.src_rect = rect_make(0.0, 0.0, 41.0, 41.0);
	
	font_load("impact20.fnt", &f);
	f.ri.pos = vec2d_make(g_sysconfig.screen_w/2, g_sysconfig.screen_h/2);
	f.ri.zval = 3.0;
	
	particle_emitter_load("cool.pex", &pe);
	pe.ri.zval = 4.0;
	pe.ri.pos = vec2d_make(g_sysconfig.screen_w/2, g_sysconfig.screen_h/2);

	particle_emitter_load("tss.pex", &pe2);
	pe2.ri.zval = 4.0;
	pe2.ri.pos = vec2d_make(g_sysconfig.screen_w/2, g_sysconfig.screen_h);

	particle_emitter_load("stars.pex", &pe3);
	pe3.ri.zval = 4.0;
	pe3.ri.pos = vec2d_make(0, 0);
	
	particle_emitter_load("stars.pex", &pe4);
	pe4.ri.zval = 4.0;
	pe4.ri.pos = vec2d_make(g_sysconfig.screen_w, 0);
	
	
	music = audio_music_load("music.mp3");
	sound = audio_sound_load("click.mp3");
	
	audio_music_play(music);
	
	return 0;
}

static void scene_update(scene_t *scene, double dt)
{
	q.ri.rot_a += 360.0 * dt;
	//r.ri.rot_a += 360.0 * dt;
	
	pe.ri.pos.x = g_sysconfig.screen_w/2 +  100.0 * sin(timer_get_double_time()*5.0);
	pe.ri.pos.y = g_sysconfig.screen_h/2 +  100.0 * cos(timer_get_double_time()*5.0);
	

	particle_emitter_update(&pe, dt);
	particle_emitter_update(&pe2, dt);
	particle_emitter_update(&pe3, dt);
	particle_emitter_update(&pe4, dt);

	
	if (input_touch_up_received())
	{	
		if (input_touch_inside_rect(&(rect_t)
									{
										.x = 0,
										.y = 0,
										.w = 256,
										.h = 256
									}
									))
		{	
			printf("playing sound %i ...\n", sound);
			audio_sound_play(sound);	
		}
	}
}

static void scene_render(scene_t *scene)
{
	quad_render(&p);
	quad_render(&q);
	atlas_quad_render(&r);
	font_render(&f, "oh hai!");
	particle_emitter_render(&pe2);
	particle_emitter_render(&pe);
	particle_emitter_render(&pe3);
	particle_emitter_render(&pe4);

}

static int scene_free(scene_t *scene)
{
	printf("menuscene free\n");
	quad_free(&p);
	quad_free(&q);
	atlas_quad_free(&r);
	font_free(&f);
	
	return 0;
}

scene_t menu_scene_create(void)
{
	scene_t ret;
	ret.init_func = scene_init;
	ret.update_func = scene_update;
	ret.render_func = scene_render;
	ret.free_func = scene_free;
	ret.user_data = NULL;
	
	return ret;
}