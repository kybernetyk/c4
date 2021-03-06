/*
 *  elite.c
 *  c4
 *
 *  Created by jrk on 13/2/11.
 *  Copyright 2011 Jaroslaw Szpilewski. All rights reserved.
 *
 */

#include "elite.h"
#include <stdio.h>
#include <stdlib.h>

bool elite_init(const char *configfile)
{
	if (!sys_config_read_config_file(&g_sysconfig, configfile))
	{
		printf("couldn't parse config file: %s!\n", configfile);
		abort();
	}
	
	if (!fs_input_init())
		return false;
	if (!fs_audio_init())
		return false;
	if (!fs_renderer_init())
		return false;
	
	return true;
}

void elite_shutdown(void)
{
	fs_audio_shutdown();
	fs_renderer_shutdown();
}