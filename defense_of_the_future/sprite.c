#include "sprite.h"
#include "read_xpm.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

Sprite * create_sprite(char *pic[], int x, int y){

	Sprite *spr = (Sprite *) malloc ( sizeof(Sprite));

	if( spr == NULL )
		return NULL;

	sscanf(pic[0],"%d %d", &(spr->width), &(spr->height));

	// read the sprite pixmap
	spr->true_color = read_xpm(pic, &(spr->true_color), &(spr->width), &(spr->height));
	if( spr->true_color == NULL ) {
		free(spr);
		return NULL;
	}
	spr->x = x;
	spr->y = y;

	spr->xspeed = 0;
	spr->yspeed = 0;
	return spr;
}

void destroy_sprite(Sprite *fig)
{
        if (!fig) return;

		//free(fig->map);
		free(fig->true_color);
		free(fig);
}

//https://code.google.com/p/sebuxoid/source/browse/trunk/LCOM/SPRITE.C
