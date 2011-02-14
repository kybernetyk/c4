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

#define ENTITIES 512
#define COMPONENTS 32

#pragma mark -
#pragma mark alloc/dealloc
void em_init(le_entity_manager_t *manager)
{
	manager->is_dirty = true;
	manager->current_guid = 1;
	
	manager->entities = calloc(ENTITIES, sizeof(le_entity_t));
	for (int i = 0; i < ENTITIES; i++)
	{	
		manager->entities[i].guid = 0;
		manager->entities[i].manager_id = i;
		manager->entities[i].in_use = false;
		manager->entities[i].entity_manager = manager;
	}
	
	manager->components = calloc(ENTITIES, sizeof(le_component_t*));
	for (int i = 0; i < ENTITIES; i++)
	{	
		manager->components[i] = calloc(COMPONENTS, sizeof(le_component_t));
		for (int j = 0; j < COMPONENTS; j++)
		{
			manager->components[i][j].family = 0;
			manager->components[i][j].subid = 0;
			manager->components[i][j].user_data_deallocator = NULL;
			manager->components[i][j].user_data = NULL;
			manager->components[i][j].in_use = false;
		}
	}
}

void em_prepare_frame(le_entity_manager_t *manager)
{
	manager->is_dirty = false;
}

void em_free(le_entity_manager_t *manager)
{
	free(manager->entities);
	for (int i = 0; i < ENTITIES; i++)
		free(manager->components[i]);

	free(manager->components);
}

#pragma mark -
#pragma mark entity management
le_entity_t *em_create_entity(le_entity_manager_t *manager)
{
	manager->is_dirty = true;
	
	for (int i = 0; i < ENTITIES; i++)
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
	for (int i = 0; i < ENTITIES; i++)
		if (manager->entities[i].in_use && manager->entities[i].guid == guid)
			return &manager->entities[i];
	
	return NULL;
}

void em_free_entity(le_entity_manager_t *manager, le_entity_t *entity)
{
	manager->is_dirty = true;

	em_remove_all_components_from_entity(manager, &manager->entities[entity->manager_id]);
	
	manager->entities[entity->manager_id].guid = 0;
	manager->entities[entity->manager_id].in_use = false;
}

#pragma mark -
#pragma mark component management
le_component_t *em_add_component_to_entity(le_entity_manager_t *manager, le_entity_t *entity, component_family_id_t fam_id)
{
	manager->is_dirty = true;

	le_component_t *ret = &manager->components[entity->manager_id][fam_id];
	if (ret->in_use)
	{
		printf("[entity %i]: replacing component %i.%i with %i.%i\n",
			   entity->guid,
			   ret->family,
			   ret->subid,
			   ret->family,
			   0);
		em_remove_component_from_entity(manager, entity, ret);
	}
	
	ret->in_use = true;
	ret->family = fam_id;
	ret->user_data = NULL;
	ret->subid = 0;
	ret->user_data_deallocator = NULL;
	return ret;
}

void em_remove_component_from_entity(le_entity_manager_t *manager, le_entity_t *entity, le_component_t *component)
{
	if (!component->in_use)
		return;
	
	manager->is_dirty = true;

	if (component->user_data_deallocator)
		component->user_data_deallocator(component->user_data);

	component->user_data_deallocator = NULL;
	component->user_data = NULL;
	component->family = 0;
	component->subid = 0;
	component->in_use = false;
}

void em_remove_all_components_from_entity(le_entity_manager_t *manager, le_entity_t *entity)
{
	manager->is_dirty = true;
	
	for (int i = 0; i < COMPONENTS; i++)
		if (manager->components[entity->manager_id][i].in_use)
			em_remove_component_from_entity(manager, entity, &manager->components[entity->manager_id][i]);
}

#pragma mark -
#pragma mark component querieng
le_component_t *em_get_component_from_entity(le_entity_manager_t *manager, le_entity_t *entity, component_family_id_t fam_id)
{
	le_component_t *ret = &manager->components[entity->manager_id][fam_id];
	if (!ret->in_use)
		return NULL;

	return ret;
}

size_t em_get_entities_with_component(le_entity_manager_t *manager, component_family_id_t fam_id, le_entity_t **outarr, size_t max_count)
{
	size_t out_counter = 0;

	for (int i = 0; i < ENTITIES; i++)
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
	for (int i = 0; i < ENTITIES; i++)
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

#pragma mark -
#pragma mark dumping

void em_dump_component(le_entity_manager_t *manager, le_component_t *component)
{
	printf("component [fam: %i, sub: %i] @ %p (%s)\n", component->family, component->subid, component, component->name);	
}

void em_dump_components(le_entity_manager_t *manager, le_entity_t *entity)
{
	for (int i = 0; i < COMPONENTS; i++)
	{	
		if (manager->components[entity->manager_id][i].in_use)
		{	
			printf("\t");
			em_dump_component(manager, &manager->components[entity->manager_id][i]);
			
		}
	}
}

void em_dump_entity(le_entity_manager_t *manager, le_entity_t *entity)
{
	printf("entity [mid: %i, guid: %i] @ %p:\n", entity->manager_id, entity->guid, entity);
	em_dump_components(manager, entity);
}