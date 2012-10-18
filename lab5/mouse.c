// System Libraries
#include <minix/syslib.h>
#include <minix/drivers.h>
#include <minix/sysutil.h>

#include "i8042.h"
#include "mouse.h"

MouseController mouse = {0x0C,0,0,0,0,0,{0,0,0}};

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

MouseController mouse_handler() {
	mouse_read();

	if(!mouse.initialized){
		if(mouse.data & BIT(3)){
			mouse.pos = 0;
			mouse.bytes[mouse.pos] = mouse.data;
			mouse.pos++;
			mouse.initialized = 1;
			mouse.counter++;
			return mouse;
		}
	} else {
		mouse.bytes[mouse.pos] = mouse.data;
		if(mouse.pos == 2){
			mouse.pos = 0;
			printf("B1=0x%X B2=0x%X B3=0x%X LB=%d MB=%d RB=%d XOV=%d YOV=%d X=%d Y=%d\n",
					mouse.bytes[0],mouse.bytes[1],mouse.bytes[2],
					mouse.bytes[0]&BIT(0),mouse.bytes[0]&BIT(2),mouse.bytes[0]&BIT(1),
					mouse.bytes[0]&BIT(6),mouse.bytes[0]&BIT(7),
					mouse.bytes[1],mouse.bytes[2]);


		}
		else
			mouse.pos++;

		mouse.counter++;
		return mouse;
	}

	return mouse;
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

void turn_mouse_off(){

	mouse_send(0x64,0xD4);
	mouse_send(0x60,0xF5);
	//mouse_send(0x64,0xA7);
}
