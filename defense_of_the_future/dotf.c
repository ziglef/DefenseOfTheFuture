#include "dotf.h"

void start_game();

int main(){

	sef_startup();

	start_game();
}

void start_game(){

	// Initializes the video memory in VIDEO_MODE (0x117)
	char *video_mem = vg_init(VIDEO_MODE);

	Sprite *spr = create_sprite(ship);

	vg_draw_sprite(spr);

	sleep(5);

	vg_exit();
}
