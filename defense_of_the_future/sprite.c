#include "sprite.h"
#include "read_xpm.h"
#include <stdio.h>
#include <string.h>

Sprite * create_sprite(char *pic[], char *base){

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
	spr->x = 100;
	spr->y = 100;

	spr->xspeed = 0;
	spr->yspeed = 0;
	return spr;
}
