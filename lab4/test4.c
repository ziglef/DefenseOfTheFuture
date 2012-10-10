// System Libraries
#include <minix/syslib.h>
#include <minix/drivers.h>
#include <minix/sysutil.h>

// User Libraries
#include "i8042.h"

#define ESC_BREAKCODE 		0x81
#define NO_OF_TRIES			500
#define BREAKCODE_MASK		0x80
#define KBC_BIT_MASK		0x02

KeyBoardController KBC = {KBC_BIT, 0, 0};
unsigned char scancode = 0;

int kbc_subscribe_exclusive(void) {

	if(sys_irqsetpolicy(KBC_IRQ, (IRQ_REENABLE | IRQ_EXCLUSIVE), &(KBC.hook_id)) != OK){
		printf("ERROR SETTING POLICY!\n");
		return -1;
	}
	if(sys_irqenable(&(KBC.hook_id)) != OK){
		printf("ERROR ENABLING SUBSCRIPTION!\n");
		return -1;
	}

	return KBC.hook_id;
}

int kbc_unsubscribe() {

	if(sys_irqrmpolicy(&(KBC.hook_id)) != OK){
		return 1;
	}
	if(sys_irqdisable(&(KBC.hook_id)) != OK){
		printf("ERROR DISABLING SUBSCRIPTION!\n");
		return 1;
	}

	return 0;
}

void kbc_handler() {
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
			return 1;
		}

		//if( (KBC.status & KBC_STAT_TIMEOUT) == 0){
			if( (KBC.status & KBC_STAT_OBF)){ 										// If Output Buffer is full...
				if(sys_inb(KBC_IO_BUF, &(KBC.data)) != OK){							// Read the data
					printf("ERROR READING KBC_IO_BUF!\n");
					return -1;
				} else {
					if( (KBC.status & (KBC_STAT_TIMEOUT | KBC_STAT_PARITY)) == 0)
						return KBC.data;
					else
						return -1;
				}
			}
		//}
		tickdelay(micros_to_ticks(DELAY_US)); 										// Wait the appropriate time
		counter++;
	}
}


int kbc_send_command(unsigned char cmd){
	int counter = 0;

	while(counter < NO_OF_TRIES){
		if(sys_inb(KBC_STAT, &(KBC.status)) != OK){
			printf("ERROR GETTING KBC_STATUS INFORMATION!\n");
			return 1;
		}

		if( (KBC.status & KBC_STAT_TIMEOUT) == 0){
			if( (KBC.status & KBC_STAT_IBF) == 0){ 	// If Input Buffer not full...
				sys_outb(KBC_CMD, cmd); 			// Issue a non argument command
				return 0;
			}
		}
		tickdelay(micros_to_ticks(DELAY_US)); 		// Wait the appropriate time
		counter++;
	}
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
		printf("I'm here\n");
		if(is_ipc_notify(ipc_status)){
			printf("i may even get here\n");
			if((msg.NOTIFY_ARG & KBC_BIT_MASK)){ printf("or who knows, HERE!\n"); kbc_read(); kbc_handler(); printf("HALLO");}
		}
	}
	printf("but not here\n");
	if(kbc_unsubscribe())
			return 1;
		else
			return 0;
}

int test_leds(unsigned short n, unsigned short *leds) {

}
