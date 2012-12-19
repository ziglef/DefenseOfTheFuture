#include "dotf.h"

int start_game();
int subscribe();
int unsubscribe();
void mainloop();
int menuloop();
void make_player_movement();
void make_bad_movement();
int exit_game();
void keystroke_handler();
void make_shooting();
void make_shooting_movement();
int is_in_screen(Sprite *spr);
int check_collision(Sprite *spr);
void make_explosion();
void remove_sprite(int x, int y);
int make_music();
void check_game_over();
void draw_game_info(game_info gi, int noelements);
int start_menu();
void draw_menu();
int make_menu_music();
void make_gun_selection();

Sprite *player;
Sprite **player_shots;
Sprite **enemies;
Sprite ***explosions;
panel cPanel;
unsigned long time = 0;
unsigned char kscancode = 0;
char BAD_MOVE = 'L';
int *EXPLOSIONS;
int music_enabled = 0;
int music_note = 0;
int menu_music_enabled = 1;
int menu_music_note = 0;
int sfx_explosion_enabled = 0;
int sfx_explosion_note = 0;
int sfx_shot = 0;
char *video_mem;
int bad_count = NO_ENEMIES;
int END = 0;
int life = 8;
game_info lives = {229, 653, 3};
game_info level = {974, 653, 1};
game_info score = {795, 703, 4252};
game_info cash = {359, 653, 64825};
Sprite *menu;
Sprite **menu_buttons;
int atMenu = 1;
int menuOption = 0;
int gunOption = 0;
MouseController lemouse = {0x0C,0,0,0,0,0,{0,0,0}};
unsigned short counter;
unsigned char packet[3];
bool LMB_PRESSED = false;
bool RMB_PRESSED = false;
unsigned long byte;

int main(){

	sef_startup();

	start_menu();
	start_game();

	//vg_exit();
	return 0;
}

void draw_menu(){
	int i;

	for(i=0; i<6; i++){
		if(menuOption == i)
			vg_draw_sprite(menu_buttons[i*2]);
		else
			vg_draw_sprite(menu_buttons[(i*2)+1]);
	}
}

void make_gun_selection(){
	int i;

	if(LMB_PRESSED)
		if(gunOption = 0)
			gunOption = 3;
		else
			gunOption--;

	if(RMB_PRESSED)
		if(gunOption = 3)
			gunOption = 0;
		else
			gunOption++;

	for(i=0; i<4; i++){
		if(gunOption == i)
			vg_draw_sprite(cPanel.guns[i*2]);
		else
			vg_draw_sprite(cPanel.guns[(i*2)+1]);
	}
}

int start_menu(){
	subscribe();

	// Initializes the video memory in VIDEO_MODE (0x117)
	video_mem = vg_init(VIDEO_MODE);
	vg_fill(0x0000);

	menu = create_sprite(wallpaper, 0, 0);
	vg_draw_sprite(menu);

	menu_buttons = (Sprite **)malloc(12 * sizeof(Sprite));
	menu_buttons[0] = create_sprite(starton, 0 ,5);
	menu_buttons[1] = create_sprite(startoff, 0, 5);
	menu_buttons[2] = create_sprite(optionson, 0, 132);
	menu_buttons[3] = create_sprite(optionsoff, 0, 132);
	menu_buttons[4] = create_sprite(highscoreson, 0, 259);
	menu_buttons[5] = create_sprite(highscoresoff, 0, 259);
	menu_buttons[6] = create_sprite(helpon, 0, 386);
	menu_buttons[7] = create_sprite(helpoff, 0, 386);
	menu_buttons[8] = create_sprite(creditson, 0, 513);
	menu_buttons[9] = create_sprite(creditsoff, 0, 513);
	menu_buttons[10] = create_sprite(exiton, 0, 640);
	menu_buttons[11] = create_sprite(exitoff, 0, 640);

	draw_menu();

	timer_set_square(0,60);

	timer_set_square(2,no);
	if(speaker_ctrl(1))
	{
	 printf("Speaker_ctrl Failed!\n");
	 return 1;
	}

	menuloop();
}

