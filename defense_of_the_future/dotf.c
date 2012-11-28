#include "dotf.h"

void start_game();

int main(){

	sef_startup();

	subscribe();

	start_game();

	//vg_exit();
}

void start_game(){

	int i;
	unsigned char keyPressed;

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


	while(keyPressed != WMAKE){

	}



	sleep(5);
	kbc_unsubscribe();
	vg_exit();
}

void subscribe(){
	kbc_subscribe_exclusive();
}


KeyBoardController KBCs = {0, 0, 0};
unsigned char kscancode = 0;

int test_scan(void) {
	int ipc_status;
	message msg;
	int r;

	if(kbc_subscribe_exclusive() < 0){
			printf("ERROR SUBSCRIBING TO KBC!\n");
			return 1;
	}

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
	}

	if(kbc_unsubscribe())
			return 1;
		else
			return 0;
}
