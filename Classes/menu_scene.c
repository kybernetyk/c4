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

static fs_audio_id music;
static fs_audio_id sound;

static le_entity_t *bubble;
static le_entity_t *minyx;
static le_entity_t *oh_hai;

static le_entity_t *firetail;

static int scene_init(scene_t *scene)
{
	em_init(&mgr);
	garbage_system_init(&gs, &mgr);
	render_system_init(&rs, &mgr);
	particle_system_init(&ps, &mgr);
	
	music = fs_audio_music_load("music.mp3");
	sound = fs_audio_sound_load("click.mp3");
	
	//background
	le_entity_t *ent = em_create_entity(&mgr);
	le_component_t *comp = entity_add_component(ent, COMP_FAMILY_POSITION);
	comp_position_init(comp, vec2d_make(g_sysconfig.screen_w/2, g_sysconfig.screen_h/2), -4.0);
	
	comp = entity_add_component(ent, COMP_FAMILY_RENDERABLE);
	comp_quad_init(comp, "menu_back.png");


	//text label
	oh_hai = em_create_entity(&mgr);
	comp = entity_add_component(oh_hai, COMP_FAMILY_POSITION);
	comp_position_init(comp, vec2d_make(g_sysconfig.screen_w/2, g_sysconfig.screen_h/2), -2.0);
	
	comp = entity_add_component(oh_hai, COMP_FAMILY_RENDERABLE);
	comp_text_init(comp, "impact20.fnt", "oh hai!");
	comp_text_set_text(comp, "minyx games");

	
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

	for (int i = 0; i < 32; i++)
	{
		bubble = em_create_entity(&mgr);
		comp = entity_add_component(bubble, COMP_FAMILY_POSITION);
		comp_position_init(comp, vec2d_make(rand()%320, rand()%480), -3.0);
		
		comp = entity_add_component(bubble, COMP_FAMILY_RENDERABLE);
		comp_atlas_quad_init(comp, "bubbles.png", rect_make(0.0, 0.0, 41.0, 41.0));
		
	}
	
	//das minyx
	minyx = em_create_entity(&mgr);
	comp = entity_add_component(minyx, COMP_FAMILY_POSITION);
	comp_position_init(comp, vec2d_make(g_sysconfig.screen_w/2, g_sysconfig.screen_h/2), 1.0);

	comp = entity_add_component(minyx, COMP_FAMILY_RENDERABLE);
	comp_quad_init(comp, "minyx_bw.png");
	
	
	//firetail
	firetail = em_create_entity(&mgr);
	comp = entity_add_component(firetail, COMP_FAMILY_RENDERABLE);
	comp_pe_init(comp, "cool.pex");
	
	comp = entity_add_component(firetail, COMP_FAMILY_POSITION);
	comp_position_init(comp, vec2d_make(g_sysconfig.screen_w/2, g_sysconfig.screen_w/2), 1.0);
	
	//rainshit
	ent = em_create_entity(&mgr);
	comp = entity_add_component(ent, COMP_FAMILY_RENDERABLE);
	comp_pe_init(comp, "tss.pex");
	
	comp = entity_add_component(ent, COMP_FAMILY_POSITION);
	comp_position_init(comp, vec2d_make(g_sysconfig.screen_w/2, g_sysconfig.screen_h), 1.0);
	

	fs_audio_music_play(music);
	
	return 0;
}

static void scene_pre_frame(scene_t *scene)
{
}

static void scene_update(scene_t *scene, double dt)
{
	particle_system_update(&ps, dt);
	
	cd_position_t *pos = entity_get_component_data(minyx, COMP_FAMILY_POSITION);
	pos->rot += dt * 360.0;
	
	pos = entity_get_component_data(firetail, COMP_FAMILY_POSITION);

	pos->pos.x = g_sysconfig.screen_w/2 +  100.0 * sin(timer_get_double_time()*5.0);
	pos->pos.y = g_sysconfig.screen_h/2 +  100.0 * cos(timer_get_double_time()*5.0);
	
	pos = entity_get_component_data(oh_hai, COMP_FAMILY_POSITION);
	pos->rot -= dt * 360.0;
	
	if (fs_input_touch_up_received())
	{	
		if (fs_input_touch_inside_rect(&(rect_t)
									{
										.x = 0,
										.y = 0,
										.w = 256,
										.h = 256
									}
									))
		{	
			printf("playing sound %i ...\n", sound);
			fs_audio_sound_play(sound);
			
			//entity_add_component(bubble, COMP_FAMILY_GARBAGE);
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