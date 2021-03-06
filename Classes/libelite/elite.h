/*
 *  elite.h
 *  c4
 *
 *  Created by jrk on 13/2/11.
 *  Copyright 2011 Jaroslaw Szpilewski. All rights reserved.
 *
 */
#pragma once
#include <stdbool.h>

#include "sys_config.h"
#include "scene.h"

#include "renderer.h"
#include "renderinfo.h"
#include "texture.h"
#include "quad.h"
#include "atlas_quad.h"
#include "font.h"
#include "particle_emitter.h"

#include "input.h"
#include "audio.h"

#include "timer.h"
#include "util.h"
#include "hash.h"

#include "entity_manager.h"
#include "entity.h"
#include "component.h"
#include "garbage_system.h"
#include "action_system.h"
#include "render_system.h"
#include "comp_renderable.h"
#include "comp_position.h"
#include "particle_system.h"

#include "game.h"





extern bool elite_init(const char *configfile);
extern void elite_shutdown(void);