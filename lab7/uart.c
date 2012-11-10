#include "uart.h"

void uart_read(unsigned short base_addr, unsigned short OFFSET, unsigned long *byte){
	sys_inb(base_addr+OFFSET, byte);
}

void uart_write(unsigned short base_addr, unsigned short OFFSET, unsigned long byte){
	sys_outb(base_addr+OFFSET, byte);
}
