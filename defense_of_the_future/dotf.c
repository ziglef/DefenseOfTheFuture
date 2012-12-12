#include "dotf.h"

int start_game();
int subscribe();
int unsubscribe();
void mainloop();
void make_player_movement();
void make_bad_movement();
void exit_game();
void keystroke_handler();
void make_shooting();
void make_shooting_movement();
int is_in_screen(Sprite *spr);
int check_collision(Sprite *spr);
void make_explosion();
void remove_sprite(int x, int y);

Sprite *player;
Sprite **player_shots;
Sprite **enemies;
Sprite ***explosions;
unsigned long time = 0;
unsigned char kscancode = 0;
char BAD_MOVE = 'L';
int *EXPLOSIONS;
int note = 0;
char *video_mem;

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
	video_mem = vg_init(VIDEO_MODE);
	vg_fill(0x0000);

	player = create_sprite(ship, SHIP_START_X, SHIP_START_Y);
	vg_draw_sprite(player);

	player_shots = (Sprite **)malloc(NO_PSHOTS * sizeof(Sprite));
	for(i=0; i<NO_PSHOTS; i++){
		player_shots[i] = create_sprite(missgood, -50, -50);
	}

	enemies = (Sprite **)malloc(NO_ENEMIES * sizeof(Sprite));
	for(i=0; i<NO_ENEMIES; i++){
		if(i<8)
			enemies[i] = create_sprite(bad, ENEMIES_START_X+ENEMIES_X_INCREMENT*i, ENEMIES_START_Y);
		else
			enemies[i] = create_sprite(bad, ENEMIES_START_X2+ENEMIES_X_INCREMENT*(i-8), ENEMIES_START_Y+ENEMIES_Y_INCREMENT);
	}

	for(i=0; i<NO_ENEMIES; i++)
		vg_draw_sprite(enemies[i]);

	explosions = (Sprite ***)malloc(NO_EXPLOSIONS*NO_PSHOTS * sizeof(Sprite));
	for(i=0; i<NO_PSHOTS; i++){
		explosions[i] = (Sprite **)malloc(NO_EXPLOSIONS * sizeof(Sprite));
		explosions[i][0] = create_sprite(boom1, -75, -75);
		explosions[i][1] = create_sprite(boom2, -75, -75);
		explosions[i][2] = create_sprite(boom3, -75, -75);
		explosions[i][3] = create_sprite(boom4, -75, -75);
		explosions[i][4] = create_sprite(boom5, -75, -75);
	}

	EXPLOSIONS = (int *)calloc(NO_EXPLOSIONS,sizeof(int));

	timer_set_square(0,60);

	 if(speaker_ctrl(1))
	 {
	  printf("Speaker_ctrl Failed!\n");
	  return 1;
	 }

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
							keystroke_handler();
						}else if ((msg.NOTIFY_ARG & TIMER_BIT_MASK)){
							time = timer_int_handler(time);
							if(time % 60 == 0)
								make_bad_movement();
							if(time % 5 == 0)
								make_shooting_movement();
							if((time % 6 == 0) && ((EXPLOSIONS[0] != 0) || (EXPLOSIONS[1] != 0) || (EXPLOSIONS[2] != 0) || (EXPLOSIONS[3] != 0)))
								make_explosion();
							if(time % 17 == 0)
								make_music();
						}break;
					default: break;
				}
			}
	}

	exit_game();
}

void make_music(){

	if(timer_set_square(2,theme_song[note]))
	 {
	  printf("Timer_set_square Failed!\n");
	  return 1;
	 }
	note++;
	if(note == NOTAS_TOTAL) note = 0;
}

void keystroke_handler(){
	if((kscancode == WMAKE) || (kscancode == SMAKE) || (kscancode == DMAKE) || (kscancode == AMAKE)) make_player_movement();
	if(kscancode == SPACEMAKE) make_shooting();
}

int is_in_screen(Sprite *spr){
	if(spr->y == -50)
		return 0;
	else
		return 1;
}

void make_shooting(){
	int i;

	for(i=0; i<NO_PSHOTS; i++){
		if(!is_in_screen(player_shots[i])){
			player_shots[i]->x = player->x+player->width/2-player_shots[i]->width/2;
			player_shots[i]->y = player->y-player_shots[i]->height;
			player_shots[i]->yspeed = 30;
			vg_draw_sprite(player_shots[i]);
			break;
		}
	}
}

void make_shooting_movement(){
	int i;

	for(i=0; i<NO_PSHOTS; i++){
		if(is_in_screen(player_shots[i])){
			if(player_shots[i]->y-20 > player_shots[i]->height){
				vg_draw_rec(player_shots[i]->x, player_shots[i]->y, player_shots[i]->x+player_shots[i]->width, player_shots[i]->y+player_shots[i]->height, 0x0000);
				player_shots[i]->y -= player_shots[i]->yspeed;
				if(!check_collision(player_shots[i])){
					vg_draw_sprite(player_shots[i]);
				} else {
					EXPLOSIONS[i] = 1;
				}
			} else {
				vg_draw_rec(player_shots[i]->x, player_shots[i]->y, player_shots[i]->x+player_shots[i]->width, player_shots[i]->y+player_shots[i]->height, 0x0000);
				player_shots[i]->y = -50;
			}
		}
	}
}


