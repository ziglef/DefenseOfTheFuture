#include "uart.h"

int hook_id = 10;

void uart_read(unsigned short base_addr, unsigned short OFFSET, unsigned long *byte){
	sys_inb(base_addr+OFFSET, byte);
}

void uart_write(unsigned short base_addr, unsigned short OFFSET, unsigned long byte){
	sys_outb(base_addr+OFFSET, byte);
}

int uart_subscribe(unsigned long IRQ) {

	if(sys_irqsetpolicy(IRQ, (IRQ_REENABLE | IRQ_EXCLUSIVE), &hook_id) != OK){
		printf("ERROR SETTING POLICY!\n");
		return -1;
	}
	if(sys_irqenable(&hook_id) != OK){
		printf("ERROR ENABLING SUBSCRIPTION!\n");
		return -1;
	}

	return hook_id;
}

int uart_unsubscribe() {

	if(sys_irqdisable(&hook_id) != OK){
		printf("ERROR DISABLING SUBSCRIPTION!\n");
		return 1;
	}

	if(sys_irqrmpolicy(&hook_id) != OK){
		return 1;
	}
	return 0;
}
