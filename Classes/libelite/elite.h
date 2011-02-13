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


#include "game.h"



extern bool elite_init(const char *configfile);
extern void elite_shutdown(void);