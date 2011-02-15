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

static le_entity_manager_t mgr;
static le_garbage_system_t gs;
static le_render_system_t rs;

static le_particle_system_t ps;

static audio_id music;
static audio_id sound;

static le_entity_t *bubble;

static int scene_init(scene_t *scene)
{
	em_init(&mgr);
	garbage_system_init(&gs, &mgr);
	render_system_init(&rs, &mgr);
	particle_system_init(&ps, &mgr);
	
	music = audio_music_load("music.mp3");
	sound = audio_sound_load("click.mp3");
	
	//background
	le_entity_t *ent = em_create_entity(&mgr);
	le_component_t *comp = entity_add_component(ent, COMP_FAMILY_POSITION);
	comp_position_init(comp, vec2d_make(g_sysconfig.screen_w/2, g_sysconfig.screen_h/2), -4.0);
	
	comp = entity_add_component(ent, COMP_FAMILY_RENDERABLE);
	comp_quad_init(comp, "menu_back.png");



	//text label
	ent = em_create_entity(&mgr);
	comp = entity_add_component(ent, COMP_FAMILY_POSITION);
	comp_position_init(comp, vec2d_make(g_sysconfig.screen_w/2, g_sysconfig.screen_h/2), -2.0);
	
	comp = entity_add_component(ent, COMP_FAMILY_RENDERABLE);
	comp_text_init(comp, "impact20.fnt", "oh hai!");
	comp_text_set_text(comp, "fick dich k?");

	
	//particle left
	ent = em_create_entity(&mgr);
	comp = entity_add_component(ent, COMP_FAMILY_POSITION);
	comp_position_init(comp, vec2d_make(0,0), -1.0);
	
	comp = entity_add_component(ent, COMP_FAMILY_RENDERABLE);
	comp_pe_init(comp, "stars.pex");
	
	//particle right
	ent = em_create_entity(&mgr);
	comp = entity_add_component(ent, COMP_FAMILY_POSITION);
	comp_position_init(comp, vec2d_make(g_sysconfig.screen_w,0), -1.0);
	
	comp = entity_add_component(ent, COMP_FAMILY_RENDERABLE);
	comp_pe_init(comp, "stars.pex");
	

	//bubble
	bubble = em_create_entity(&mgr);
	comp = entity_add_component(bubble, COMP_FAMILY_POSITION);
	comp_position_init(comp, vec2d_make(100, 100), -3.0);
	
	comp = entity_add_component(bubble, COMP_FAMILY_RENDERABLE);
	comp_atlas_quad_init(comp, "bubbles.png", rect_make(0.0, 0.0, 41.0, 41.0));
	
	
	
	audio_music_play(music);
	
	return 0;
}

static void scene_pre_frame(scene_t *scene)
{
}

static void scene_update(scene_t *scene, double dt)
{
	particle_system_update(&ps, dt);
	
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
			
			entity_add_component(bubble, COMP_FAMILY_GARBAGE);
		}
	}

}

static void scene_render(scene_t *scene)
{
	render_system_render(&rs);
	
}

//clean up post frame
static void scene_post_frame(scene_t *scene)
{
	em_update(&mgr);	//set mgr->is_dirty to false
	garbage_system_collect(&gs); 	//collect previous frame's garbage
}

static int scene_free(scene_t *scene)
{
	garbage_system_shutdown(&gs);
	particle_system_shutdown(&ps);
	render_system_shutdown(&rs);
	em_shutdown(&mgr);
	printf("menuscene free\n");
	
	return 0;
}

scene_t menu_scene_create(void)
{
	scene_t ret;
	ret.init_func = scene_init;
	ret.pre_frame_func = scene_pre_frame;
	ret.update_func = scene_update;
	ret.render_func = scene_render;
	ret.post_frame_func = scene_post_frame;
	ret.free_func = scene_free;
	ret.user_data = NULL;
	
	return ret;
}