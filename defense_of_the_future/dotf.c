#include "dotf.h"

// TODO: player exploding
// TODO: level 2 and level 3
// TODO: write highscores at the end

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
int make_victory_music();
void make_bad_shooting();
int is_in_screen_bads(Sprite *spr);
int check_collision_bad(Sprite *spr);
void remove_sprite_bad(int x, int y);
void make_explosion_bad();
void draw_lifebar();
void options_menu();
void option_handler();
void draw_options();
void draw_highscores();
void draw_help();
void draw_credits();
void draw_strings(char *string, int n, int xi,int yi);
void draw_shop();
void shop_menu();
void shop_handler();

Sprite *laser;
Sprite *player;
Sprite **player_shots;
Sprite **enemies;
Sprite ***enemies_shots;
int bad_shot_random = 0;
Sprite ***explosions;
panel cPanel;
unsigned long timec = 0;
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
int end_music_note = 0;
int life = 8;
game_info lives = {229, 653, 3};
game_info level = {974, 653, 1};
game_info score = {795, 703, 0};
game_info cash = {359, 653, 10000};
Sprite *menu;
Sprite **menu_buttons;
int atMenu = 1;
int menuOption = 0;
int gunOption = 0;
Sprite *victory;
int guns[4] = {1, 1, 1, 1};
Sprite ***playerExplosions;
int *PLAYEREXPLOSIONS;
int levelvar = 1;
int optionsOption = 0;
int optionsDifi = 1;
int optionsSound = 1;
Sprite **options_buttons;
Sprite **help;
int lifebuy;
char *nomoney = "not enough money";
char *alreadyo = "already owned";

int main(){

	sef_startup();
	srand(time(NULL));

	subscribe();

	start_menu();
	//start_game();

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

	for(i=0; i<4; i++){
		if(gunOption == i){
			if(guns[i]){
				switch(i){
					case 0: vg_draw_sprite(cPanel.guns[0]); break;
					case 1: vg_draw_sprite(cPanel.guns[2]); break;
					case 2: vg_draw_sprite(cPanel.guns[5]); break;
					case 3: vg_draw_sprite(cPanel.guns[8]); break;
					default: break;
				}
			} else {
				switch(i){
					case 0: vg_draw_sprite(cPanel.guns[1]); break;
					case 1: vg_draw_sprite(cPanel.guns[4]); break;
					case 2: vg_draw_sprite(cPanel.guns[7]); break;
					case 3: vg_draw_sprite(cPanel.guns[10]); break;
					default: break;
				}
			}
		} else {
			if(guns[i]){
				switch(i){
					case 0: vg_draw_sprite(cPanel.guns[1]); break;
					case 1: vg_draw_sprite(cPanel.guns[3]); break;
					case 2: vg_draw_sprite(cPanel.guns[6]); break;
					case 3: vg_draw_sprite(cPanel.guns[9]); break;
					default: break;
				}
			} else {
				switch(i){
					case 0: vg_draw_sprite(cPanel.guns[1]); break;
					case 1: vg_draw_sprite(cPanel.guns[4]); break;
					case 2: vg_draw_sprite(cPanel.guns[7]); break;
					case 3: vg_draw_sprite(cPanel.guns[10]); break;
					default: break;
				}
			}
		}
	}
}

