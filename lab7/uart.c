#include "uart.h"

void uart_read(unsigned long base_addr, unsigned short OFFSET, unsigned long *byte){
	sys_inb(base_addr+OFFSET, byte);
}

void uart_write(unsigned long base_addr, unsigned short OFFSET, unsigned long byte){
	sys_outb(base_addr+OFFSET, byte);
}
