/*
 *  texture.c
 *  c4
 *
 *  Created by jrk on 8/2/11.
 *  Copyright 2011 flux forge. All rights reserved.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "texture.h"
#include "util.h"
#include "SOIL.h"
#include "hash.h"

#define MAX_TEXTURES 64

GLuint texture_current_bound_texture = 0;

typedef struct tex_cache_element
{
	unsigned int hash;				//32bit hash value for the filename
	int retain_count;
	tex2d_t texture;
} tex_cache_element;

static tex_cache_element tex_cache[MAX_TEXTURES];

tex2d_id tex2d_load(const char *filename)
{
	const char *fn = path_for_file(filename);
	unsigned int fnhash = murmur_hash_2(filename, strlen(filename), 0); 
	
	for (int i = 0; i < MAX_TEXTURES; i++)
	{
		if (tex_cache[i].hash == fnhash && tex_cache[i].retain_count > 0)
		{
			tex_cache[i].retain_count++;
			
			printf("found file[%s] in cache[0x%x]! returning texture %i with retaincount %i!\n", filename, tex_cache[i].hash, tex_cache[i].texture.name, tex_cache[i].retain_count);
			
			return i;
		}
	}
	
	
	unsigned int w = 0;
	unsigned int h = 0;
	
	GLuint texname = SOIL_load_OGL_texture2(
	 fn,
	 SOIL_LOAD_AUTO,
	 SOIL_CREATE_NEW_ID,
	 0,//SOIL_FLAG_COMPRESS_TO_DXT,
	 // SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
	 //SOIL_FLAG_MIPMAPS | SOIL_FLAG_COMPRESS_TO_DXT
	 //	 SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
	 &w,&h
	 );
	
	if (texname == 0)
	{
		printf("SOIL loading error: '%s' - %s\n", SOIL_last_result(),fn);
		abort();
		return -1;
	}
	
	//find free texture slot
	tex2d_id ti = -1;
	for (int i = 0; i < MAX_TEXTURES; i++)
	{
		if (tex_cache[i].retain_count == 0)
		{	
			ti = i;
			break;
		}
	}	
	if (ti < 0)
	{
		printf("Texture Pool full! Increse MAX_TEXTURES!\n");
		abort();
		return -1;
	}
	
	tex_cache[ti].retain_count = 1;
	tex_cache[ti].hash = fnhash;
	tex_cache[ti].texture = (tex2d_t){
		.name = texname,
		.w = w,
		.h = h
	};
	
	//for 2d we most probably don't want anti aliased textures as they look blurry
	tex2d_set_alias_texparams(&tex_cache[ti].texture);
	
	//printf("new file loaded to cache[%i]! 0x%x -> %i\n",ti,tex_cache[ti].hash, tex_cache[ti].texture.name );
	return ti;
}

tex2d_t *tex2d_get_tex_by_id(tex2d_id tex_id)
{
	if (tex_id < 0 || tex_id >= MAX_TEXTURES)
	{
		printf("%i is an invalid tex_id!\n", tex_id);
		abort();
		return NULL;
	}
	
	return &tex_cache[tex_id].texture;
}

void tex2d_bind(GLuint texname)
{
	if (texname == texture_current_bound_texture)
		return;
	texture_current_bound_texture = texname;
	
	glBindTexture(GL_TEXTURE_2D, texname);
}

void tex2d_release(tex2d_id tex_id)
{
	tex_cache[tex_id].retain_count --;
	if (tex_cache[tex_id].retain_count <= 0)
	{
		if (texture_current_bound_texture == tex_cache[tex_id].texture.name)
			texture_current_bound_texture = 0;
		
		glDeleteTextures(1, &tex_cache[tex_id].texture.name);
		
		tex_cache[tex_id].texture = (tex2d_t){
			.name = 0,
			.w = 0,
			.h = 0
		};
		
		tex_cache[tex_id].hash = 0;
		tex_cache[tex_id].retain_count = 0;
	}
}

extern void tex2d_set_texparams(tex2d_t *tex, texparams_t *params)
{
	tex2d_bind(tex->name);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, params->minFilter );
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, params->magFilter );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, params->wrapS );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, params->wrapT );
}

extern void tex2d_set_alias_texparams(tex2d_t *tex)
{
	texparams_t t = { GL_NEAREST, GL_NEAREST, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE };
	tex2d_set_texparams(tex, &t);
}

extern void tex2d_set_antialias_texparams(tex2d_t *tex)
{
	texparams_t t = { GL_LINEAR, GL_LINEAR, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE };
	tex2d_set_texparams(tex, &t);
}