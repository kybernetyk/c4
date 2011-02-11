/*
 *  c4_config.c
 *  c4
 *
 *  Created by jrk on 11/2/11.
 *  Copyright 2011 Jaroslaw Szpilewski. All rights reserved.
 *
 */

#include <stdlib.h>
#include <stdio.h>

#include "c4_config.h"
#include "util.h"

c4_config_t g_sysconfig;


bool c4_config_read_config_file(c4_config_t *cfg, const char *filename)
{
	const char *fn = path_for_file(filename);
	printf("reading config file @ %s ...\n", fn);
	
	cfg->screen_w = 320.0;
	cfg->screen_h = 480.0;
	cfg->desired_fps = 60.0;
	cfg->orientation = PORTRAIT;
	
	return true;
}