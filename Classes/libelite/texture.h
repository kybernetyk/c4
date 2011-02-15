/*
 *  texture.h
 *  c4
 *
 * A simple texture manager 
 *
 */
#pragma once
#include "ogl.h"

typedef int fs_tex2d_id;

typedef struct fs_texture2d_t
{
	GLuint w;
	GLuint h;
	GLuint name;
} fs_tex2d_t;

typedef struct fs_texture_params_t 
{
	GLuint	minFilter;
	GLuint	magFilter;
	GLuint	wrapS;
	GLuint	wrapT;
} fs_texparams_t;

extern GLuint texture_current_bound_texture;

extern bool fs_tex_manager_init(void);
extern void fs_tex_manager_shutdown(void);

extern fs_tex2d_id fs_tex2d_load(const char *filename);
extern fs_tex2d_t *fs_tex2d_get_tex_by_id(fs_tex2d_id tex_id);
extern void fs_tex2d_bind(GLuint texname);
extern void fs_tex2d_release(fs_tex2d_id tex_id);

extern void fs_tex2d_set_texparams(fs_tex2d_t *tex, fs_texparams_t *params);
extern void fs_tex2d_set_alias_texparams(fs_tex2d_t *tex);
extern void fs_tex2d_set_antialias_texparams(fs_tex2d_t *tex);