int menuloop(){
	int ipc_status;
	message msg;
	int r;

	while(kscancode != ENTERBREAK){
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
							if(time % 6 == 0)
								make_menu_music();
						}break;
					default: break;
				}
			}
	}
	menu_music_enabled = 0;
	music_enabled = 1;
	atMenu = 0;
	//unsubscribe();
	if(speaker_ctrl(0)) {
		printf("Timer_Test_Int Failed!\n");
		return 1;
	}
}

int make_menu_music(){
	if(menu_music_enabled){
		if(timer_set_square(2,theme_menu[menu_music_note])){
			printf("Timer_set_square Failed!\n");
			return 1;
		}
	}
	menu_music_note++;
	if(menu_music_note == NOTAS_MENU) menu_music_note = 0;
}

int start_game(){
	int i;

	//subscribe();

	if(speaker_ctrl(1))
	{
	 printf("Speaker_ctrl Failed!\n");
	 return 1;
	}

	// Initializes the video memory in VIDEO_MODE (0x117)
	//video_mem = vg_init(VIDEO_MODE);
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
		explosions[i][5] = create_sprite(boom6, -75, -75);
		explosions[i][6] = create_sprite(boom7, -75, -75);
		explosions[i][7] = create_sprite(boom8, -75, -75);
		explosions[i][8] = create_sprite(boom9, -75, -75);
		explosions[i][9] = create_sprite(boom10, -75, -75);
	}

	EXPLOSIONS = (int *)calloc(NO_EXPLOSIONS,sizeof(int));

	// Control Panel Creation
	// Frame
	cPanel.frames = (Sprite **)malloc(4 * sizeof(Sprite));
	cPanel.frames[0] = create_sprite(left_right_frame, 0, 644);
	cPanel.frames[1] = create_sprite(left_right_frame, 1019, 644);
	cPanel.frames[2] = create_sprite(upper_lower_frame, 0, 640);
	cPanel.frames[3] = create_sprite(upper_lower_frame, 0, 764);

	cPanel.lives = create_sprite(livesS, 5, 644);
	cPanel.level = create_sprite(levelS, 681, 644);
	cPanel.score = create_sprite(scoreS, 681, 704);

	cPanel.lifebar = (Sprite **)malloc(8 * sizeof(Sprite));
	cPanel.lifebar[0] = create_sprite(lbarg, 10, 709);
	cPanel.lifebar[1] = create_sprite(lbarg, 51, 709);
	cPanel.lifebar[2] = create_sprite(lbarg, 92, 709);
	cPanel.lifebar[3] = create_sprite(lbarg, 133, 709);
	cPanel.lifebar[4] = create_sprite(lbarg, 174, 709);
	cPanel.lifebar[5] = create_sprite(lbarg, 215, 709);
	cPanel.lifebar[6] = create_sprite(lbarg, 256, 709);
	cPanel.lifebar[7] = create_sprite(lbarg, 297, 709);

	cPanel.dolar = create_sprite(dolarS, 643 ,644);

	cPanel.algarisms = (Sprite **)malloc(10 * sizeof(Sprite));
	cPanel.algarisms[0] = create_sprite(alg0, -100, -100);
	cPanel.algarisms[1] = create_sprite(alg1, -100, -100);
	cPanel.algarisms[2] = create_sprite(alg2, -100, -100);
	cPanel.algarisms[3] = create_sprite(alg3, -100, -100);
	cPanel.algarisms[4] = create_sprite(alg4, -100, -100);
	cPanel.algarisms[5] = create_sprite(alg5, -100, -100);
	cPanel.algarisms[6] = create_sprite(alg6, -100, -100);
	cPanel.algarisms[7] = create_sprite(alg7, -100, -100);
	cPanel.algarisms[8] = create_sprite(alg8, -100, -100);
	cPanel.algarisms[9] = create_sprite(alg9, -100, -100);

	cPanel.guns = (Sprite **)malloc(8 * sizeof(Sprite));
	cPanel.guns[0] = create_sprite(g1on, 343 ,704);
	cPanel.guns[1] = create_sprite(g1off, -100, -100);
	cPanel.guns[2] = create_sprite(g2on, -100, -100);
	cPanel.guns[3] = create_sprite(g2off, 427, 704);
	cPanel.guns[4] = create_sprite(g3on, -100, -100);
	cPanel.guns[5] = create_sprite(g3off, 511, 704);
	cPanel.guns[6] = create_sprite(g4on, -100, -100);
	cPanel.guns[7] = create_sprite(g4off, 595, 704);

	// Control Panel Draw
	vg_draw_sprite(cPanel.guns[0]);
	vg_draw_sprite(cPanel.guns[3]);
	vg_draw_sprite(cPanel.guns[5]);
	vg_draw_sprite(cPanel.guns[7]);

	vg_draw_sprite(cPanel.lives);
	vg_draw_sprite(cPanel.frames[0]);
	vg_draw_sprite(cPanel.frames[1]);
	vg_draw_sprite(cPanel.frames[2]);
	vg_draw_sprite(cPanel.frames[3]);
	vg_draw_sprite(cPanel.level);
	vg_draw_sprite(cPanel.score);

	vg_draw_sprite(cPanel.lifebar[0]);
	vg_draw_sprite(cPanel.lifebar[1]);
	vg_draw_sprite(cPanel.lifebar[2]);
	vg_draw_sprite(cPanel.lifebar[3]);
	vg_draw_sprite(cPanel.lifebar[4]);
	vg_draw_sprite(cPanel.lifebar[5]);
	vg_draw_sprite(cPanel.lifebar[6]);
	vg_draw_sprite(cPanel.lifebar[7]);

	vg_draw_sprite(cPanel.dolar);

	draw_game_info(lives, 1);
	draw_game_info(level, 1);
	draw_game_info(score, 4);
	draw_game_info(cash, 5);

	mainloop();
}