int start_menu(){
	//subscribe();

	// Initializes the video memory in VIDEO_MODE (0x117)
	video_mem = vg_init(VIDEO_MODE);
	vg_fill(0x0000);

	menu = create_sprite(wallpaper, 500, 16);
	vg_draw_sprite(menu);

	menu_buttons = (Sprite **)malloc(12 * sizeof(Sprite));
	menu_buttons[0] = create_sprite(starton, 40, 25);
	menu_buttons[1] = create_sprite(startoff, 40, 25);
	menu_buttons[2] = create_sprite(optionson, 40, 152);
	menu_buttons[3] = create_sprite(optionsoff, 40, 152);
	menu_buttons[4] = create_sprite(highscoreson, 40, 279);
	menu_buttons[5] = create_sprite(highscoresoff, 40, 279);
	menu_buttons[6] = create_sprite(helpon, 40, 406);
	menu_buttons[7] = create_sprite(helpoff, 40, 406);
	menu_buttons[8] = create_sprite(creditson, 40, 533);
	menu_buttons[9] = create_sprite(creditsoff, 40, 533);
	menu_buttons[10] = create_sprite(exiton, 40, 660);
	menu_buttons[11] = create_sprite(exitoff, 40, 660);

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
						if((msg.NOTIFY_ARG & KBC_BIT_MASK)){
							kscancode = kbc_handler();
							keystroke_handler();
						}
						if ((msg.NOTIFY_ARG & TIMER_BIT_MASK)){
							timec = timer_int_handler(timec);
							if(timec % 5 == 0)
								make_menu_music();
						}
						break;
					default: break;
				}
			}
	}
	kscancode = 0;
	atMenu = 0;

	switch(menuOption){
		case 0:
			//butões menu
			menu_music_enabled = 0;
			music_enabled = 1;

			if(speaker_ctrl(0)) {
				printf("Timer_Test_Int Failed!\n");
				return 1;
			}

			start_game();
			break;
		case 1:
			options_menu();
			atMenu = 1;
			start_menu();
			break;
		case 2:
			draw_highscores();
			atMenu = 1;
			start_menu();
			break;
		case 3:
			draw_help();
			atMenu = 1;
			start_menu();
			break;
		case 4:
			draw_credits();
			atMenu = 1;
			start_menu();
			break;
		case 5:
			exit_game();
			break;
						}
	/*
	//butões menu
	menu_music_enabled = 0;
	music_enabled = 1;
	atMenu = 0;

	if(speaker_ctrl(0)) {
		printf("Timer_Test_Int Failed!\n");
		return 1;
	}*/
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

	laser = create_sprite(laserS, -50, -50);

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

	enemies_shots = (Sprite ***)malloc(2*NO_ENEMIES * sizeof(Sprite));
	for(i=0; i<NO_ENEMIES; i++){
		enemies_shots[i] = (Sprite **)malloc(2 * sizeof(Sprite));
		enemies_shots[i][0] = create_sprite(missbad, -50, -50);
		enemies_shots[i][1] = create_sprite(missbad, -50, -50);
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

	playerExplosions = (Sprite ***)malloc(NO_EXPLOSIONS*NO_PSHOTS * sizeof(Sprite));
	for(i=0; i<NO_PSHOTS; i++){
		playerExplosions[i] = (Sprite **)malloc(NO_EXPLOSIONS * sizeof(Sprite));
		playerExplosions[i][0] = create_sprite(boom1, -75, -75);
		playerExplosions[i][1] = create_sprite(boom2, -75, -75);
		playerExplosions[i][2] = create_sprite(boom3, -75, -75);
		playerExplosions[i][3] = create_sprite(boom4, -75, -75);
		playerExplosions[i][4] = create_sprite(boom5, -75, -75);
		playerExplosions[i][5] = create_sprite(boom6, -75, -75);
		playerExplosions[i][6] = create_sprite(boom7, -75, -75);
		playerExplosions[i][7] = create_sprite(boom8, -75, -75);
		playerExplosions[i][8] = create_sprite(boom9, -75, -75);
		playerExplosions[i][9] = create_sprite(boom10, -75, -75);
	}

	EXPLOSIONS = (int *)calloc(NO_EXPLOSIONS,sizeof(int));
	PLAYEREXPLOSIONS = (int *)calloc(NO_EXPLOSIONS,sizeof(int));

	// Control Panel Creation
	// Frame
	cPanel.frames = (Sprite **)malloc(4 * sizeof(Sprite));
	cPanel.frames[0] = create_sprite(left_right_frame, 0, 644);
	cPanel.frames[1] = create_sprite(left_right_frame, 1019, 644);
	cPanel.frames[2] = create_sprite(upper_lower_frame, 0, 640);
	cPanel.frames[3] = create_sprite(upper_lower_frame, 0, 764);

	cPanel.lives = create_sprite(livesS, 20, 653);
	cPanel.level = create_sprite(levelS, 698, 653);
	cPanel.score = create_sprite(scoreS, 684, 720);
	cPanel.dolar = create_sprite(dolarS, 653 ,653);

	cPanel.lifebar = (Sprite **)malloc(8 * sizeof(Sprite));
	cPanel.lifebar[0] = create_sprite(lbarg, 10, 709);
	cPanel.lifebar[1] = create_sprite(lbarg, 51, 709);
	cPanel.lifebar[2] = create_sprite(lbarg, 92, 709);
	cPanel.lifebar[3] = create_sprite(lbarg, 133, 709);
	cPanel.lifebar[4] = create_sprite(lbarg, 174, 709);
	cPanel.lifebar[5] = create_sprite(lbarg, 215, 709);
	cPanel.lifebar[6] = create_sprite(lbarg, 256, 709);
	cPanel.lifebar[7] = create_sprite(lbarg, 297, 709);

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

	cPanel.guns = (Sprite **)malloc(11 * sizeof(Sprite));
	cPanel.guns[0] = create_sprite(g1on, 343 ,704);
	cPanel.guns[1] = create_sprite(g1off, 343, 704);
	cPanel.guns[2] = create_sprite(g2on, 427, 704);
	cPanel.guns[3] = create_sprite(g2off, 427, 704);
	cPanel.guns[4] = create_sprite(g2lock, 427, 704);
	cPanel.guns[5] = create_sprite(g3on, 511, 704);
	cPanel.guns[6] = create_sprite(g3off, 511, 704);
	cPanel.guns[7] = create_sprite(g3lock, 511, 704);
	cPanel.guns[8] = create_sprite(g4on, 595, 704);
	cPanel.guns[9] = create_sprite(g4off, 595, 704);
	cPanel.guns[10] = create_sprite(g4lock, 595, 704);

	// Control Panel Draw
	make_gun_selection();

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

	// Victory Sprite
	victory = create_sprite(victoryS, 22, 364);

	mainloop();
}

void mainloop(){
	int ipc_status;
	message msg;
	int r;

	while((kscancode != ESC_BREAKCODE) && (END != 2)){
			r = driver_receive(ANY, &msg, &ipc_status);
			if( r != 0 ){
				printf("driver_receive failed with %d\n", r);
				continue;
			}
			if(is_ipc_notify(ipc_status)){

				switch(_ENDPOINT_P(msg.m_source)){
					case HARDWARE:
						if(END == 0){
							if((msg.NOTIFY_ARG & KBC_BIT_MASK)){
								kscancode = kbc_handler();
								keystroke_handler();
							}
							if ((msg.NOTIFY_ARG & TIMER_BIT_MASK)){
								timec = timer_int_handler(timec);
								if(timec % 60 == 0)
									make_bad_movement();
								if(timec % 5 == 0){
									make_shooting_movement();
									make_bad_shooting();
								}
								if((timec % 3 == 0) && ((EXPLOSIONS[0] != 0) || (EXPLOSIONS[1] != 0) || (EXPLOSIONS[2] != 0) || (EXPLOSIONS[3] != 0)))
									make_explosion();
								if((timec % 3 == 0) && ((PLAYEREXPLOSIONS[0] != 0) || (PLAYEREXPLOSIONS[1] != 0) || (PLAYEREXPLOSIONS[2] != 0) || (PLAYEREXPLOSIONS[3] != 0)))
									make_explosion_bad();
								if(timec % 6 == 0)
									make_music();
							}
						} else {
							if(levelvar != 3){
								shop_menu();
								END = 0;
								break;
							}
							if ((msg.NOTIFY_ARG & TIMER_BIT_MASK)){
								timec = timer_int_handler(timec);
								if(timec % 5 == 0)
									make_victory_music();
							}
						}
						break;
					default: break;
				}
			}
	}
	exit_game();
}

int make_victory_music(){
	if(END == 1){
		if(timer_set_square(2,theme_victory[end_music_note])){
			printf("Timer_set_square Failed!\n");
			return 1;
		}
		end_music_note++;
		if(end_music_note == NOTAS_VICT) END=2;
	}
}

