#include "dotf.h"

int start_game();
int subscribe();
int unsubscribe();

KeyBoardController KBCs = {0, 0, 0};
unsigned char kscancode = 0;

int main(){

	sef_startup();

	start_game();

	//vg_exit();
	return 0;
}

int start_game(){

	int ipc_status;
	message msg;
	int r;
	int i;

	subscribe();

	// Initializes the video memory in VIDEO_MODE (0x117)
	char *video_mem = vg_init(VIDEO_MODE);
	vg_fill(0x0000);

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

	while(kscancode != ESC_BREAKCODE){

			r = driver_receive(ANY, &msg, &ipc_status);
			if( r != 0 ){
				printf("driver_receive failed with %d\n", r);
				continue;
			}

			if(is_ipc_notify(ipc_status)){

				switch(_ENDPOINT_P(msg.m_source)){
					case HARDWARE:
						if((msg.NOTIFY_ARG & KBC_IRQ)){
							kscancode = kbc_handler();
						} break;
					default: break;
				}
			}

			switch(kscancode){
				case WMAKE:
					vg_draw_rec(player->x, player->y, player->x+player->width, player->y+player->height, 0xFF00);
					player->y -= 20;
					vg_draw_sprite(player);
					break;
				case SMAKE:
					vg_draw_rec(player->x, player->y, player->x+player->width, player->y+player->height, 0xFF00);
					player->y += 20;
					vg_draw_sprite(player);
					break;
				case AMAKE:
					vg_draw_rec(player->x, player->y, player->x+player->width, player->y+player->height, 0xFF00);
					player->x -= 20;
					vg_draw_sprite(player);
					break;
				case DMAKE:
					vg_draw_rec(player->x, player->y, player->x+player->width, player->y+player->height, 0xFF00);
					player->x += 20;
					vg_draw_sprite(player);
					break;
			}
	}

	unsubscribe();
	vg_exit();
}

int subscribe(){
	if(kbc_subscribe_exclusive() < 0){
			printf("ERROR SUBSCRIBING TO KBC!\n");
			return 1;
	}
	return 0;
}

int unsubscribe(){
	if(kbc_unsubscribe())
			return 1;
		else
			return 0;
}
