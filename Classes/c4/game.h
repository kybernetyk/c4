/*
 *  game.h
 *  c4
 *
 *	your game loop	
 *
 *  Created by jrk on 8/2/11.
 *  Copyright 2011 flux forge. All rights reserved.
 *
 */
#pragma once
#include "scene.h"

extern int game_init(void);
extern void game_tick(void);
extern void game_render(void);
extern int game_end(void);

extern void game_set_next_scene (scene_t scene);