void draw_game_info(game_info gi, int noelements){
	int i = noelements-1;
	int num;
	int elements[noelements];

	if(noelements == 1){
		cPanel.algarisms[gi.value]->x = gi.x;
		cPanel.algarisms[gi.value]->y = gi.y;
		vg_draw_sprite(cPanel.algarisms[gi.value]);
	}else{
		num = gi.value;
		while (i >= 0){
		    elements[i] = num % 10;
		    num = num / 10;
		    i--;
		}

		for(i=0; i<noelements; i++){
			cPanel.algarisms[elements[i]]->x = gi.x+60*i;
			cPanel.algarisms[elements[i]]->y = gi.y;
			vg_draw_sprite(cPanel.algarisms[elements[i]]);
		}
	}
}

void check_game_over(){

	if(bad_count == 0){
		END = 1;
		timer_set_square(2, no);
		vg_fill(0);
		vg_draw_sprite(victory);
	}
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
	if(atMenu){
		if(kscancode == DOWNMAKE)
			if(menuOption != 5)
				menuOption++;
			else
				menuOption = 0;
		if(kscancode == UPMAKE)
			if(menuOption != 0)
				menuOption--;
			else
				menuOption = 5;
		draw_menu();
	} else {
		if((kscancode == WMAKE) || (kscancode == SMAKE) || (kscancode == DMAKE) || (kscancode == AMAKE)) make_player_movement();
		if(kscancode == SPACEMAKE) make_shooting();
		if((kscancode == NO1MAKE) || (kscancode == NO2MAKE) || (kscancode == NO3MAKE) || (kscancode == NO4MAKE)){
			if(guns[kscancode-2] == 1)
				gunOption = kscancode-2;
			make_gun_selection();
		}
	}
}

int is_in_screen(Sprite *spr){
	if((spr->y >= 0) && (spr->y < 768) && (spr->x >= 0) && (spr->x < 1024))
		return 1;
	else
		return 0;
}

int is_in_screen_bads(Sprite *spr){
	if((spr->y >= 0) && (spr->y < 640) && (spr->x >= 0) && (spr->x < 1024))
		return 1;
	else
		return 0;
}

void make_shooting(){
	int i;

	if(gunOption == 0)
		for(i=0; i<NO_PSHOTS; i++){
			if(!is_in_screen(player_shots[i])){
				player_shots[i]->x = player->x+player->width/2-player_shots[i]->width/2;
				player_shots[i]->y = player->y-player_shots[i]->height;
				player_shots[i]->yspeed = 30;
				player_shots[i]->xspeed = 0;
				vg_draw_sprite(player_shots[i]);

				sfx_shot = 1;
				break;
			}
		}

	if(gunOption == 1)
		for(i=0; i<NO_PSHOTS/2; i++){
			if((!is_in_screen(player_shots[i*2])) && (!is_in_screen(player_shots[i*2+1]))){
				player_shots[i*2]->x = player->x;
				player_shots[i*2]->y = player->y-player_shots[i*2]->height;
				player_shots[i*2]->yspeed = 30;
				player_shots[i*2]->xspeed = 0;
				vg_draw_sprite(player_shots[i*2]);

				player_shots[i*2+1]->x = player->x+player->width-player_shots[i*2]->width;
				player_shots[i*2+1]->y = player->y-player_shots[i*2]->height;
				player_shots[i*2+1]->yspeed = 30;
				player_shots[i*2+1]->xspeed = 0;
				vg_draw_sprite(player_shots[i*2+1]);

				sfx_shot = 1;
				break;
			}
		}

	if(gunOption == 2){
		if((!is_in_screen(player_shots[0])) && (!is_in_screen(player_shots[1])) && (!is_in_screen(player_shots[2])) && (!is_in_screen(player_shots[3]))){
			player_shots[0]->x = player->x;
			player_shots[0]->y = player->y-player_shots[0]->height;
			player_shots[0]->yspeed = 30;
			player_shots[0]->xspeed = 10;
			vg_draw_sprite(player_shots[0]);

			player_shots[1]->x = player->x+player->width-player_shots[1]->width;
			player_shots[1]->y = player->y-player_shots[1]->height;
			player_shots[1]->yspeed = 30;
			player_shots[1]->xspeed = -10;
			vg_draw_sprite(player_shots[1]);

			player_shots[2]->x = player->x-player->width/2;
			player_shots[2]->y = player->y-player_shots[2]->height;
			player_shots[2]->yspeed = 30;
			player_shots[2]->xspeed = 20;
			vg_draw_sprite(player_shots[2]);

			player_shots[3]->x = player->x+player->width+player->width/2;
			player_shots[3]->y = player->y-player_shots[3]->height;
			player_shots[3]->yspeed = 30;
			player_shots[3]->xspeed = -20;
			vg_draw_sprite(player_shots[3]);

			sfx_shot = 1;
		}
	}

	if(gunOption == 3)
		if(!is_in_screen(laser)){
			laser->x = player->x+player->width/2-laser->width/2;
			laser->y = player->y-laser->height;
			laser->yspeed = 90;
			laser->xspeed = 0;
			vg_draw_sprite(laser);

			sfx_shot = 1;
		}
}

void make_bad_shooting(){
	int i,j;

	for(i=0; i<NO_ENEMIES; i++){
		for(j=0; j<2; j++){
			bad_shot_random = rand() % 100;
			if(bad_shot_random % 50 == 0){
				if(!is_in_screen(enemies_shots[i][j])){
					enemies_shots[i][j]->x = enemies[i]->x+enemies[i]->width/2-enemies_shots[i][j]->width/2;
					enemies_shots[i][j]->y = enemies[i]->y+enemies[i]->height;
					enemies_shots[i][j]->yspeed = 30;
					vg_draw_sprite(enemies_shots[i][j]);
					break;
				}
			}
		}
	}
}

