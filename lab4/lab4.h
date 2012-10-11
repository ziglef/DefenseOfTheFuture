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
int doublebyte = 0;

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

	if(doublebyte != 2){
		if(scancode == TWOBYTE_CODE)
			doublebyte = 2;
		else
			doublebyte = 0;
	} else {
		doublebyte = 1;
	}

	if(doublebyte == 0){
		if(scancode & BREAKCODE_MASK)
			printf("Breakcode: 0x%X\n", scancode);
		else
			printf("Makecode: 0x%X\n", scancode);
	} else if(doublebyte == 1) {
		if(scancode & BREAKCODE_MASK)
			printf("Breakcode: 0xE0%X\n", scancode);
		else
			printf("Makecode: 0xE0%X\n", scancode);
	}
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

