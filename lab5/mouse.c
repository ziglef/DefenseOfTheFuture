// System Libraries
#include <minix/syslib.h>
#include <minix/drivers.h>
#include <minix/sysutil.h>

#include "i8042.h"
#include "mouse.h"

unsigned short mousecounter = 0;
MouseController mouse = {0x0C,0,0,0,0,{0,0,0}};

void return_vars(unsigned short *counter, MouseController *mmouse, unsigned char *packet){
	counter = &mousecounter;
	mmouse = &mouse;
	packet = mouse.bytes;
}

int mouse_subscribe_exclusive() {

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

	if(!mouse.initialized){
		if(mouse.data & BIT(3)){
			mouse.pos = 0;
			mouse.bytes[mouse.pos] = mouse.data;
			mouse.pos++;
			mouse.initialized = 1;
			printf("0x%X", mouse.data);
			return 0;
		}
	} else {
		mouse.bytes[mouse.pos] = mouse.data;
		if(mouse.pos == 2)
			mouse.pos = 0;
		else
			mouse.pos++;

		printf("0x%X", mouse.data);
		return 0;
	}

	return -1;
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

int mouse_send(unsigned long port, unsigned char cmd){
	int counter = 0;

	while(counter < NO_OF_TRIES){
		if(sys_inb(KBC_STAT, &(mouse.status)) != OK){
			printf("ERROR GETTING KBC_STATUS INFORMATION!\n");
			return 1;
		}

		if( (mouse.status & KBC_STAT_IBF) == 0){ 		// If Input Buffer not full...
			sys_outb(port, cmd); 					// Issue a command
			return 0;
		}

		tickdelay(micros_to_ticks(DELAY_US)); 		// Wait the appropriate time
		counter++;
	}
	return -1;
}

void turn_mouse_on(){

	mouse_send(0x64,0xA8);
	mouse_send(0x64,0xD4);
	mouse_send(0x60,0xF4);

}
