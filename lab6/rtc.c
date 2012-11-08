#include <minix/syslib.h>
#include <minix/drivers.h>
#include <minix/sysutil.h>

#include "rtc.h"

typedef struct{
	int hook_id;
}RTC_STRUCT;

RTC_STRUCT rtc = {1};

int rtc_subscribe(void ) {

	if(sys_irqsetpolicy(RTC_IRQ, IRQ_REENABLE, &(rtc.hook_id)) != OK){
		printf("ERROR SETTING POLICY!\n");
		return -1;
	}
	if(sys_irqenable(&(rtc.hook_id)) != OK){
		printf("ERROR ENABLING SUBSCRIPTION!\n");
		return -1;
	}

	return rtc.hook_id;
}

int rtc_unsubscribe() {

	if(sys_irqdisable(&(rtc.hook_id)) != OK){
		printf("ERROR DISABLING SUBSCRIPTION!\n");
		return 1;
	}

	if(sys_irqrmpolicy(&(rtc.hook_id)) != OK){
		return 1;
	}
	return 0;
}

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
