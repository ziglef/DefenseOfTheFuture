#include <minix/syslib.h>
#include <minix/drivers.h>
#include <minix/sysutil.h>

#include "i8042.h"
#include "mouse.h"

MouseController lemouse = {0x0C,0,0,0,0,0,{0,0,0}};
unsigned short counter;
unsigned char packet[3];

unsigned long byte;

int test_packet() {
	printf("1\n");
	int ipc_status;
	message msg;
	int r;

	if(mouse_subscribe_exclusive() < 0){
			printf("ERROR SUBSCRIBING TO KBC!\n");
			return 1;
	}

	turn_mouse_on();

	do{
		sys_inb(KBC_STAT, &(lemouse.status));
		sys_inb(KBC_O_BUF, &byte);
	}while(lemouse.status & KBC_STAT_OBF);

	printf("2!\n");
	while(counter < 30){
		printf("3!\n");
		//sys_inb(KBC_O_BUF, &byte);
		r = driver_receive(ANY, &msg, &ipc_status);
		if( r != 0 ){
			printf("driver_receive failed with %d\n", r);
			continue;
		}

		printf("4!\n");
		if(is_ipc_notify(ipc_status)){
			printf("5!\n");
			switch(_ENDPOINT_P(msg.m_source)){
				case HARDWARE:
					if((msg.NOTIFY_ARG /*& lemouse.hook_id*/)){
						printf("6!\n");
						lemouse = mouse_handler();
						packet[0] = lemouse.bytes[0];
						packet[1] = lemouse.bytes[1];
						packet[2] = lemouse.bytes[2];
						counter = lemouse.counter;
						printf("7?!\n");
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
