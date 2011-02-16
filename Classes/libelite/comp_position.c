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
#include <stdio.h>

static cd_position_t *comp_position_new(void)
{
	return calloc(1, sizeof(cd_position_t));
}

static void comp_position_free(void *data)
{
	free(data);
}

cd_position_t *comp_position_init(le_component_t *comp, vec2d_t pos, double z)
{
	cd_position_t *ret = comp_position_new();
	ret->pos = pos;
	ret->z = z;
	ret->rot = 0.0;
	
	comp->family = COMP_FAMILY_POSITION;
	comp->subid = POS_SUB_ROTPOS;
	comp->comp_data = ret;
	comp->comp_data_deallocator = comp_position_free;
#ifdef COMP_INCLUDES_NAME 
	sprintf(comp->name, "%s", "COMP_FAMILY_POSITION.POS_SUB_ROTPOS");
#endif
	
	return ret;
}