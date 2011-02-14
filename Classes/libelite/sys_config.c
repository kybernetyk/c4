/*
 *  c4_config.c
 *  c4
 *
 *  Created by jrk on 11/2/11.
 *  Copyright 2011 Jaroslaw Szpilewski. All rights reserved.
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "util.h"
#include "hash.h"
#include "sys_config.h"

sys_config_t g_sysconfig;

static void dump_config(sys_config_t *cfg)
{
	printf("dumping config ...\n{\n");
	printf("\tscreen:\n\t{\n");
	printf("\t\tscreen_w = %.2f\n", cfg->screen_w);
	printf("\t\tscreen_h = %.2f\n", cfg->screen_h);
	printf("\t\tscreen_scale = %.2f\n", cfg->screen_scale);
	printf("\t\ttexture_cache_size = %i\n", cfg->texture_cache_size);
	printf("\t\tdesired_fps = %.2f\n", cfg->desired_fps);
	printf("\t\torientation = %i", cfg->orientation);
	if (cfg->orientation == LE_PORTRAIT)
		printf(" (= PORTRAIT)\n");
	if (cfg->orientation == LE_LANDSCAPE)
		printf(" (= LANDSCAPE)\n");
	printf("\t}\n");
	//sfx
	printf("\taudio:\n\t{\n");
	printf("\t\tsound_vol = %.2f\n", cfg->sound_vol);
	printf("\t\tmusic_vol = %.2f\n", cfg->music_vol);
	printf("\t\taudio_cache_size = %i\n", cfg->audio_cache_size);
	printf("\t}\n");
	//ent system
	printf("\tentity system:\n\t{\n");
	printf("\t\tentity_pool_size = %i\n", cfg->entity_pool_size);
	printf("\t\tcomponents_per_entity = %i\n", cfg->components_per_entity);
	printf("\t}\n");
	printf("}\n");
	
}

static bool line_is_comment(const char *line)
{
	if (!line)
		return true;
	
	if (strlen(line) == 0)
		return true;
	
	if (line[0] == '#' || line[0] == '\n' || line[0] == 0)
		return true;
	
	return false;
}

//lowercase a string (will alter the string passed!)
static char *str_lower(char *line)
{
	char *p = line;
	
	while (1) 
	{
		if (*p == 0)
			break;
		if (*p >= 'A' && *p <= 'Z')
			*p = *p + 'a' - 'A';
		p++;
	}
	
	return line;
}

//trim whitespace and newline from left and right (will alter the string passed!)
static char *str_trim(char *line)
{
	//trim from left
	char *pstart = line;
	while(1)
	{
		if ((*pstart != ' ' && *pstart != '\n' && *pstart != '\t') || pstart == 0)
			break;
		pstart++;
	}
	
	//trim from right
	char *pend = pstart + strlen(pstart)-1;
	while (1) 
	{
		if ((*pend != ' ' && *pend != '\n' && *pend != '\t') || pend == 0)
		{	
			break;
		}
		*pend = 0;
		pend--;
	}
	return pstart;
}

static unsigned int line_get_key(const char *line)
{
	char copy[strlen(line)+1];
	sprintf(copy, "%s", line);
	char *pend = str_lower(copy);
	
	while(1)
	{
		if (*pend == '=' || *pend == '\n' || *pend == 0)
		{	
			*pend = 0;
			break;
		}
		pend++;
	}

	char *tmp = str_trim(copy);
	unsigned int hash = murmur_hash_2(tmp, strlen(tmp), 0);
	

	return hash;
}

//sets 0 bytes to extract the value. (will alter the line!)
static char *line_get_value(char *line)
{
	char *pstart = line;
	while(1)
	{
		if (*pstart == '=')
		{
			pstart++;
			break;
		}
		if (*pstart == 0 || *pstart =='\n')
			return NULL;
		pstart ++;
	}
	
	char *pend = pstart;
	while(1)
	{
		if (*pend == '\n' || *pend == 0 || *pend == '#')
		{
			*pend = 0;
			return str_lower(pstart);
		}
		pend++;
	}
	
	return NULL;
}

static void set_key_value(sys_config_t *cfg, unsigned int key_hash, const char *value)
{
	char copy[strlen(value)+1];
	sprintf(copy, "%s", value);
	char *val = str_trim(copy);
	
	unsigned int h_val = murmur_hash_2(val, strlen(val), 0);
	
	unsigned int h_screen_w = murmur_hash_2("screen.w", strlen("screen.w"), 0);
	unsigned int h_screen_h = murmur_hash_2("screen.h", strlen("screen.h"), 0);
	unsigned int h_screen_scale = murmur_hash_2("screen.scale", strlen("screen.scale"), 0);
	unsigned int h_tex_cache_size = murmur_hash_2("screen.texture_cache_size", strlen("screen.texture_cache_size"), 0);
	unsigned int h_des_fps = murmur_hash_2("screen.desired_fps", strlen("screen.desired_fps"), 0);
	unsigned int h_orientation = murmur_hash_2("screen.orientation", strlen("screen.orientation"), 0);

	if (key_hash == h_screen_w)
		cfg->screen_w = atof(val);

	if (key_hash == h_screen_h)
		cfg->screen_h = atof(val);
	
	if (key_hash == h_screen_scale)
		cfg->screen_scale = atof(val);
	
	if (key_hash == h_des_fps)
		cfg->desired_fps = atof(val);

	if (key_hash == h_tex_cache_size)
		cfg->texture_cache_size = atoi(val);
	
	if (key_hash == h_orientation)
	{	
		unsigned int h_portrait = murmur_hash_2("portrait", strlen("portrait"), 0);
		unsigned int h_landscape = murmur_hash_2("landscape", strlen("landscape"), 0);

		if (h_val == h_portrait)
			cfg->orientation = LE_PORTRAIT;
		if (h_val == h_landscape)
			cfg->orientation = LE_LANDSCAPE;
	}

	
	unsigned int h_audio_cache_size = murmur_hash_2("audio.audio_cache_size", strlen("audio.audio_cache_size"), 0);
	unsigned int h_sound_volume = murmur_hash_2("audio.sound_vol", strlen("audio.sound_vol"), 0);
	unsigned int h_music_volume = murmur_hash_2("audio.music_vol", strlen("audio.music_vol"), 0);
	
	if (key_hash == h_audio_cache_size)
		cfg->audio_cache_size = atoi(val);
	if (key_hash == h_sound_volume)
		cfg->sound_vol = atof(val);
	if (key_hash == h_music_volume)
		cfg->music_vol = atof(val);
	
	
	
	unsigned int h_em_entity_pool = murmur_hash_2("entity_manager.entity_pool_size", strlen("entity_manager.entity_pool_size"), 0);
	unsigned int h_em_components_per_ent = murmur_hash_2("entity_manager.components_per_entity", strlen("entity_manager.components_per_entity"), 0);

	if (key_hash == h_em_entity_pool)
		cfg->entity_pool_size = atoi(val);
	if (key_hash == h_em_components_per_ent)
		cfg->components_per_entity = atoi(val);
	
	
	return;
}

static void set_defaults(sys_config_t *cfg)
{
	//screen
	cfg->screen_w = 320.0;
	cfg->screen_h = 480.0;
	cfg->screen_scale = 1.0;
	cfg->desired_fps = 60.0;
	cfg->orientation = LE_PORTRAIT;
	cfg->texture_cache_size = 32;
	
	//audio
	cfg->audio_cache_size = 32;
	cfg->sound_vol = 0.8;
	cfg->music_vol = 0.5;
	
	//entity
	cfg->entity_pool_size = 512;
	cfg->components_per_entity = 32;

}

bool sys_config_read_config_file(sys_config_t *cfg, const char *filename)
{
	set_defaults(cfg);
	
	const char *fn = path_for_file(filename);
	if (!fn)
		return false;
	printf("reading config file @ %s ...\n", fn);

	FILE *f_in = fopen(fn, "r");
	if (!f_in)
		return false;
	
	unsigned int key_hash = 0;
	char *value = NULL;
	char line[1024];
	while(1)
	{
		if(!fgets(line, 1024, f_in))
			break;
		
		char *ltrimmed = str_lower(str_trim(line));
		
		if(line_is_comment(ltrimmed))
			continue;
		
		key_hash = line_get_key(ltrimmed);
		value = line_get_value(ltrimmed);
		
		set_key_value(cfg, key_hash, value);
	}
	fclose(f_in);

	dump_config(cfg);
	
	return true;
}