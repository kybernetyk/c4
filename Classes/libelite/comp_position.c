/*
 *  comp_position.c
 *  c4
 *
 *  Created by jrk on 14/2/11.
 *  Copyright 2011 Jaroslaw Szpilewski. All rights reserved.
 *
 */

#include "comp_position.h"
#include <memory.h>
#include <stdlib.h>

static comp_position_t *comp_position_new(void)
{
	return calloc(1, sizeof(comp_position_t));
}

static void comp_position_free(void *data)
{
	free(data);
}

comp_position_t *comp_position_init(le_component_t *comp, vec2d_t pos)
{
	comp_position_t *ret = comp_position_new();
	ret->pos = pos;
	ret->rot = 0.0;
	
	comp->family = COMP_FAMILY_POSITION;
	comp->subid = POS_SUB_ROTPOS;
	comp->user_data = ret;
	comp->user_data_deallocator = comp_position_free;
	
	return ret;
}