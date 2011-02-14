/*
 *  entity.h
 *  c4
 *
 *  Created by jrk on 14/2/11.
 *  Copyright 2011 Jaroslaw Szpilewski. All rights reserved.
 *
 */

#pragma once
#include "le_types.h"

typedef struct le_entity_t
{
	struct le_entity_manager_t *entity_manager;
	
	guid_t guid;
	guid_t manager_id;

	bool in_use;
} le_entity_t;