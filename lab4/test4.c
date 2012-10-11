#include "lab4.h"

int test_scan(void) {
	int ipc_status;
	message msg;
	int r;

	if(kbc_subscribe_exclusive() < 0){
			printf("ERROR SUBSCRIBING TO KBC!\n");
			return 1;
	}

	while(scancode != ESC_BREAKCODE){

		r = driver_receive(ANY, &msg, &ipc_status);
		if( r != 0 ){
			printf("driver_receive failed with %d\n", r);
			continue;
		}

		if(is_ipc_notify(ipc_status)){

			switch(_ENDPOINT_P(msg.m_source)){
				case HARDWARE:
					if((msg.NOTIFY_ARG & KBC_IRQ)){
						kbc_handler();
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

int test_leds(unsigned short n, unsigned short *leds) {

	int r, i;
	int ScrollLock = 0;
	int NumLock = 0;
	int CapsLock = 0;
	unsigned long LEDS;

	for(i=0; i<n; i++){
		kbc_send_command(KBD_PORT, KBD_LEDS);
		sys_inb(KBD_PORT, &(KBC.data));

		if(KBC.data == CKBD_ERROR){
			printf("ERROR IN THE SEQUENCE\n");
			i = 0;
		} else if(KBC.data == CKBD_RESEND) {
			printf("ERROR IN THE LAST LED VALUE\n");
			i--;
		} else {
			switch(leds[i]){
				case 0:
					if(ScrollLock == 0){
						ScrollLock = BIT(0);
						printf("SCROLL LOCK ON!\n");
					} else {
						ScrollLock = 0;
						printf("SCROLL LOCK OFF!\n");
					}
					break;
				case 1:
					if(NumLock == 0){
						NumLock = BIT(1);
						printf("NUM LOCK ON!\n");
					} else {
						NumLock = 0;
						printf("NUM LOCK OFF!\n");
					}
					break;
				case 2:
					if(CapsLock == 0){
						CapsLock = BIT(2);
						printf("CAPS LOCK ON!\n");
					} else {
						CapsLock = 0;
						printf("CAPS LOCK OFF!\n");
					}
					break;
				default: printf("ERROR IN POSITION %d VALUE OUT OF BOUNDS!\n",i); break;
			}

			LEDS = (ScrollLock | NumLock | CapsLock);
			kbc_send_command(KBD_PORT, LEDS);
			sys_inb(KBD_PORT, &(KBC.data));

			if(KBC.data == CKBD_ERROR){
				printf("ERROR IN THE SEQUENCE\n");
				i = 0;
			} else if(KBC.data == CKBD_RESEND) {
				printf("ERROR IN THE LAST LED VALUE\n");
				i--;
			} else {
				timer_test_int(1);
			}
		}
	}
}
