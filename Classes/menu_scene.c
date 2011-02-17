/*
 *  menu_scene.c
 *  c4
 *
 *  Created by jrk on 8/2/11.
 *  Copyright 2011 Jaroslaw Szpilewski. All rights reserved.
 *
 */
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "elite.h"
#include "menu_scene.h"
#include <unistd.h>


typedef struct menu_scene_state
{
	le_entity_manager_t mgr;
	le_garbage_system_t gs;
	le_action_system_t as;
	le_render_system_t rs;
	
	le_particle_system_t ps;
	
	
	le_entity_t *bubble;
	le_entity_t *minyx;
	le_entity_t *oh_hai;
	
	le_entity_t *firetail;
	fs_audio_id sound;
	
	le_entity_t *time;
	
	double time_counter;
	
} menu_scene_state;


static int scene_init(scene_t *scene)
{
	menu_scene_state *state = scene->user_data;
	state->sound = fs_audio_sound_load("click.mp3");

	printf("menu scene init %p ...\n", scene);
	em_init(&state->mgr);
	action_system_init(&state->as, &state->mgr);
	
	garbage_system_init(&state->gs, &state->mgr);
	
	render_system_init(&state->rs, &state->mgr);
	particle_system_init(&state->ps, &state->mgr);
	
	
	//background
	le_entity_t *ent = em_create_entity(&state->mgr);
	le_component_header_t *comp = entity_add_component(ent, COMP_FAMILY_POSITION);
	comp_position_init(comp, vec2d_make(g_sysconfig.screen_w/2, g_sysconfig.screen_h/2), -4.0);
	
	comp = entity_add_component(ent, COMP_FAMILY_RENDERABLE);
	comp_quad_init(comp, "menu_back.png");


	//text label
	state->oh_hai = em_create_entity(&state->mgr);
	comp = entity_add_component(state->oh_hai, COMP_FAMILY_POSITION);
	comp_position_init(comp, vec2d_make(g_sysconfig.screen_w/2, g_sysconfig.screen_h/2), -2.0);
	
	comp = entity_add_component(state->oh_hai, COMP_FAMILY_RENDERABLE);
	comp_text_init(comp, "impact20.fnt", "oh hai!");
	comp_text_set_text(comp, "minyx games");

	
	//particle left
	ent = em_create_entity(&state->mgr);
	comp = entity_add_component(ent, COMP_FAMILY_POSITION);
	comp_position_init(comp, vec2d_make(0,0), -1.0);
	
	comp = entity_add_component(ent, COMP_FAMILY_RENDERABLE);
	comp_pe_init(comp, "stars.pex");
	
	//particle right
	ent = em_create_entity(&state->mgr);
	comp = entity_add_component(ent, COMP_FAMILY_POSITION);
	comp_position_init(comp, vec2d_make(g_sysconfig.screen_w,0), -1.0);
	
	comp = entity_add_component(ent, COMP_FAMILY_RENDERABLE);
	comp_pe_init(comp, "stars.pex");
	

	//bubble

	for (int i = 0; i < 1; i++)
	{
		state->bubble = em_create_entity(&state->mgr);
		comp = entity_add_component(state->bubble, COMP_FAMILY_POSITION);
		comp_position_init(comp, vec2d_make(rand()%320, rand()%480), -3.0);
		
		comp = entity_add_component(state->bubble, COMP_FAMILY_RENDERABLE);
		cd_atlas_quad_t *aq = comp_atlas_quad_init(comp, "bubbles.png", rect_make(0.0, 0.0, 41.0, 41.0));
		aq->atlas_quad->ri.scale = vec2d_make(2.0, 2.0);
		aq->atlas_quad->ri.alpha = 0.3;
	}
	
	//das minyx
	state->minyx = em_create_entity(&state->mgr);
	comp = entity_add_component(state->minyx, COMP_FAMILY_POSITION);
	comp_position_init(comp, vec2d_make(g_sysconfig.screen_w/2, g_sysconfig.screen_h/2), 1.0);

	comp = entity_add_component(state->minyx, COMP_FAMILY_RENDERABLE);
	comp_quad_init(comp, "minyx_bw.png");
	
	
	//firetail
	state->firetail = em_create_entity(&state->mgr);
	comp = entity_add_component(state->firetail, COMP_FAMILY_RENDERABLE);
	comp_pe_init(comp, "cool.pex");
	
	comp = entity_add_component(state->firetail, COMP_FAMILY_POSITION);
	comp_position_init(comp, vec2d_make(g_sysconfig.screen_w/2, g_sysconfig.screen_w/2), 1.0);
	
	//rainshit
	ent = em_create_entity(&state->mgr);
	comp = entity_add_component(ent, COMP_FAMILY_RENDERABLE);
	comp_pe_init(comp, "tss.pex");
	
	comp = entity_add_component(ent, COMP_FAMILY_POSITION);
	comp_position_init(comp, vec2d_make(g_sysconfig.screen_w/2, g_sysconfig.screen_h), 1.0);
	
	//time label
	state->time = em_create_entity(&state->mgr);
	comp = entity_add_component(state->time, COMP_FAMILY_POSITION);
	comp_position_init(comp, vec2d_make(g_sysconfig.screen_w, g_sysconfig.screen_h), -2.0);
	
	comp = entity_add_component(state->time, COMP_FAMILY_RENDERABLE);
	cd_text_t *t = comp_text_init(comp, "impact20.fnt", "00:00:00");
	t->font->ri.anchor_point = vec2d_make(1.0, 1.0);
	state->time_counter = 0.0;
	
	
	
	action_header_t *acthdr = action_system_add_action_to_entity(&state->as, state->time);
	action_move_to_init(acthdr, vec2d_make(g_sysconfig.screen_w/2 , g_sysconfig.screen_h/2));
	acthdr->duration = 10.0;
	
	
//	cd_actn_move_to_t *acn = action_move_to_init(comp, vec2d_make(g_sysconfig.screen_w/2, g_sysconfig.screen_h/2));
//	acn->header.duration = 10.0;
	
	
	return 0;
}

