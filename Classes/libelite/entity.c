/*
 *  entity.c
 *  c4
 *
 *  Created by jrk on 14/2/11.
 *  Copyright 2011 Jaroslaw Szpilewski. All rights reserved.
 *
 */

#include "entity.h"
#include "elite.h"
#include <stdio.h>
#include <stdlib.h>

#pragma mark -
#pragma mark component management
le_component_t *entity_add_component(le_entity_t *entity, component_family_id_t fam_id)
{
	if (!entity->in_use)
	{
		printf("trying to add a component to a not existing entity!\n");
		abort();
		return NULL;
	}
	
	le_entity_manager_t *manager = entity->entity_manager;
	
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
		entity_remove_component(entity, ret);
	}
	
	ret->in_use = true;
	ret->family = fam_id;
	ret->user_data = NULL;
	ret->subid = 0;
	ret->user_data_deallocator = NULL;
	return ret;
}



void entity_remove_component(le_entity_t *entity, le_component_t *component)
{
	if (!entity->in_use)
	{
		printf("trying to remove a component from a not existing entity!\n");
		abort();
		return;
	}
	
	le_entity_manager_t *manager = entity->entity_manager;
	
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

void entity_remove_all_components(le_entity_t *entity)
{
	le_entity_manager_t *manager = entity->entity_manager;
	
	manager->is_dirty = true;
	
	for (int i = 0; i < g_sysconfig.components_per_entity; i++)
		if (manager->components[entity->manager_id][i].in_use)
			entity_remove_component(entity, &manager->components[entity->manager_id][i]);
}


le_component_t *entity_get_component(le_entity_t *entity, component_family_id_t fam_id)
{
	le_entity_manager_t *manager = entity->entity_manager;
	
	le_component_t *ret = &manager->components[entity->manager_id][fam_id];
	if (!ret->in_use)
		return NULL;
	
	return ret;
}

void *entity_get_component_data(le_entity_t *entity, component_family_id_t fam_id)
{
	return entity_get_component(entity, fam_id)->user_data;
}


#pragma mark -
#pragma mark dumping

void component_dump(le_component_t *component)
{
	printf("component [fam: %i, sub: %i] @ %p (%s)\n", component->family, component->subid, component, component->name);	
}

void entity_dump_components(le_entity_t *entity)
{
	le_entity_manager_t *manager = entity->entity_manager;
	for (int i = 0; i < g_sysconfig.components_per_entity; i++)
	{	
		if (manager->components[entity->manager_id][i].in_use)
		{	
			printf("\t");
			component_dump(&manager->components[entity->manager_id][i]);
		}
	}
}

void entity_dump(le_entity_t *entity)
{
	printf("entity [mid: %i, guid: %i] @ %p:\n", entity->manager_id, entity->guid, entity);
	entity_dump_components(entity);
}
