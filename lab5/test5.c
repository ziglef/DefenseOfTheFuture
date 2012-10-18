#include <minix/syslib.h>
#include <minix/drivers.h>
#include <minix/sysutil.h>

#include "i8042.h"
#include "i8254.h"
#include "mouse.h"
#include "timer.h"

MouseController lemouse = {0x0C,0,0,0,0,0,{0,0,0}};
unsigned short counter;
unsigned char packet[3];

unsigned long byte;

int test_packet() {
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
		if(lemouse.status & KBC_STAT_OBF)
			sys_inb(KBC_O_BUF, &byte);
	}while(lemouse.status & KBC_STAT_OBF);

	while(counter < 30){
		r = driver_receive(ANY, &msg, &ipc_status);
		if( r != 0 ){
			printf("driver_receive failed with %d\n", r);
			continue;
		}
		if(is_ipc_notify(ipc_status)){
			switch(_ENDPOINT_P(msg.m_source)){
				case HARDWARE:
					if((msg.NOTIFY_ARG & BIT(M_IRQ))){
						lemouse = mouse_handler();
						packet[0] = lemouse.bytes[0];
						packet[1] = lemouse.bytes[1];
						packet[2] = lemouse.bytes[2];
						counter = lemouse.counter;
					} break;
				default: break;
			}
		}
	}

	if(mouse_unsubscribe())
		return 1;

	do{
		sys_inb(KBC_STAT, &(lemouse.status));
		if(lemouse.status & KBC_STAT_OBF)
			sys_inb(KBC_O_BUF, &byte);
	}while(lemouse.status & KBC_STAT_OBF);

	return 0;
}

int test_asynch(unsigned short duration) {
		int ipc_status;
		message msg;
		int r;
		int timer_counter = 0;

		if(mouse_subscribe_exclusive() < 0){
				printf("ERROR SUBSCRIBING TO KBC!\n");
				return 1;
		}

		if(timer_subscribe_int() < 0){
			printf("ERROR SUBSCRIBING/ENABLING INTERRUPTS ON TIMER 0\n");
			return 1;
		}

		turn_mouse_on();

		do{
			sys_inb(KBC_STAT, &(lemouse.status));
			if(lemouse.status & KBC_STAT_OBF)
				sys_inb(KBC_O_BUF, &byte);
		}while(lemouse.status & KBC_STAT_OBF);

		while((timer_counter / 60) < duration){
			r = driver_receive(ANY, &msg, &ipc_status);
			if( r != 0 ){
				printf("driver_receive failed with %d\n", r);
				continue;
			}
			if(is_ipc_notify(ipc_status)){
				switch(_ENDPOINT_P(msg.m_source)){
					case HARDWARE:
						if((msg.NOTIFY_ARG & BIT(M_IRQ))){
							lemouse = mouse_handler();
							packet[0] = lemouse.bytes[0];
							packet[1] = lemouse.bytes[1];
							packet[2] = lemouse.bytes[2];
							counter = lemouse.counter;
						} else {
							if((msg.NOTIFY_ARG & TIMER_BIT_MASK)){
								timer_counter++;
							}
						}break;
					default: break;
				}
			}
		}

		if(mouse_unsubscribe())
			return 1;
		if(timer_unsubscribe_int())
			return 1;

		do{
			sys_inb(KBC_STAT, &(lemouse.status));
			if(lemouse.status & KBC_STAT_OBF)
				sys_inb(KBC_O_BUF, &byte);
		}while(lemouse.status & KBC_STAT_OBF);

		return 0;
}

int test_config(void) {
    /* To be completed ... */
}
