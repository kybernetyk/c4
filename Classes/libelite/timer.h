/*
 *  timer.h
 *  c4
 *
 *  Created by jrk on 8/2/11.
 *  Copyright 2011 Jaroslaw Szpilewski. All rights reserved.
 *
 */

typedef struct timer_t
{
	double tick_count;
	double last_tick_count;
	double delta;
	
	double temp;
	double frames;
	double fps;
} timer_t;

extern void timer_update(timer_t *timer);
extern float timer_get_fps(timer_t *timer);

extern unsigned int timer_get_tick_count(void);
extern double timer_get_double_time(void);
extern float timer_get_float_time(void);
