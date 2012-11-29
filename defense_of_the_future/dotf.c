#include "dotf.h"

int start_game();
int subscribe();
int unsubscribe();
void mainloop();
void make_player_movement();
void make_bad_movement();
void exit_game();

Sprite *player;
Sprite **enemies;
unsigned long time = 0;
unsigned char kscancode = 0;
char BAD_MOVE = 'l';

int main(){

	sef_startup();

	start_game();

	//vg_exit();
	return 0;
}

int start_game(){
	int i;

	subscribe();

	// Initializes the video memory in VIDEO_MODE (0x117)
	char *video_mem = vg_init(VIDEO_MODE);
	vg_fill(0x0000);

	player = create_sprite(ship, SHIP_START_X, SHIP_START_Y);
	vg_draw_sprite(player);

	enemies = (Sprite **)malloc(NO_ENEMIES * sizeof(Sprite));
	for(i=0; i<NO_ENEMIES; i++){
		if(i<8)
			enemies[i] = create_sprite(bad, ENEMIES_START_X+ENEMIES_X_INCREMENT*i, ENEMIES_START_Y);
		else
			enemies[i] = create_sprite(bad, ENEMIES_START_X2+ENEMIES_X_INCREMENT*(i-8), ENEMIES_START_Y+ENEMIES_Y_INCREMENT);
	}

	for(i=0; i<NO_ENEMIES; i++)
		vg_draw_sprite(enemies[i]);

	timer_set_square(0,60);

	mainloop();
}

void mainloop(){
	int ipc_status;
	message msg;
	int r;

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
							make_player_movement();
						}else if ((msg.NOTIFY_ARG & TIMER_BIT_MASK)){
							time = timer_int_handler(time);
							if(time % 60 == 0)
								make_bad_movement();
						}break;
					default: break;
				}
			}
	}

	exit_game();
}

void make_player_movement(){
	// MAKE MOVEMENT FUNCTION
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

void make_bad_movement(){
	int i;

	if(BAD_MOVE == 'l'){
		for(i=0; i<NO_ENEMIES; i++){
			vg_draw_rec(enemies[i]->x, enemies[i]->y, enemies[i]->x+enemies[i]->width, enemies[i]->y+enemies[i]->height, 0x0000);
			enemies[i]->x += 20;
			vg_draw_sprite(enemies[i]);
		}
		BAD_MOVE = 'd';
	} else {
		for(i=0; i<NO_ENEMIES; i++){
			vg_draw_rec(enemies[i]->x, enemies[i]->y, enemies[i]->x+enemies[i]->width, enemies[i]->y+enemies[i]->height, 0x0000);
			enemies[i]->x -= 20;
			vg_draw_sprite(enemies[i]);
		}
		BAD_MOVE = 'l';
	}
}

void exit_game(){

	unsubscribe();
	vg_exit();
}

int subscribe(){
	if(kbc_subscribe_exclusive() < 0){
			printf("ERROR SUBSCRIBING TO KBC!\n");
			return 1;
	}

	if(timer_subscribe_int() < 0){
			printf("ERROR SUBSCRIBING TO TIMER!\n");
			return 1;
	}
	return 0;
}

int unsubscribe(){
	if(kbc_unsubscribe())
		return 1;

	if(timer_unsubscribe_int())
		return 1;

	return 0;
}
