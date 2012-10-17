// System Libraries
#include <minix/syslib.h>
#include <minix/drivers.h>
#include <minix/sysutil.h>

#include "i8042.h"
#include "mouse.h"

MouseController mouse = {0x0C,0,0,{0,0,0}};

int mouse_subscribe_exclusive(void) {

	if(sys_irqsetpolicy(M_IRQ, (IRQ_REENABLE|IRQ_EXCLUSIVE), &(mouse.hook_id)) != OK){
		printf("ERROR SETTING POLICY!\n");
		return -1;
	}
	if(sys_irqenable(&(mouse.hook_id)) != OK){
		printf("ERROR ENABLING SUBSCRIPTION!\n");
		return -1;
	}

	return 0;
}

int mouse_unsubscribe() {

	if(sys_irqdisable(&(mouse.hook_id)) != OK){
		printf("ERROR DISABLING SUBSCRIPTION!\n");
		return 1;
	}

	if(sys_irqrmpolicy(&(mouse.hook_id)) != OK){
		printf("ERROR DISABLING POLICY\n");
		return 1;
	}

	return 0;
}

int mouse_handler() {
	mouse_read();


}

int mouse_read(){
	int counter = 0;

	while(counter < NO_OF_TRIES){
		if(sys_inb(KBC_STAT, &(mouse.status)) != OK){
			printf("ERROR GETTING KBC_STATUS INFORMATION!\n");
			return -1;
		}

		if( (mouse.status & KBC_STAT_OBF)){ 										// If Output Buffer is full...
			if(sys_inb(KBC_O_BUF, &(mouse.data)) != OK){							// Read the data
				printf("ERROR READING KBC_O_BUF!\n");
				return -1;
			} else {
				if( (mouse.status & (KBC_STAT_TIMEOUT | KBC_STAT_PARITY)) == 0){
					mousecounter++;
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

int kbc_send(unsigned long port, unsigned char cmd){
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
