// System Libraries
#include <minix/syslib.h>
#include <minix/drivers.h>
#include <minix/sysutil.h>

// User Libraries
#include "i8042.h"

KeyBoardController KBC = {KBC_BIT, 0, 0};
unsigned char scancode = 0;

int kbc_subscribe_exclusive(void) {

	int *HOOK_ID;
	HOOK_ID = (int*)malloc(sizeof(int));
	*HOOK_ID = 8;

	if(sys_irqsetpolicy(KBC_IRQ, (IRQ_REENABLE|IRQ_EXCLUSIVE), HOOK_ID) != OK){
		printf("ERROR SETTING POLICY!\n");
		return -1;
	}
	if(sys_irqenable(&(KBC.hook_id)) != OK){
		printf("ERROR ENABLING SUBSCRIPTION!\n");
		return -1;
	}

	KBC.hook_id = *HOOK_ID;
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
					printf("GOT DATA!\n");
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

		//printf("I'm here\n");

		if(is_ipc_notify(ipc_status)){

			switch(_ENDPOINT_P(msg.m_source)){
				case HARDWARE:
					printf("message: %d\n", msg.NOTIFY_ARG);
					if((msg.NOTIFY_ARG & KBC_IRQ)){
						printf("or who knows, HERE!\n");
						kbc_handler();
						printf("HALLO");
					} break;
				default: break;
			}
		}
	}

	printf("but not here\n");
	if(kbc_unsubscribe())
			return 1;
		else
			return 0;
}

int test_leds(unsigned short n, unsigned short *leds) {
	int i;
	unsigned long var=1;
	for(i=0; i<n; i++){

	}
}
