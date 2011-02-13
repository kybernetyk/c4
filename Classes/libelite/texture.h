/*
 *  texture.h
 *  c4
 *
 * A simple texture manager 
 *
 */
#pragma once
#include "ogl.h"

typedef int tex2d_id;

typedef struct texture2d_t
{
	GLuint w;
	GLuint h;
	GLuint name;
} tex2d_t;

typedef struct texture_params_t 
{
	GLuint	minFilter;
	GLuint	magFilter;
	GLuint	wrapS;
	GLuint	wrapT;
} texparams_t;

extern GLuint texture_current_bound_texture;

extern bool tex_manager_init(void);
extern void tex_manager_shutdown(void);

extern tex2d_id tex2d_load(const char *filename);
extern tex2d_t *tex2d_get_tex_by_id(tex2d_id tex_id);
extern void tex2d_bind(GLuint texname);
extern void tex2d_release(tex2d_id tex_id);

extern void tex2d_set_texparams(tex2d_t *tex, texparams_t *params);
extern void tex2d_set_alias_texparams(tex2d_t *tex);
extern void tex2d_set_antialias_texparams(tex2d_t *tex);