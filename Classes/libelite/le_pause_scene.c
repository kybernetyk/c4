/*
 *  le_pause_scene.c
 *  c4
 *
 *  Created by jrk on 16/2/11.
 *  Copyright 2011 Jaroslaw Szpilewski. All rights reserved.
 *
 */

#include "le_pause_scene.h"

#include <stdio.h>
#include "elite.h"
#include "le_pause_scene.h"

/* warning! do not set the next scene in the init method! do it in update */

static int scene_init(scene_t *scene)
{
	return 0;
}

static void scene_pre_frame(scene_t *scene)
{

}

static void scene_update(scene_t *scene, double dt)
{
	printf("pause scene!\n");
}

static void scene_render(scene_t *scene)
{
}

static void scene_post_frame(scene_t *scene)
{
}


static int scene_free(scene_t *scene)
{
	return 0;
}

scene_t le_pause_scene_create(void)
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