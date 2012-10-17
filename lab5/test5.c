#include <minix/syslib.h>
#include <minix/drivers.h>
#include <minix/sysutil.h>

#include "i8042.h"
#include "mouse.h"

unsigned short lemousecounter = 0;
MouseController lemouse = {0x0C,0,0,0,0,{0,0,0}};
unsigned char *packet;


int test_packet() {
	return_vars(&lemousecounter, &lemouse, packet);
	int ipc_status;
	message msg;
	int r;

	if(mouse_subscribe_exclusive() < 0){
			printf("ERROR SUBSCRIBING TO KBC!\n");
			return 1;
	}

	turn_mouse_on();

	while(lemousecounter < 30){
		r = driver_receive(ANY, &msg, &ipc_status);
		if( r != 0 ){
			printf("driver_receive failed with %d\n", r);
			continue;
		}

		if(is_ipc_notify(ipc_status)){
			switch(_ENDPOINT_P(msg.m_source)){
				case HARDWARE:
					if((msg.NOTIFY_ARG /*& M_IRQ*/)){
						mouse_handler();
						printf("MOUSE INFO: 0x%X\n", lemouse.data);
					} break;
				default: break;
			}
		}
		if(lemouse.initialized)
			lemousecounter++;
	}

	if(mouse_unsubscribe())
			return 1;
		else
			return 0;
}

int test_asynch(unsigned short duration) {
    /* To be completed ... */
}
int test_config(void) {
    /* To be completed ... */
}
