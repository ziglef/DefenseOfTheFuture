#include <minix/syslib.h>
#include <minix/drivers.h>
#include <minix/sysutil.h>

#include "rtc.h"

void enable(){
	endpoint_t ep;
	char name[256];
	int priv_f;

	// FETCH OUR ENDPOINT
	sys_whoami(&ep, name, 256, &priv_f);
	// ENABLE IO-SENSITIVE OPERATIONS FOR OURSELVES
	sys_enable_iop(ep);
	asm("STI");
}

void disable(){
	endpoint_t ep;
	char name[256];
	int priv_f;

	// FETCH OUR ENDPOINT
	sys_whoami(&ep, name, 256, &priv_f);
	// ENABLE IO-SENSITIVE OPERATIONS FOR OURSELVES
	sys_enable_iop(ep);
	asm("CLI");
}

void rtc_read(unsigned long reg, unsigned long *byte){
	disable();
	sys_outb(RTC_ADDRESS_REG, reg);
	sys_inb(RTC_DATA_REG, byte);
	enable();
}

void rtc_write(unsigned long reg, unsigned long byte){
	disable();
	sys_outb(RTC_ADDRESS_REG, reg);
	sys_outb(RTC_DATA_REG, byte);
	enable();
}
