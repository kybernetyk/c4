/*
 *  entity.h
 *  c4
 *
 *  Created by jrk on 14/2/11.
 *  Copyright 2011 Jaroslaw Szpilewski. All rights reserved.
 *
 */

#pragma once
#include "component.h"

typedef struct le_entity_t
{
	struct le_entity_manager_t *entity_manager;
	
	guid_t guid;
	guid_t manager_id;

	bool in_use;
} le_entity_t;

extern le_component_t *entity_add_component(le_entity_t *entity, component_family_id_t fam_id); 
extern le_component_t *entity_get_component(le_entity_t *entity, component_family_id_t fam_id);
void *entity_get_component_data(le_entity_t *entity, component_family_id_t fam_id);

extern void entity_remove_component(le_entity_t *entity, le_component_t *component);
extern void entity_remove_all_components(le_entity_t *entity);

extern void entity_dump(le_entity_t *entity);