void mainloop(){
	int ipc_status;
	message msg;
	int r;

	while((kscancode != ESC_BREAKCODE) && (END == 0)){
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
							if((time % 3 == 0) && ((EXPLOSIONS[0] != 0) || (EXPLOSIONS[1] != 0) || (EXPLOSIONS[2] != 0) || (EXPLOSIONS[3] != 0)))
								make_explosion();
							if(time % 6 == 0)
								make_music();
						}else if((msg.NOTIFY_ARG & BIT(M_IRQ))){
							lemouse = mouse_handler();
							packet[0] = lemouse.bytes[0];
							packet[1] = lemouse.bytes[1];
							packet[2] = lemouse.bytes[2];
							counter = lemouse.counter;

							if(packet[0] & BIT(0))
								LMB_PRESSED = true;
							else
								LMB_PRESSED = false;

							if(packet[0] & BIT(1))
								RMB_PRESSED = true;
							else
								RMB_PRESSED = false;

							make_gun_selection();

						}break;
					default: break;
				}
			}
	}

	exit_game();
}

void draw_game_info(game_info gi, int noelements){
	int i = 0;
	int num;
	int elements[noelements];

	if(noelements == 1){
		cPanel.algarisms[gi.value]->x = gi.x;
		cPanel.algarisms[gi.value]->y = gi.y;
		vg_draw_sprite(cPanel.algarisms[gi.value]);
	}else{
		num = gi.value;
		while (num > 0){
		    elements[i] = num % 10;
		    num = num / 10;
		    i++;
		}

		for(i=noelements-1; i>=0; i--){
			cPanel.algarisms[elements[i]]->x = gi.x+60*(noelements-(i+1));
			cPanel.algarisms[elements[i]]->y = gi.y;
			vg_draw_sprite(cPanel.algarisms[elements[i]]);
		}
	}
}

void check_game_over(){

	if(bad_count == 0)
		END = 1;
}

