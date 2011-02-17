/*
 *  entity_manager.c
 *  c4
 *
 *  Created by jrk on 14/2/11.
 *  Copyright 2011 Jaroslaw Szpilewski. All rights reserved.
 *
 */
#include <memory.h>
#include <stdlib.h>
#include <stdio.h>

#include "entity_manager.h"
#include "entity.h"
#include "sys_config.h"

#pragma mark -
#pragma mark alloc/dealloc
void em_init(le_entity_manager_t *manager)
{
	manager->is_dirty = true;
	manager->current_guid = 1;
	
	manager->entities = calloc(g_sysconfig.entity_pool_size, sizeof(le_entity_t));
	for (int i = 0; i < g_sysconfig.entity_pool_size; i++)
	{	
		manager->entities[i].guid = 0;
		manager->entities[i].manager_id = i;
		manager->entities[i].in_use = false;
		manager->entities[i].entity_manager = manager;
	}
	
	manager->components = calloc(g_sysconfig.entity_pool_size, sizeof(le_component_header_t*));
	for (int i = 0; i < g_sysconfig.entity_pool_size; i++)
	{	
		manager->components[i] = calloc(g_sysconfig.components_per_entity, sizeof(le_component_header_t));
		for (int j = 0; j < g_sysconfig.components_per_entity; j++)
		{
			manager->components[i][j].family = 0;
			manager->components[i][j].subid = 0;
			manager->components[i][j].component_deallocator = NULL;
			manager->components[i][j].component = NULL;
			manager->components[i][j].in_use = false;
		}
	}
}

void em_update(le_entity_manager_t *manager)
{
	manager->is_dirty = false;
}

void em_shutdown(le_entity_manager_t *manager)
{
	em_remove_all_entities(manager);
	
	free(manager->entities);
	for (int i = 0; i < g_sysconfig.entity_pool_size; i++)
		free(manager->components[i]);

	free(manager->components);
}

#pragma mark -
#pragma mark entity management
le_entity_t *em_create_entity(le_entity_manager_t *manager)
{
	manager->is_dirty = true;
	
	for (int i = 0; i < g_sysconfig.entity_pool_size; i++)
	{
		if (!manager->entities[i].in_use)
		{
			manager->entities[i].in_use = true;
			manager->entities[i].manager_id = i;
			manager->entities[i].guid = manager->current_guid++;
			return &manager->entities[i];
		}
	}
	printf("no free entity found! alloc moar space!\n");
	abort();
	return NULL;
}

le_entity_t *em_get_entity_by_guid(le_entity_manager_t *manager, guid_t guid)
{
	for (int i = 0; i < g_sysconfig.entity_pool_size; i++)
		if (manager->entities[i].in_use && manager->entities[i].guid == guid)
			return &manager->entities[i];
	
	return NULL;
}

void em_remove_entity(le_entity_manager_t *manager, le_entity_t *entity)
{
	manager->is_dirty = true;

	entity_remove_all_components(&manager->entities[entity->manager_id]);
	
	manager->entities[entity->manager_id].guid = 0;
	manager->entities[entity->manager_id].in_use = false;
}

void em_remove_all_entities(le_entity_manager_t *manager)
{
	for (int i = 0; i < g_sysconfig.entity_pool_size; i++)
		if (manager->entities[i].in_use)
			em_remove_entity(manager, &manager->entities[i]);
}


#pragma mark -
#pragma mark component querieng
size_t em_get_entities_with_component(le_entity_manager_t *manager, component_family_id_t fam_id, le_entity_t **outarr, size_t max_count)
{
	size_t out_counter = 0;

	for (int i = 0; i < g_sysconfig.entity_pool_size; i++)
	{
		if (manager->components[i][fam_id].in_use)
			outarr[out_counter++] = &manager->entities[i];
		
		if (out_counter >= max_count)
			break;
	}
	
	return out_counter;
}

size_t em_get_entities_with_components(le_entity_manager_t *manager, component_family_id_t query[], size_t query_size, le_entity_t *outarr[], size_t max_count)
{
	size_t out_counter = 0;
	
	bool add_ent = true;	
	for (int i = 0; i < g_sysconfig.entity_pool_size; i++)
	{
		add_ent = true;
		for (int qi = 0; qi < query_size; qi++)
		{
			if (!manager->components[i][query[qi]].in_use)
			{	
				add_ent = false;
				break;
			}
		}
		
		if (add_ent)
		{	
			outarr[out_counter++] = &manager->entities[i];
			if (out_counter >= max_count)
				break;
		}
	}
	
	return out_counter;
}