void make_shooting_movement(){
	int i, j;

	for(i=0; i<NO_PSHOTS; i++){
		if(is_in_screen(player_shots[i])){
			if(player_shots[i]->y-20 > player_shots[i]->height){
				vg_draw_rec(player_shots[i]->x, player_shots[i]->y, player_shots[i]->x+player_shots[i]->width, player_shots[i]->y+player_shots[i]->height, 0x0000);
				player_shots[i]->y -= player_shots[i]->yspeed;
				player_shots[i]->x += player_shots[i]->xspeed;
				if(!check_collision(player_shots[i])){
					vg_draw_sprite(player_shots[i]);
				} else {
					EXPLOSIONS[i] = 1;
					sfx_explosion_enabled = 1;
					cash.value += 500;
					score.value += 50;
					draw_game_info(score, 4);
					draw_game_info(cash, 5);
				}
			} else {
				vg_draw_rec(player_shots[i]->x, player_shots[i]->y, player_shots[i]->x+player_shots[i]->width, player_shots[i]->y+player_shots[i]->height, 0x0000);
				player_shots[i]->y = -50;
			}
		}
	}

	for(i=0; i<NO_ENEMIES; i++){
		for(j=0; j<2; j++){
			if(is_in_screen_bads(enemies_shots[i][j])){
				if(enemies_shots[i][j]->y+20 < SHIP_START_Y+player->height){
					vg_draw_rec(enemies_shots[i][j]->x, enemies_shots[i][j]->y, enemies_shots[i][j]->x+enemies_shots[i][j]->width, enemies_shots[i][j]->y+enemies_shots[i][j]->height, 0x0000);
					enemies_shots[i][j]->y += enemies_shots[i][j]->yspeed;
					if(!check_collision_bad(enemies_shots[i][j])){
						vg_draw_sprite(enemies_shots[i][j]);
					} else {

					}
				} else {
					vg_draw_rec(enemies_shots[i][j]->x, enemies_shots[i][j]->y, enemies_shots[i][j]->x+enemies_shots[i][j]->width, enemies_shots[i][j]->y+enemies_shots[i][j]->height, 0x0000);
					enemies_shots[i][j]->y = -50;
				}
			}
		}
	}

	if(is_in_screen(laser)){
		if(laser->y-20 > laser->height){
			vg_draw_rec(laser->x, laser->y, laser->x+laser->width, laser->y+laser->height, 0x0000);
			laser->y -= laser->yspeed;
			laser->x += laser->xspeed;
			if(!check_collision(laser)){
				vg_draw_sprite(laser);
			} else {
				EXPLOSIONS[0] = 1;
				sfx_explosion_enabled = 1;
				cash.value += 500;
				score.value += 50;
				draw_game_info(score, 4);
				draw_game_info(cash, 5);
			}
		} else {
			vg_draw_rec(laser->x, laser->y, laser->x+laser->width, laser->y+laser->height, 0x0000);
			laser->y = -50;
		}
	}


}

int check_collision_bad(Sprite *spr){
	int i,j;

	for(i=0; i<spr->height; i++){
		for(j=0; j<spr->width; j++){
			if((spr->x+j >= player->x) && (spr->x+j < player->x+player->width) && (spr->y+i >= player->y) && (spr->y+i < player->y+player->height) && (vg_get_pixel(spr->x+j, spr->y+i) != 0)){
				remove_sprite_bad(spr->x+j, spr->y+i);
				return 1;
			}
		}
	}
	return 0;
}

int check_collision(Sprite *spr){
	int i,j,k;

	for(i=0; i<spr->height; i++){
		for(j=0; j<spr->width; j++){
			for(k=0; k<NO_ENEMIES; k++){
				if((spr->x+j >= enemies[k]->x) && (spr->x+j < enemies[k]->x+enemies[k]->width) && (spr->y+i >= enemies[k]->y) && (spr->y+i < enemies[k]->y+enemies[k]->height) && (vg_get_pixel(spr->x+j, spr->y+i) != 0)){
					remove_sprite(spr->x+j, spr->y+i);
					return 1;
				}
			}
		}
	}

	return 0;
}

void draw_life_bar(){
	vg_draw_sprite(cPanel.lifebar[0]);
	vg_draw_sprite(cPanel.lifebar[1]);
	vg_draw_sprite(cPanel.lifebar[2]);
	vg_draw_sprite(cPanel.lifebar[3]);
	vg_draw_sprite(cPanel.lifebar[4]);
	vg_draw_sprite(cPanel.lifebar[5]);
	vg_draw_sprite(cPanel.lifebar[6]);
	vg_draw_sprite(cPanel.lifebar[7]);
}

void make_life_bar(char lcolor){
	int i;

	for(i=0; i<life; i++){
		destroy_sprite(cPanel.lifebar[i]);
		switch(lcolor){
			case 'r':
				cPanel.lifebar[i] = create_sprite(lbarr, 10+41*i, 709);
				break;
			case 'y':
				cPanel.lifebar[i] = create_sprite(lbary, 10+41*i, 709);
				break;
			case 'g':
				cPanel.lifebar[i] = create_sprite(lbarg, 10+41*i, 709);
				break;
			default: break;
		}
	}

	for(i=life; i<8; i++){
		destroy_sprite(cPanel.lifebar[i]);
		cPanel.lifebar[i] = create_sprite(lbarb, 10+41*i, 709);
	}
}

