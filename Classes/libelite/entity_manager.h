/*
 *  entity_manager.h
 *  c4
 *
 *  Created by jrk on 14/2/11.
 *  Copyright 2011 Jaroslaw Szpilewski. All rights reserved.
 *
 */

#pragma once
#include <stddef.h>

#include "le_types.h"
#include "entity.h"
#include "component.h"

typedef struct le_entity_manager_t
{
	guid_t current_guid;
	bool is_dirty;
	
	struct le_entity_t *entities;	
	struct le_component_t **components;
} le_entity_manager_t;

extern void em_init(le_entity_manager_t *manager);
extern void em_prepare_frame(le_entity_manager_t *manager);				///call this on each frame enter
extern void em_free(le_entity_manager_t *manager);

extern le_entity_t *em_create_entity(le_entity_manager_t *manager);
extern le_entity_t *em_get_entity_by_guid(le_entity_manager_t *manager, guid_t guid);
extern void em_free_entity(le_entity_manager_t *manager, le_entity_t *entity);

extern le_component_t *em_add_component_to_entity(le_entity_manager_t *manager, le_entity_t *entity, component_family_id_t fam_id); 
extern le_component_t *em_get_component_from_entity(le_entity_manager_t *manager, le_entity_t *entity, component_family_id_t fam_id);
extern void em_remove_component_from_entity(le_entity_manager_t *manager, le_entity_t *entity, le_component_t *component);
extern void em_remove_all_components_from_entity(le_entity_manager_t *manager, le_entity_t *entity);

extern size_t em_get_entities_with_component(le_entity_manager_t *manager, component_family_id_t fam_id, le_entity_t *outarr[], size_t max_count);
extern size_t em_get_entities_with_components(le_entity_manager_t *manager, component_family_id_t query[], size_t query_size, le_entity_t **outarr, size_t max_count);

extern void em_dump_entity(le_entity_manager_t *manager, le_entity_t *entity);