static void scene_pre_frame(scene_t *scene)
{
}

static void scene_update(scene_t *scene, double dt)
{
	//printf("%f\n", dt);
	menu_scene_state *state = scene->user_data;
	garbage_system_collect(&state->gs); 	//collect previous frame's garbage
	
	action_system_update(&state->as, dt);
	particle_system_update(&state->ps, dt);
	
	cd_position_t *pos = entity_get_component(state->minyx, COMP_FAMILY_POSITION);
	pos->rot += dt * 360.0;
	
	pos = entity_get_component(state->firetail, COMP_FAMILY_POSITION);

	pos->pos.x = g_sysconfig.screen_w/2 +  100.0 * sin(timer_get_double_time()*5.0);
	pos->pos.y = g_sysconfig.screen_h/2 +  100.0 * cos(timer_get_double_time()*5.0);
	
	pos = entity_get_component(state->oh_hai, COMP_FAMILY_POSITION);
	pos->rot -= dt * 360.0;
	
	state->time_counter+=dt;
	
	char c[255];
	sprintf(c,"%.4f", state->time_counter);
	
	comp_text_set_text(entity_get_component_header(state->time, COMP_FAMILY_RENDERABLE), c);
	if (state->time_counter > 1.0)
	{	
		state->time_counter = 0.0;
		fs_audio_sound_play(state->sound);
	}
	
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
			printf("playing sound %i ...\n", state->sound);
			//fs_audio_sound_play(state->sound);
			game_pop_scene();
			//entity_add_component(state->bubble, COMP_FAMILY_GARBAGE);
			
		}
	}
	
}

static void scene_render(scene_t *scene)
{
	menu_scene_state *state = scene->user_data;
	render_system_render(&state->rs);
	
}

//clean up post frame
static void scene_post_frame(scene_t *scene)
{
	menu_scene_state *state = scene->user_data;
	em_update(&state->mgr);	//set mgr->is_dirty to false
	
}

static int scene_free(scene_t *scene)
{
	menu_scene_state *state = scene->user_data;
	printf("menuscene free %p ...\n", scene);
	action_system_shutdown(&state->as);
	garbage_system_shutdown(&state->gs);
	particle_system_shutdown(&state->ps);
	render_system_shutdown(&state->rs);
	em_shutdown(&state->mgr);

	free(scene->user_data);
	
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
	ret.user_data = calloc(1, sizeof(menu_scene_state));
	
	return ret;
}