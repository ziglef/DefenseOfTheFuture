#include "dotf.h"

void start_game();

int main(){

	sef_startup();

	start_game();
}

void start_game(){

	char *video_mem = vg_init(0x117);

	Sprite *spr = create_sprite(ship, 0 );

	vg_draw_sprite(spr);

	sleep(5);

	vg_exit();
}
