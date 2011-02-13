/*
 *  timer.c
 *  c4
 *
 *  Created by jrk on 8/2/11.
 *  Copyright 2011 Jaroslaw Szpilewski. All rights reserved.
 *
 */

#include "timer.h"
#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include <mach/mach.h>
#include <mach/mach_time.h>

void timer_update(timer_t *timer)
{
	timer->last_tick_count = timer->tick_count;
	timer->tick_count = timer_get_double_time();
	timer->delta = (timer->tick_count - timer->last_tick_count);
}

float timer_get_fps(timer_t *timer)
{
	timer->frames ++;
	timer->temp += timer->delta;
	//printf ("%f\n",temp);
	if (timer->temp >= 0.1f)
	{
		//if (printout)
		//printf("fps: %f\n", timer->frames / timer->temp);
		
		timer->fps = timer->frames / timer->temp;
		timer->temp = 0.0;
		timer->frames = 0.0;
		
	}
	//return 1.0 / m_ulDelta;
	return timer->fps;
}

unsigned int timer_get_tick_count(void)
{
	struct timeval v;
	gettimeofday(&v, 0);
	return (v.tv_sec * 1000) + (v.tv_usec / 1000);
}

double timer_get_double_time(void)
{
	mach_timebase_info_data_t base;
	mach_timebase_info(&base);
	
	uint64_t nanos = (mach_absolute_time()*base.numer)/base.denom;
	return (double)nanos*1.0e-9;
}

float timer_get_float_time(void)
{
	mach_timebase_info_data_t base;
	mach_timebase_info(&base);
	
	uint64_t nanos = (mach_absolute_time()*base.numer)/base.denom;
	return (float)nanos*1.0e-9;
}
