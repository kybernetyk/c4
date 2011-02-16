/*
 *  game.h
 *  c4
 *
 *	your game loop	
 *
 *  Created by jrk on 8/2/11.
 *  Copyright 2011 Jaroslaw Szpilewski. All rights reserved.
 *
 */
#pragma once
#include <stdbool.h>

#include "scene.h"

extern int g_game_paused;

extern bool game_init(scene_t initial_scene);
extern void game_tick(void);
extern void game_render(void);
extern void game_end(void);

extern void game_set_next_scene(scene_t scene);

extern void game_push_scene(scene_t scene);
extern void game_pop_scene(void);

//extern void game_set_paused(bool paused);

extern void game_did_become_active(void);
extern void game_did_enter_background(void);