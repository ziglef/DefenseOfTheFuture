// System Libraries
#include <minix/syslib.h>
#include <minix/drivers.h>
#include <minix/sysutil.h>

// User Libraries
#include "i8042.h"
#include "i8254.h"
#include "timer.h"

KeyBoardController KBC = {0, 0, 0};
unsigned char scancode = 0;

int kbc_subscribe_exclusive(void) {

	if(sys_irqsetpolicy(KBC_IRQ, (IRQ_REENABLE|IRQ_EXCLUSIVE), &(KBC.hook_id)) != OK){
		printf("ERROR SETTING POLICY!\n");
		return -1;
	}
	if(sys_irqenable(&(KBC.hook_id)) != OK){
		printf("ERROR ENABLING SUBSCRIPTION!\n");
		return -1;
	}

	return 0;
}

int kbc_unsubscribe() {

	if(sys_irqdisable(&(KBC.hook_id)) != OK){
		printf("ERROR DISABLING SUBSCRIPTION!\n");
		return 1;
	}

	if(sys_irqrmpolicy(&(KBC.hook_id)) != OK){
		printf("ERROR DISABLING POLICY\n");
		return 1;
	}

	return 0;
}

void kbc_handler() {
	kbc_read();

	scancode = KBC.data;

	if(scancode & BREAKCODE_MASK)
		printf("Breakcode: 0x%X\n", scancode);
	else
		printf("Makecode: 0x%X\n", scancode);
}

int kbc_read(){
	int counter = 0;

	while(counter < NO_OF_TRIES){
		if(sys_inb(KBC_STAT, &(KBC.status)) != OK){
			printf("ERROR GETTING KBC_STATUS INFORMATION!\n");
			return -1;
		}

		if( (KBC.status & KBC_STAT_OBF)){ 										// If Output Buffer is full...
			if(sys_inb(KBC_O_BUF, &(KBC.data)) != OK){							// Read the data
				printf("ERROR READING KBC_O_BUF!\n");
				return -1;
			} else {
				if( (KBC.status & (KBC_STAT_TIMEOUT | KBC_STAT_PARITY)) == 0){
					return 0;
				}
				else{
					printf("PARITY OR TIMEOUT ERROR!\n");
					return -1;
				}
			}
		}
		tickdelay(micros_to_ticks(DELAY_US)); 										// Wait the appropriate time
		counter++;
	}

	return -1;
}

int kbc_send_command(unsigned long port, unsigned char cmd){
	int counter = 0;

	while(counter < NO_OF_TRIES){
		if(sys_inb(KBC_STAT, &(KBC.status)) != OK){
			printf("ERROR GETTING KBC_STATUS INFORMATION!\n");
			return 1;
		}

		if( (KBC.status & KBC_STAT_IBF) == 0){ 		// If Input Buffer not full...
			sys_outb(port, cmd); 					// Issue a command
			return 0;
		}

		tickdelay(micros_to_ticks(DELAY_US)); 		// Wait the appropriate time
		counter++;
	}
	return -1;
}

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
