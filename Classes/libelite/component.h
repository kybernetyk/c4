/*
 *  component.h
 *  c4
 *
 *  Created by jrk on 14/2/11.
 *  Copyright 2011 Jaroslaw Szpilewski. All rights reserved.
 *
 */

#pragma once
#include "le_types.h"



#define COMP_FAMILY_RENDERABLE 0x01
#define COMP_FAMILY_POSITION 0x02
#define COMP_FAMILY_GARBAGE 0x03

typedef struct le_component_t
{
	component_family_id_t family;
	component_sub_id_t subid;

	void *comp_data;
	void (*comp_data_deallocator)(void *);
	bool in_use;

#ifdef COMP_INCLUDES_NAME 	
	char name[128];
#endif
	
} le_component_t;

#define comp_get_data(obj, type) ((type*)obj->comp_data)
#define comp_data(obj) (obj->comp_data)