void remove_sprite_bad(int x, int y){
	int i,j,k,l;

	for(i=0; i<NO_ENEMIES; i++){
		for(j=0; j<2; j++){
			if((x >= enemies_shots[i][j]->x) && (x < enemies_shots[i][j]->x+enemies_shots[i][j]->width) && (y >= enemies_shots[i][j]->y) && (y < enemies_shots[i][j]->y+enemies_shots[i][j]->height)){
				vg_draw_rec(enemies_shots[i][j]->x, enemies_shots[i][j]->y, enemies_shots[i][j]->x+enemies_shots[i][j]->width, enemies_shots[i][j]->y+enemies_shots[i][j]->height, 0x0000);
				enemies_shots[i][j]->x = -100;
				enemies_shots[i][j]->y = -100;
				life -= 1*optionsDifi;
				if(life < 0)
					life = 0;

				if(life != 0){
					if(life <= 2)
						make_life_bar('r');
					else if(life <= 4)
						make_life_bar('y');
					else
						make_life_bar('g');
					draw_life_bar();
				}

				if(life == 0){
					for(k=0; k<NO_PSHOTS; k++){
						for(l=0; l<NO_EXPLOSIONS; l++){
							playerExplosions[k][l]->x = enemies_shots[i][j]->x - playerExplosions[k][l]->width/2;
							playerExplosions[k][l]->y = enemies_shots[i][j]->y - playerExplosions[k][l]->height/2;

							switch(k){
								case 0:
									playerExplosions[k][l]->x -= 48;
									playerExplosions[k][l]->y -= 54;
									break;
								case 1:
									playerExplosions[k][l]->x -= 18;
									playerExplosions[k][l]->y += 24;
									break;
								case 2:
									playerExplosions[k][l]->x += 16;
									playerExplosions[k][l]->y -= 33;
									break;
								case 3:
									playerExplosions[k][l]->x -= 73;
									playerExplosions[k][l]->y += 15;
									break;
								default: break;
							}
						}
					}
					PLAYEREXPLOSIONS[0] = 1;
					PLAYEREXPLOSIONS[1] = 3;
					PLAYEREXPLOSIONS[2] = 5;
					PLAYEREXPLOSIONS[3] = 7;
				}
			}
		}
	}
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

void make_explosion_bad(){
	int i;

	for(i=0; i<NO_EXPLOSIONS; i++){
		if((PLAYEREXPLOSIONS[i] != 11) && (PLAYEREXPLOSIONS[i] != 0)){
			vg_draw_sprite(playerExplosions[i][PLAYEREXPLOSIONS[i]-1]);
			PLAYEREXPLOSIONS[i]++;
		} else if(PLAYEREXPLOSIONS[i] != 0){
			vg_draw_rec(playerExplosions[i][PLAYEREXPLOSIONS[i]-2]->x, playerExplosions[i][PLAYEREXPLOSIONS[i]-2]->y,
						playerExplosions[i][PLAYEREXPLOSIONS[i]-2]->x+playerExplosions[i][PLAYEREXPLOSIONS[i]-2]->width,
						playerExplosions[i][PLAYEREXPLOSIONS[i]-2]->y+playerExplosions[i][PLAYEREXPLOSIONS[i]-2]->height,
						0x0000);
			PLAYEREXPLOSIONS[i] = 0;
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
	/*
	if(mouse_subscribe_exclusive() < 0){
			printf("ERROR SUBSCRIBING TO KBC!\n");
			return 1;
	}*/
	if(kbc_subscribe_exclusive() < 0){
			printf("ERROR SUBSCRIBING TO KBC!\n");
			return 1;
	}

	if(timer_subscribe_int() < 0){
			printf("ERROR SUBSCRIBING TO TIMER!\n");
			return 1;
	}

	//turn_mouse_on();
/*
	do{
		sys_inb(KBC_STAT, &(lemouse.status));
		if(lemouse.status & KBC_STAT_OBF)
			sys_inb(KBC_O_BUF, &byte);
	}while(lemouse.status & KBC_STAT_OBF);
	*/
	return 0;
}

int unsubscribe(){
	unsigned long status;
	unsigned long garbage;

	do{
		sys_inb(KBC_STAT, &status);
		if(status & KBC_STAT_OBF)
			sys_inb(KBC_O_BUF, &garbage);
	}while(status & KBC_STAT_OBF);

	if(kbc_unsubscribe())
		return 1;

	if(timer_unsubscribe_int())
		return 1;
/*
	if(mouse_unsubscribe())
		return 1;

	do{
		sys_inb(KBC_STAT, &(lemouse.status));
		if(lemouse.status & KBC_STAT_OBF)
			sys_inb(KBC_O_BUF, &byte);
	}while(lemouse.status & KBC_STAT_OBF);*/
	return 0;
}


void options_menu(){

	int ipc_status;
	message msg;
	int r;

	vg_fill(0x000000);
	options_buttons = (Sprite **)malloc(12* sizeof(Sprite));
	options_buttons[0] = create_sprite(difficultyon, 367, 50);
	options_buttons[1] = create_sprite(difficultyoff, 367, 50);
	options_buttons[2] = create_sprite(easyon, 239, 100);
	options_buttons[3] = create_sprite(easyoff, 239, 100);
	options_buttons[4] = create_sprite(normalon, 383, 100);
	options_buttons[5] = create_sprite(normaloff, 383, 100);
	options_buttons[6] = create_sprite(insaneon, 595, 100);
	options_buttons[7] = create_sprite(insaneoff, 595, 100);
	options_buttons[8] = create_sprite(soundon, 412, 300);
	options_buttons[9] = create_sprite(soundoff, 412, 300);
	options_buttons[10] = create_sprite(speakeron, 412, 375);
	options_buttons[11] = create_sprite(speakeroff, 412, 375);

	/*
	vg_draw_sprite(options_buttons[0]);
	vg_draw_sprite(options_buttons[2]);
	vg_draw_sprite(options_buttons[5]);
	vg_draw_sprite(options_buttons[7]);
	vg_draw_sprite(options_buttons[9]);
	vg_draw_sprite(options_buttons[10]);
*/
	optionsOption = 0;
	draw_options();



		while(kscancode != ENTERBREAK){
			r = driver_receive(ANY, &msg, &ipc_status);
			if( r != 0 ){
				printf("driver_receive failed with %d\n", r);
				continue;
			}

			if(is_ipc_notify(ipc_status)){

				switch(_ENDPOINT_P(msg.m_source)){
					case HARDWARE:
						if((msg.NOTIFY_ARG & KBC_BIT_MASK)){
							kscancode = kbc_handler();
							option_handler();
						}
						break;
					default: break;
				}
			}
	}
		kscancode = 0;

	}

void option_handler(){
	if(kscancode == DOWNMAKE)
		if(optionsOption == 0)
			optionsOption = 1;
		else
			optionsOption = 0;
	if(kscancode == UPMAKE)
		if(optionsOption == 0)
			optionsOption = 1;
		else
			optionsOption = 0;

	if(kscancode == RIGHTMAKE){
		switch(optionsOption){
			case 0:
				if(optionsDifi == 1){
					optionsDifi = 2;
					break;}
				if(optionsDifi == 2){
					optionsDifi = 3;
					break;}
				if(optionsDifi == 3){
					optionsDifi = 1;
					break;}

			case 1:
				if(optionsSound == 1)
					optionsSound = 0;
				else
					optionsSound = 1;
			break;
			}
			}

	if(kscancode == LEFTMAKE){
	switch(optionsOption){
		case 0:
			if(optionsDifi == 1){
				optionsDifi = 3;
				break;}
			if(optionsDifi == 2){
				optionsDifi = 1;
				break;}
			if(optionsDifi == 3){
				optionsDifi = 2;
				break;}
		break;

		case 1:
			if(optionsSound == 1)
				optionsSound = 0;
			else
				optionsSound = 1;
		break;
		}
		}


	draw_options();
		}

void draw_options()
{
	if(optionsOption == 0)
	{
	vg_draw_sprite(options_buttons[0]);
	vg_draw_sprite(options_buttons[9]);
	}

	if(optionsOption == 1)
	{
	vg_draw_sprite(options_buttons[1]);
	vg_draw_sprite(options_buttons[8]);
	}

	switch(optionsDifi){
		case 1:
			vg_draw_sprite(options_buttons[2]);
			vg_draw_sprite(options_buttons[5]);
			vg_draw_sprite(options_buttons[7]);
		break;

		case 2:
			vg_draw_sprite(options_buttons[3]);
			vg_draw_sprite(options_buttons[4]);
			vg_draw_sprite(options_buttons[7]);
		break;

		case 3:
			vg_draw_sprite(options_buttons[3]);
			vg_draw_sprite(options_buttons[5]);
			vg_draw_sprite(options_buttons[6]);
		break;
		}

	switch(optionsSound){
		case 1:
			vg_draw_sprite(options_buttons[10]);
		break;

		case 0:
			vg_draw_rec(412, 375, 650, 519, 0x000000);
			vg_draw_sprite(options_buttons[11]);
		break;
		}
}


void draw_highscores(){
	int ipc_status;
	message msg;
	int r;
	int i;

	vg_fill(0x000000);

	// LE DRAW //
	highscore HIGHSCORES[10];
	FILE *fp;

	fp = fopen("/usr/lcom1213-t5g1/defense_of_the_future/highscores.txt", "r");

	for(i=0; i<10; i++){
		fgets(HIGHSCORES[i].name, 5, fp);
		fgets(HIGHSCORES[i].score, 6, fp);
		fgets(HIGHSCORES[i].day, 4, fp);
		fgets(HIGHSCORES[i].month, 4, fp);
		fgets(HIGHSCORES[i].year, 3, fp);

		draw_strings(HIGHSCORES[i].name, 3, 223, 89+(60*i));
		draw_strings(HIGHSCORES[i].score, 4, 359, 89+(60*i));
		draw_strings(HIGHSCORES[i].day, 3, 529, 89+(60*i));
		draw_strings(HIGHSCORES[i].month, 3, 631, 89+(60*i));
		draw_strings(HIGHSCORES[i].year, 3, 733, 89+(60*i));
	}

	while(kscancode != ENTERBREAK){
		r = driver_receive(ANY, &msg, &ipc_status);
		if( r != 0 ){
			printf("driver_receive failed with %d\n", r);
			continue;
		}

		if(is_ipc_notify(ipc_status)){

			switch(_ENDPOINT_P(msg.m_source)){
				case HARDWARE:
					if((msg.NOTIFY_ARG & KBC_BIT_MASK)){
						kscancode = kbc_handler();
					}
					break;
				default: break;
			}
		}
	}
	kscancode = 0;
	fclose(fp);
}
void draw_help(){

	int ipc_status;
	message msg;
	int r;

	vg_fill(0x000000);


	help = (Sprite **)malloc(19 * sizeof(Sprite));
	help[0] = create_sprite(help1, 423, 17);
	help[1] = create_sprite(help2, 212, 93);
	help[2] = create_sprite(help3, 732, 93);
	help[3] = create_sprite(help4, 100, 141);
	help[4] = create_sprite(help5_1, 200, 191);
	help[5] = create_sprite(help5_2, 200, 234);
	help[6] = create_sprite(help5_3, 200, 277);
	help[7] = create_sprite(help6, 612, 141);
	help[8] = create_sprite(help7_1, 662, 143);
	help[9] = create_sprite(help7_2, 662, 184);
	help[10] = create_sprite(help7_3, 662, 225);
	help[11] = create_sprite(help7_4, 662, 266);
	help[12] = create_sprite(help8, 100, 373);
	help[13] = create_sprite(help9, 200, 373);
	help[14] = create_sprite(help10, 549, 373);
	help[15] = create_sprite(help11, 662, 373);
	help[16] = create_sprite(help12, 581, 415);
	help[17] = create_sprite(help13_1, 662, 415);
	help[18] = create_sprite(help13_2, 662, 457);


	vg_draw_sprite(help[0]);
	vg_draw_sprite(help[1]);
	vg_draw_sprite(help[2]);
	vg_draw_sprite(help[3]);
	vg_draw_sprite(help[4]);
	vg_draw_sprite(help[5]);
	vg_draw_sprite(help[6]);
	vg_draw_sprite(help[7]);
	vg_draw_sprite(help[8]);
	vg_draw_sprite(help[9]);
	vg_draw_sprite(help[10]);
	vg_draw_sprite(help[11]);
	vg_draw_sprite(help[12]);
	vg_draw_sprite(help[13]);
	vg_draw_sprite(help[14]);
	vg_draw_sprite(help[15]);
	vg_draw_sprite(help[16]);
	vg_draw_sprite(help[17]);
	vg_draw_sprite(help[18]);

		while(kscancode != ENTERBREAK){
		r = driver_receive(ANY, &msg, &ipc_status);
		if( r != 0 ){
			printf("driver_receive failed with %d\n", r);
			continue;
		}

		if(is_ipc_notify(ipc_status)){

			switch(_ENDPOINT_P(msg.m_source)){
				case HARDWARE:
					if((msg.NOTIFY_ARG & KBC_BIT_MASK)){
						kscancode = kbc_handler();
					}
					break;
				default: break;
			}
		}
}
kscancode = 0;
}
void draw_credits(){

	int ipc_status;
	message msg;
	int r;

	vg_fill(0x000000);

	/******/
	draw_strings(creditos, strlen(creditos), 20, 20);
	//shop_menu();


	/******/
		while(kscancode != ENTERBREAK){
		r = driver_receive(ANY, &msg, &ipc_status);
		if( r != 0 ){
			printf("driver_receive failed with %d\n", r);
			continue;
		}

		if(is_ipc_notify(ipc_status)){

			switch(_ENDPOINT_P(msg.m_source)){
				case HARDWARE:
					if((msg.NOTIFY_ARG & KBC_BIT_MASK)){
						kscancode = kbc_handler();
					}
					break;
				default: break;
			}
		}
	}
	kscancode = 0;
}



void draw_strings(char *string, int n, int xi,int yi){
	int i = 0;
	int xaux = xi;
	int yaux = yi;
	char letra;
	Sprite *x;

	for(i=0; i<n; i++){

		if( (yaux + 50) >= 768)
				break;

		if( (xaux + 34) >= 1024){
			xaux = xi;
			yaux += 50;
		} else {
			letra = string[i];

			switch(letra){
				case 'A':
				case 'a':
					x = create_sprite(aS, xaux, yaux);
					vg_draw_sprite(x);
					xaux += 34;
					destroy_sprite(x);
				break;

				case 'B':
				case 'b':
					x = create_sprite(bS, xaux, yaux);
					vg_draw_sprite(x);
					xaux += 34;
					destroy_sprite(x);
				break;

				case 'C':
				case 'c':
					x = create_sprite(cS, xaux, yaux);
					vg_draw_sprite(x);
					xaux += 34;
					destroy_sprite(x);
				break;

				case 'D':
				case 'd':
					x = create_sprite(dS, xaux, yaux);
					vg_draw_sprite(x);
					xaux += 34;
					destroy_sprite(x);
				break;

				case 'E':
				case 'e':
					x = create_sprite(eS, xaux, yaux);
					vg_draw_sprite(x);
					xaux += 34;
					destroy_sprite(x);
				break;

				case 'F':
				case 'f':
					x = create_sprite(fS, xaux, yaux);
					vg_draw_sprite(x);
					xaux += 34;
					destroy_sprite(x);
				break;

				case 'G':
				case 'g':
					x = create_sprite(gS, xaux, yaux);
					vg_draw_sprite(x);
					xaux += 34;
					destroy_sprite(x);
				break;

				case 'H':
				case 'h':
					x = create_sprite(hS, xaux, yaux);
					vg_draw_sprite(x);
					xaux += 34;
					destroy_sprite(x);
				break;

				case 'I':
				case 'i':
					x = create_sprite(iS, xaux, yaux);
					vg_draw_sprite(x);
					xaux += 34;
					destroy_sprite(x);
				break;

				case 'J':
				case 'j':
					x = create_sprite(jS, xaux, yaux);
					vg_draw_sprite(x);
					xaux += 34;
					destroy_sprite(x);
				break;

				case 'K':
				case 'k':
					x = create_sprite(kS, xaux, yaux);
					vg_draw_sprite(x);
					xaux += 34;
					destroy_sprite(x);
				break;

				case 'L':
				case 'l':
					x = create_sprite(lS, xaux, yaux);
					vg_draw_sprite(x);
					xaux += 34;
					destroy_sprite(x);
				break;

				case 'M':
				case 'm':
					x = create_sprite(mS, xaux, yaux);
					vg_draw_sprite(x);
					xaux += 34;
					destroy_sprite(x);
				break;

				case 'N':
				case 'n':
					x = create_sprite(nS, xaux, yaux);
					vg_draw_sprite(x);
					xaux += 34;
					destroy_sprite(x);
				break;

				case 'O':
				case 'o':
					x = create_sprite(oS, xaux, yaux);
					vg_draw_sprite(x);
					xaux += 34;
					destroy_sprite(x);
				break;

				case 'P':
				case 'p':
					x = create_sprite(pS, xaux, yaux);
					vg_draw_sprite(x);
					xaux += 34;
					destroy_sprite(x);
				break;

				case 'Q':
				case 'q':
					x = create_sprite(qS, xaux, yaux);
					vg_draw_sprite(x);
					xaux += 34;
					destroy_sprite(x);
				break;

				case 'R':
				case 'r':
					x = create_sprite(rS, xaux, yaux);
					vg_draw_sprite(x);
					xaux += 34;
					destroy_sprite(x);
				break;

				case 'S':
				case 's':
					x = create_sprite(sS, xaux, yaux);
					vg_draw_sprite(x);
					xaux += 34;
					destroy_sprite(x);
				break;

				case 'T':
				case 't':
					x = create_sprite(tS, xaux, yaux);
					vg_draw_sprite(x);
					xaux += 34;
					destroy_sprite(x);
				break;

				case 'U':
				case 'u':
					x = create_sprite(uS, xaux, yaux);
					vg_draw_sprite(x);
					xaux += 34;
					destroy_sprite(x);
				break;

				case 'V':
				case 'v':
					x = create_sprite(vS, xaux, yaux);
					vg_draw_sprite(x);
					xaux += 34;
					destroy_sprite(x);
				break;

				case 'W':
				case 'w':
					x = create_sprite(wS, xaux, yaux);
					vg_draw_sprite(x);
					xaux += 34;
					destroy_sprite(x);
				break;

				case 'X':
				case 'x':
					x = create_sprite(xS, xaux, yaux);
					vg_draw_sprite(x);
					xaux += 34;
					destroy_sprite(x);
				break;

				case 'Y':
				case 'y':
					x = create_sprite(yS, xaux, yaux);
					vg_draw_sprite(x);
					xaux += 34;
					destroy_sprite(x);
				break;

				case 'Z':
				case 'z':
					x = create_sprite(zS, xaux, yaux);
					vg_draw_sprite(x);
					xaux += 34;
					destroy_sprite(x);
				break;

				case '0':
					x = create_sprite(alg0, xaux, yaux);
					vg_draw_sprite(x);
					xaux += 34;
					destroy_sprite(x);
				break;

				case '1':
					x = create_sprite(alg1, xaux, yaux);
					vg_draw_sprite(x);
					xaux += 34;
					destroy_sprite(x);
				break;

				case '2':
					x = create_sprite(alg2, xaux, yaux);
					vg_draw_sprite(x);
					xaux += 34;
					destroy_sprite(x);
				break;

				case '3':
					x = create_sprite(alg3, xaux, yaux);
					vg_draw_sprite(x);
					xaux += 34;
					destroy_sprite(x);
				break;

				case '4':
					x = create_sprite(alg4, xaux, yaux);
					vg_draw_sprite(x);
					xaux += 34;
					destroy_sprite(x);
				break;

				case '5':
					x = create_sprite(alg5, xaux, yaux);
					vg_draw_sprite(x);
					xaux += 34;
					destroy_sprite(x);
				break;

				case '6':
					x = create_sprite(alg6, xaux, yaux);
					vg_draw_sprite(x);
					xaux += 34;
					destroy_sprite(x);
				break;

				case '7':
					x = create_sprite(alg7, xaux, yaux);
					vg_draw_sprite(x);
					xaux += 34;
					destroy_sprite(x);
				break;

				case '8':
					x = create_sprite(alg8, xaux, yaux);
					vg_draw_sprite(x);
					xaux += 34;
					destroy_sprite(x);
				break;

				case '9':
					x = create_sprite(alg9, xaux, yaux);
					vg_draw_sprite(x);
					xaux += 34;
					destroy_sprite(x);
				break;

				case ' ':
					xaux +=34;
				break;

				case '/':
					x = create_sprite(bar, xaux, yaux);
					vg_draw_sprite(x);
					xaux += 34;
					destroy_sprite(x);
				break;

				default: xaux +=34; break;
			}
		}
	}
}
void shop_menu()
{
	int ipc_status;
	message msg;
	int r;
	lifebuy = 1;

	if(life < 8)
		lifebuy = 1;

	vg_fill(0x000000);
	draw_shop();

	while(kscancode != ENTERBREAK){
	r = driver_receive(ANY, &msg, &ipc_status);
	if( r != 0 ){
		printf("driver_receive failed with %d\n", r);
		continue;
	}

	if(is_ipc_notify(ipc_status)){

		switch(_ENDPOINT_P(msg.m_source)){
			case HARDWARE:
				if((msg.NOTIFY_ARG & KBC_BIT_MASK)){
					kscancode = kbc_handler();
					shop_handler();
				}
				break;
			default: break;
		}
	}

	}
	kscancode = 0;
}

void shop_handler()
{
	vg_draw_rec(200, 550, 800, 592, 0x000000);

	switch(kscancode){
		case NO1MAKE:
			if(cash.value >= 1000){
				if(lifebuy == 1){
					lifebuy = 0;
					life = 8;
					cash.value -= 1000;
				}
				else
					draw_strings(alreadyo, strlen(alreadyo), 320, 550);

			}
			else
				draw_strings(nomoney, strlen(nomoney), 256, 550);
			break;

		case NO2MAKE:
			if(guns[1] == 0){
				if(cash.value >= 3000){
					guns[1] = 1;
					cash.value -= 3000;
				}
				else
					draw_strings(nomoney, strlen(nomoney), 256, 550);
			}
			else
				draw_strings(alreadyo, strlen(alreadyo), 320, 550);
			break;

		case NO3MAKE:
			if(guns[2] == 0){
				if(cash.value >= 5000){
					guns[2] = 1;
					cash.value -= 5000;
				}
				else
					draw_strings(nomoney, strlen(nomoney), 256, 550);
			}
			else
				draw_strings(alreadyo, strlen(alreadyo), 320, 550);
			break;

		case NO4MAKE:
			if(guns[3] == 0){
				if(cash.value >= 9500){
					guns[3] = 1;
					cash.value -= 9500;
				}
				else
					draw_strings(nomoney, strlen(nomoney), 256, 550);
			}
			else
				draw_strings(alreadyo, strlen(alreadyo), 320, 550);
			break;


	}


	draw_shop();
}
//por alguma razao, o 1º algarismo dentro do jogo, com cash.value < 10000, é um 2
void draw_shop(){
	/* dinheiro por inimigo
	 * lvl1 = 15 pequenos 500$ cada
	 * lvl2 = 4 pequenos 500$ cada, boss 1 2000$
	 * lvl3 = boss 2 5000$
	 * ou seja, se compra a gun2 e gun3, nao compra a 4
	 */

	int money = cash.value;
	int i = 4;
	char string[5]={"00000"};
	Sprite *dolar;
	Sprite **shop;
	shop = (Sprite **)malloc(8 * sizeof(Sprite));

	while (i >= 0){
		string[i] = ((char)(48+(money % 10)));
	    money = money / 10;
	    i--;
	}

	draw_strings(string, 5, 416, 600);
	dolar = create_sprite(dolarS, 600, 600);
	vg_draw_sprite(dolar);

	draw_strings("Shop", 4, 430, 50);

	draw_strings("1000", 4, 15, 320);
	draw_strings("3000", 4, 289, 320);
	draw_strings("5000", 4, 550, 320);
	draw_strings("9500", 4, 810, 320);


	if(lifebuy == 1)
		{
			shop[0]= create_sprite(lbarg, 60, 250);
			vg_draw_sprite(shop[0]);
			shop[7]= create_sprite(hp, 71, 268);
			vg_draw_sprite(shop[7]);
		}
	else{
			vg_draw_rec(60, 250, 121, 300, 0x000000);
	 	}

	if(guns[1] == 0)
		{
			shop[1]= create_sprite(g2lock, 316, 250);
			vg_draw_sprite(shop[1]);
		}
		else{
			shop[2]= create_sprite(g2off, 316, 250);
			vg_draw_sprite(shop[2]);
			}

	if(guns[2] == 0)
		{
			shop[3]= create_sprite(g3lock, 572, 250);
			vg_draw_sprite(shop[3]);
		}
		else{
			shop[4]= create_sprite(g3off, 572, 250);
			vg_draw_sprite(shop[4]);
			}

	if(guns[3] == 0)
		{
			shop[5]= create_sprite(g4lock, 828, 250);
			vg_draw_sprite(shop[5]);
		}
		else{
			shop[6]= create_sprite(g4off, 828, 250);
			vg_draw_sprite(shop[6]);
			}

}
