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

extern bool game_init(void);
extern void game_tick(void);
extern void game_render(void);
extern void game_end(void);

extern void game_set_next_scene (scene_t scene);