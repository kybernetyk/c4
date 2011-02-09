/*
 *  scene.h
 *  c4
 *
 *  Created by jrk on 8/2/11.
 *  Copyright 2011 flux forge. All rights reserved.
 *
 */
#pragma once

/* abstract scene interface 
 	
 	populate the struct with pointers to the respective methods of your scene

 
 */

typedef struct scene_t
{
	int (*init_func)(struct scene_t *);
	void (*update_func)(struct scene_t *, double);
	void (*render_func)(struct scene_t *);
	int (*free_func)(struct scene_t *);
	
	void *user_data;
} scene_t;