int make_music(){

	if(music_enabled){
		if(timer_set_square(2,theme_loop[music_note])){
			printf("Timer_set_square Failed!\n");
			return 1;
		}
	}
	music_note++;
	if(music_note == NOTAS_LOOP) music_note = 0;

	if(sfx_explosion_enabled){
		if(timer_set_square(2,explosion1[sfx_explosion_note])){
			printf("Timer_set_square Failed!\n");
			return 1;
		}
		sfx_explosion_note++;
		if(sfx_explosion_note == NOTAS_EXPL){
			sfx_explosion_note = 0;
			sfx_explosion_enabled = 0;
			music_enabled = 1;
		}
	}

	if(sfx_shot){
		if(timer_set_square(2,shots[0])){
			printf("Timer_set_square Failed!\n");
			return 1;
		}
		sfx_shot = 0;
		music_enabled = 1;
	}

}

void keystroke_handler(){
	if((kscancode == WMAKE) || (kscancode == SMAKE) || (kscancode == DMAKE) || (kscancode == AMAKE)) make_player_movement();
	if(kscancode == SPACEMAKE) make_shooting();
	if(atMenu){
		if(kscancode == DOWNMAKE)
			if(menuOption != 5){
				menuOption++;
				draw_menu();
			}
		if(kscancode == UPMAKE)
			if(menuOption != 0){
				menuOption--;
				draw_menu();
			}
	}
}

int is_in_screen(Sprite *spr){
	if((spr->y >= 0) && (spr->y < 768) && (spr->x >= 0) && (spr->x < 1024))
		return 1;
	else
		return 0;
}

void make_shooting(){
	int i;

	for(i=0; i<NO_PSHOTS; i++){
		if(!is_in_screen(player_shots[i])){
			player_shots[i]->x = player->x+player->width/2-player_shots[i]->width/2;
			player_shots[i]->y = player->y-player_shots[i]->height;
			player_shots[i]->yspeed = 30;
			vg_draw_sprite(player_shots[i]);
			sfx_shot = 1;
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
					sfx_explosion_enabled = 1;
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
			bad_count--;
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
		if((EXPLOSIONS[i] != 11) && (EXPLOSIONS[i] != 0)){
			vg_draw_sprite(explosions[i][EXPLOSIONS[i]-1]);
			EXPLOSIONS[i]++;
		} else if(EXPLOSIONS[i] != 0){
			vg_draw_rec(explosions[i][EXPLOSIONS[i]-2]->x, explosions[i][EXPLOSIONS[i]-2]->y,
						explosions[i][EXPLOSIONS[i]-2]->x+explosions[i][EXPLOSIONS[i]-2]->width,
						explosions[i][EXPLOSIONS[i]-2]->y+explosions[i][EXPLOSIONS[i]-2]->height,
						0x0000);
			EXPLOSIONS[i] = 0;
			check_game_over();
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
			if(player->y+20+player->height <= 630){
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

int exit_game(){

	if(speaker_ctrl(0)) {
		printf("Timer_Test_Int Failed!\n");
		return 1;
	}
	unsubscribe();
	vg_exit();
	return 0;
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
	if(mouse_subscribe_exclusive() < 0){
			printf("ERROR SUBSCRIBING TO KBC!\n");
			return 1;
	}

	turn_mouse_on();

	do{
		sys_inb(KBC_STAT, &(lemouse.status));
		if(lemouse.status & KBC_STAT_OBF)
			sys_inb(KBC_O_BUF, &byte);
	}while(lemouse.status & KBC_STAT_OBF);
	return 0;
}

int unsubscribe(){
	if(timer_unsubscribe_int())
		return 1;

	if(kbc_unsubscribe())
		return 1;

	if(mouse_unsubscribe())
		return 1;

	do{
		sys_inb(KBC_STAT, &(lemouse.status));
		if(lemouse.status & KBC_STAT_OBF)
			sys_inb(KBC_O_BUF, &byte);
	}while(lemouse.status & KBC_STAT_OBF);
	return 0;
}
