/*
 *  menu_scene.c
 *  c4
 *
 *  Created by jrk on 8/2/11.
 *  Copyright 2011 Jaroslaw Szpilewski. All rights reserved.
 *
 */
#include <stdio.h>
#include "menu_scene.h"

#include "renderer.h"
#include "atlas_quad.h"
#include "quad.h"
#include "font.h"

quad_t p;
quad_t q;
atlas_quad_t r;
font_t f;

static int scene_init(scene_t *scene)
{
	quad_load("menu_back.png", &p);
	p.renderinfo.zval = 0;
	p.renderinfo.anchor_point = vec2d_make(0.0, 0.0);
	
	quad_load("minyx_bw.png", &q);
	q.renderinfo.pos.x = SCREEN_W/2;
	q.renderinfo.pos.y = SCREEN_H/2;
	q.renderinfo.zval = 1.0;
	
	atlas_quad_load("bubbles.png", &r);
	r.renderinfo.pos = vec2d_make(100, 100);
	r.renderinfo.zval = 2.0;
	r.renderinfo.size = size2d_make(43, 43);
	r.src_rect = rect_make(0.0, 0.0, 43.0, 43.0);
	
	font_load("impact20.fnt", &f);
	f.renderinfo.pos = vec2d_make(SCREEN_W/2, SCREEN_H/2);
	f.renderinfo.zval = 3.0;
	
	return 0;
}

static void scene_update(scene_t *scene, double dt)
{
	q.renderinfo.rot_a += 360.0 * dt;
	r.renderinfo.rot_a += 360.0 * dt;
}

static void scene_render(scene_t *scene)
{
	quad_render(&p);
	quad_render(&q);
	atlas_quad_render(&r);
	font_render(&f, "oh hai!");
}

static int scene_free(scene_t *scene)
{
	printf("menuscene free\n");
	quad_free(&p);
	quad_free(&q);
	atlas_quad_free(&r);
	font_free(&f);
	
	return 0;
}

scene_t menu_scene_create(void)
{
	scene_t ret;
	ret.init_func = scene_init;
	ret.update_func = scene_update;
	ret.render_func = scene_render;
	ret.free_func = scene_free;
	ret.user_data = NULL;
	
	return ret;
}