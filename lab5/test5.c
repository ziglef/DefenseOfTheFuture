#include "mouse.h"

int test_packet() {
	int ipc_status;
	message msg;
	int r;

	if(mouse_subscribe_exclusive() < 0){
			printf("ERROR SUBSCRIBING TO KBC!\n");
			return 1;
	}

	while(counter < 30){

		r = driver_receive(ANY, &msg, &ipc_status);
		if( r != 0 ){
			printf("driver_receive failed with %d\n", r);
			continue;
		}

		if(is_ipc_notify(ipc_status)){

			switch(_ENDPOINT_P(msg.m_source)){
				case HARDWARE:
					if((msg.NOTIFY_ARG & M_IRQ)){
						mouse_handler();
					} break;
				default: break;
			}
		}
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
