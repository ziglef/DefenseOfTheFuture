#include "sprite.h"
#include <stdio.h>
#include <string.h>

Sprite * create_sprite(char *pic[], char *base){

	Sprite *spr = (Sprite *) malloc ( sizeof(Sprite));

	if( spr == NULL )
		return NULL;

	spr->width = itoa(strtok(pic[0], " "));
	spr->height = itoa(strtok(NULL, " "));

	// read the sprite pixmap
	spr->map = read_xpm(pic, &(spr->width), &(spr->height));

	if( spr->map == NULL ) {
		free(spr);
		return NULL;
	}

	spr->x = 100;
	spr->y = 100;

	spr->xspeed = 0;
	spr->yspeed = 0;

	return spr;
}
