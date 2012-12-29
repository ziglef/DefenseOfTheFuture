#include "sprite.h"
#include "read_xpm.h"
#include <stdio.h>
#include <string.h>

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

/*
 void delete_sprite(Sprite *sprt, char *base)
{
        char* bgMap = sprt->bgmap;
        if(!bgMap){
        int i, k;
        for(i=0; i < sprt->height; i++)
                for(k=0; k < sprt->width; k++)
                        set_pixel(sprt->x+k,sprt->y+i, 0, base);


        return;}
        sprt->draw=false;
        int i, k;
        for(i=0; i < sprt->height; i++)
                for(k=0; k < sprt->width; k++)
                        set_pixel(sprt->x+k,sprt->y+i, *(bgMap + sprt->width*i + k), base);

}





void destroy_sprite(Sprite *sprt)
{
        if (!sprt) return;
        free(sprt->map);
        free(sprt->bgmap);
        free(sprt);
}
 */

//https://code.google.com/p/sebuxoid/source/browse/trunk/LCOM/SPRITE.C
