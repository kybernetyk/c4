/*
 *  startup_scene.c
 *  c4
 *
 *  Created by jrk on 8/2/11.
 *  Copyright 2011 Jaroslaw Szpilewski. All rights reserved.
 *
 */
#include <stdio.h>
#include "elite.h"
#include "startup_scene.h"
#include "menu_scene.h"

/* warning! do not set the next scene in the init method! do it in update */

static int scene_init(scene_t *scene)
{
	return 0;
}

static void scene_update(scene_t *scene, double dt)
{
	printf("startup %p fired!\n", scene);
	printf("setting next scene to menu scene ...\n");
	game_set_next_scene(menu_scene_create());
}

static void scene_render(scene_t *scene)
{
}

static int scene_free(scene_t *scene)
{
	return 0;
}

scene_t startup_scene_create(void)
{
	scene_t ret;
	ret.init_func = scene_init;
	ret.update_func = scene_update;
	ret.render_func = scene_render;
	ret.free_func = scene_free;
	ret.user_data = NULL;
	
	return ret;
}