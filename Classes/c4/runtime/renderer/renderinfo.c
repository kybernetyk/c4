/*
 *  renderinfo.c
 *  c4
 *
 *  Created by jrk on 8/2/11.
 *  Copyright 2011 flux forge. All rights reserved.
 *
 */

#include "renderinfo.h"
#include "ogl.h"

void renderinfo_transform(renderinfo_t *ri)
{
	glTranslatef(ri->pos.x, ri->pos.y, ri->zval);
	
	if (ri->rot_a != 0.0f )
		glRotatef( ri->rot_a, 0.0f, 0.0f, 1.0f );
	
	if (ri->scale.x != 1.0 || ri->scale.y != 1.0)
		glScalef( ri->scale.x, ri->scale.y, 1.0f );
	
	glTranslatef(- (ri->anchor_point.x * ri->size.w), - (ri->anchor_point.y * ri->size.h), 0);
	
}

