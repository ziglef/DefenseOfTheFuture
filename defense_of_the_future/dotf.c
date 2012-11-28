#include "dotf.h"

void start_game();

int main(){

	sef_startup();
	start_game();

	//vg_exit();
}

void start_game(){

	int i;

	// Initializes the video memory in VIDEO_MODE (0x117)
	char *video_mem = vg_init(VIDEO_MODE);

	Sprite *player = create_sprite(ship, SHIP_START_X, SHIP_START_Y);
	vg_draw_sprite(player);

	Sprite **enemies = (Sprite **)malloc(NO_ENEMIES * sizeof(Sprite));
	for(i=0; i<NO_ENEMIES; i++){
		if(i<8)
			enemies[i] = create_sprite(bad, ENEMIES_START_X+ENEMIES_X_INCREMENT*i, ENEMIES_START_Y);
		else
			enemies[i] = create_sprite(bad, ENEMIES_START_X2+ENEMIES_X_INCREMENT*(i-8), ENEMIES_START_Y+ENEMIES_Y_INCREMENT);
	}

	for(i=0; i<NO_ENEMIES; i++)
		vg_draw_sprite(enemies[i]);

	sleep(5);

	vg_exit();
}
