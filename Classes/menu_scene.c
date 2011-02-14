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
static le_render_system_t rs;

static audio_id music;
static audio_id sound;

static int scene_init(scene_t *scene)
{
	em_init(&mgr);
	render_system_init(&rs, &mgr);
	
	music = audio_music_load("music.mp3");
	sound = audio_sound_load("click.mp3");
	
	
	le_entity_t *ent = em_create_entity(&mgr);
	le_component_t *comp = em_add_component_to_entity(&mgr, ent, COMP_FAMILY_POSITION);
	comp_position_init(comp, vec2d_make(g_sysconfig.screen_w/2, g_sysconfig.screen_h/2), -4.0);
	
	comp = em_add_component_to_entity(&mgr, ent, COMP_FAMILY_RENDERABLE);
	comp_quad_init(comp, "menu_back.png");


	ent = em_create_entity(&mgr);
	comp = em_add_component_to_entity(&mgr, ent, COMP_FAMILY_POSITION);
	comp_position_init(comp, vec2d_make(100, 100), -3.0);
	
	comp = em_add_component_to_entity(&mgr, ent, COMP_FAMILY_RENDERABLE);
	comp_atlas_quad_init(comp, "bubbles.png", rect_make(0.0, 0.0, 41.0, 41.0));
	

	ent = em_create_entity(&mgr);
	comp = em_add_component_to_entity(&mgr, ent, COMP_FAMILY_POSITION);
	comp_position_init(comp, vec2d_make(g_sysconfig.screen_w/2, g_sysconfig.screen_h/2), -2.0);
	
	comp = em_add_component_to_entity(&mgr, ent, COMP_FAMILY_RENDERABLE);
	comp_text_init(comp, "impact20.fnt", "oh hai!");
	comp_text_set_text(comp_get_userdata(comp, comp_text_t), "fick dich k?");
	
	audio_music_play(music);
	
	return 0;
}

static void scene_update(scene_t *scene, double dt)
{
	render_system_update(&rs, dt);
	
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
	render_system_render(&rs);
}

static int scene_free(scene_t *scene)
{
	
	render_system_shutdown(&rs);
	printf("menuscene free\n");
	
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