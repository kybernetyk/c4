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

#include "input.h"
#include "texture.h"
#include "renderinfo.h"
#include "renderer.h"
#include "audio.h"

#include "timer.h"
#include "font.h"
#include "scene.h"
#include "util.h"
#include "hash.h"

extern bool elite_init(void);
extern void elite_shutdown(void);