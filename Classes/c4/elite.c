/*
 *  elite.c
 *  c4
 *
 *  Created by jrk on 13/2/11.
 *  Copyright 2011 Jaroslaw Szpilewski. All rights reserved.
 *
 */

#include "elite.h"

bool elite_init(void)
{
	if (!input_init())
		return false;
	if (!audio_init())
		return false;
	if (!renderer_init(1.0))
		return false;

	return true;
}

void elite_shutdown(void)
{
	audio_shutdown();
	renderer_shutdown();
}