int check_collision(Sprite *spr){
	int i,j;

	for(i=0; i<spr->height; i++){
		for(j=0; j<spr->width; j++){
			if(vg_get_pixel(spr->x+j, spr->y+i) != 0){
				remove_sprite(spr->x+j, spr->y+i);
				return 1;
			}
		}
	}

	return 0;
}

void remove_sprite(int x, int y){
	int i,j;

	for(i=0; i<NO_ENEMIES; i++){
		if((x >= enemies[i]->x) && (x < enemies[i]->x+enemies[i]->width) && (y >= enemies[i]->y) && (y < enemies[i]->y+enemies[i]->height)){
			vg_draw_rec(enemies[i]->x, enemies[i]->y, enemies[i]->x+enemies[i]->width, enemies[i]->y+enemies[i]->height, 0x0000);
			enemies[i]->x = -100;
			enemies[i]->y = -100;
		}
	}

	for(i=0; i<NO_PSHOTS; i++){
		if((x >= player_shots[i]->x) && (x < player_shots[i]->x+player_shots[i]->width) && (y >= player_shots[i]->y) && (y < player_shots[i]->y+player_shots[i]->height)){
			vg_draw_rec(player_shots[i]->x, player_shots[i]->y, player_shots[i]->x+player_shots[i]->width, player_shots[i]->y+player_shots[i]->height, 0x0000);
			for(j=0; j<NO_EXPLOSIONS; j++){
				explosions[i][j]->x = player_shots[i]->x - explosions[i][j]->width/2;
				explosions[i][j]->y = player_shots[i]->y - explosions[i][j]->height/2;
			}
			player_shots[i]->x = -100;
			player_shots[i]->y = -100;
		}
	}

}

void make_explosion(){
	int i;

	for(i=0; i<NO_EXPLOSIONS; i++){
		switch(EXPLOSIONS[i]){
			case 1:
				vg_draw_sprite(explosions[i][EXPLOSIONS[i]-1]);
				EXPLOSIONS[i]++;
				break;
			case 2:
				vg_draw_sprite(explosions[i][EXPLOSIONS[i]-1]);
				EXPLOSIONS[i]++;
				break;
			case 3:
				vg_draw_sprite(explosions[i][EXPLOSIONS[i]-1]);
				EXPLOSIONS[i]++;
				break;
			case 4:
				vg_draw_sprite(explosions[i][EXPLOSIONS[i]-1]);
				EXPLOSIONS[i]++;
				break;
			case 5:
				vg_draw_sprite(explosions[i][EXPLOSIONS[i]-1]);
				EXPLOSIONS[i]++;
				break;
			case 6:
				vg_draw_rec(explosions[i][EXPLOSIONS[i]-2]->x, explosions[i][EXPLOSIONS[i]-2]->y,
							explosions[i][EXPLOSIONS[i]-2]->x+explosions[i][EXPLOSIONS[i]-2]->width,
							explosions[i][EXPLOSIONS[i]-2]->y+explosions[i][EXPLOSIONS[i]-2]->height,
							0x0000);
				EXPLOSIONS[i] = 0;
				break;
			default:
				break;
		}
	}
}

void make_player_movement(){
	// MAKE MOVEMENT FUNCTION
	switch(kscancode){
		case WMAKE:
			if(player->y-20 >= 500){
				vg_draw_rec(player->x, player->y, player->x+player->width, player->y+player->height, 0x0000);
				player->y -= 20;
				vg_draw_sprite(player);
			}
			break;
		case SMAKE:
			if(player->y+20+player->height < 700){
				vg_draw_rec(player->x, player->y, player->x+player->width, player->y+player->height, 0x0000);
				player->y += 20;
				vg_draw_sprite(player);
			}
			break;
		case AMAKE:
			if(player->x-20 >= 100){
				vg_draw_rec(player->x, player->y, player->x+player->width, player->y+player->height, 0x0000);
				player->x -= 20;
				vg_draw_sprite(player);
			}
			break;
		case DMAKE:
			if(player->x+20+player->width < 924){
				vg_draw_rec(player->x, player->y, player->x+player->width, player->y+player->height, 0x0000);
				player->x += 20;
				vg_draw_sprite(player);
			}
			break;
	}
}

void make_bad_movement(){
	int i;

	if((BAD_MOVE == 'd') || (BAD_MOVE == 'D')){
		for(i=0; i<NO_ENEMIES; i++){
			vg_draw_rec(enemies[i]->x, enemies[i]->y, enemies[i]->x+enemies[i]->width, enemies[i]->y+enemies[i]->height, 0x0000);
			enemies[i]->x += 20;
			vg_draw_sprite(enemies[i]);
		}
		if(BAD_MOVE == 'd'){
			BAD_MOVE = 'D';
		} else BAD_MOVE = 'l';

	} else if((BAD_MOVE == 'l') || (BAD_MOVE == 'L')) {
		for(i=0; i<NO_ENEMIES; i++){
			vg_draw_rec(enemies[i]->x, enemies[i]->y, enemies[i]->x+enemies[i]->width, enemies[i]->y+enemies[i]->height, 0x0000);
			enemies[i]->x -= 20;
			vg_draw_sprite(enemies[i]);
		}
		if(BAD_MOVE == 'l'){
			BAD_MOVE = 'L';
		} else BAD_MOVE = 'd';

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
