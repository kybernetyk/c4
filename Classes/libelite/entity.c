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
le_component_header_t *entity_add_component(le_entity_t *entity, component_family_id_t fam_id)
{
	if (!entity->in_use)
	{
		printf("trying to add a component to a not existing entity!\n");
		abort();
		return NULL;
	}
	
	le_entity_manager_t *manager = entity->entity_manager;
	
	manager->is_dirty = true;
	
	le_component_header_t *ret = &manager->components[entity->manager_id][fam_id];
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
	ret->component = NULL;
	ret->subid = 0;
	ret->component_deallocator = NULL;
	return ret;
}



void entity_remove_component(le_entity_t *entity, le_component_header_t *comp_hdr)
{
	if (!entity->in_use)
	{
		printf("trying to remove a component from a not existing entity!\n");
		abort();
		return;
	}
	
	le_entity_manager_t *manager = entity->entity_manager;
	
	if (!comp_hdr->in_use)
		return;
	
	manager->is_dirty = true;
	
	if (comp_hdr->component_deallocator)
		comp_hdr->component_deallocator(comp_hdr->component);
	
	comp_hdr->component_deallocator = NULL;
	comp_hdr->component = NULL;
	comp_hdr->family = 0;
	comp_hdr->subid = 0;
	comp_hdr->in_use = false;
}

void entity_remove_all_components(le_entity_t *entity)
{
	le_entity_manager_t *manager = entity->entity_manager;
	
	manager->is_dirty = true;
	
	for (int i = 0; i < g_sysconfig.components_per_entity; i++)
		if (manager->components[entity->manager_id][i].in_use)
			entity_remove_component(entity, &manager->components[entity->manager_id][i]);
}


le_component_header_t *entity_get_component_header(le_entity_t *entity, component_family_id_t fam_id)
{
	le_entity_manager_t *manager = entity->entity_manager;
	
	le_component_header_t *ret = &manager->components[entity->manager_id][fam_id];
	if (!ret->in_use)
		return NULL;
	
	return ret;
}

void *entity_get_component(le_entity_t *entity, component_family_id_t fam_id)
{
	return entity_get_component_header(entity, fam_id)->component;
}


#pragma mark -
#pragma mark dumping

void component_dump(le_component_header_t *component)
{
#ifdef COMP_INCLUDES_NAME 
	printf("component [fam: %i, sub: %i] @ %p (%s)\n", component->family, component->subid, component, component->name);	
#else
	printf("component [fam: %i, sub: %i] @ %p\n", component->family, component->subid, component);	
#endif
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
