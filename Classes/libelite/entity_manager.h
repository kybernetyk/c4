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
	
	le_entity_t *entities;	
	le_component_header_t **components;
} le_entity_manager_t;

extern 		void 			em_init(le_entity_manager_t *manager);
extern 		void 			em_update(le_entity_manager_t *manager);				///call this on each frame enter
extern 		void 			em_shutdown(le_entity_manager_t *manager);

extern 		le_entity_t *	em_create_entity(le_entity_manager_t *manager);
extern 		void 			em_remove_entity(le_entity_manager_t *manager, le_entity_t *entity);
extern 		void 			em_remove_all_entities(le_entity_manager_t *manager);

extern 		le_entity_t *	em_get_entity_by_guid(le_entity_manager_t *manager, guid_t guid);

extern 		size_t 			em_get_entities_with_component(le_entity_manager_t *manager, component_family_id_t fam_id, le_entity_t *outarr[], size_t max_count);
extern 		size_t 			em_get_entities_with_components(le_entity_manager_t *manager, component_family_id_t query[], size_t query_size, le_entity_t **outarr, size_t max_count);









