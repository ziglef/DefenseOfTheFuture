#include <minix/syslib.h>
#include <minix/drivers.h>
#include <minix/sysutil.h>

#include "rtc.h"

typedef struct{
	int hook_id;
}RTC_STRUCT;

typedef struct{
	unsigned long seconds;
	unsigned long minutes;
	unsigned long hours;
	unsigned long dayOfWeek;
	unsigned long dayOfMonth;
	unsigned long month;
	unsigned long year;
}DATE_STRUCT;

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

int rtc_handler(){
	unsigned long REG_C;
	DATE_STRUCT DATE;


	rtc_read(RTC_REGC, REG_C);

	if(((REG_C & RTC_UF)&&RTC_UF)){
		rtc_read(RTC_SECONDS, DATE.seconds);
		rtc_read(RTC_MINUTES, DATE.minutes);
		rtc_read(RTC_HOURS, DATE.hours);
		rtc_read(RTC_DOW, DATE.dow);
		rtc_read(RTC_DOM, DATE.dom);
		rtc_read(RTC_MONTH, DATE.month);
		rtc_read(RTC_YEAR, DATE.year);

		print_date(DATE);
		return 1;
	}

	return 0;
}

void print_date(DATE_STRUCT DATE){

	char dow[16];
	char ext[2];

	switch(DATE.dow){
		case 1:
			strcpy(dow, "Sunday");
			break;
		case 2:
			strcpy(dow, "Monday");
			break;
		case 3:
			strcpy(dow, "Tuesday");
			break;
		case 4:
			strcpy(dow, "Wednesday");
			break;
		case 5:
			strcpy(dow, "Thursday");
			break;
		case 6:
			strcpy(dow, "Friday");
			break;
		case 7:
			strcpy(dow, "Saturday");
			break;
		default:
			strcpy(dow, "ERROR");
			break;
	}

	switch(DATE.dom){
		case 1:
			strcpy(dom, "st");
			break;
		case 2:
			strcpy(dom, "nd");
			break;
		case 3:
			strcpy(dom, "rd");
			break;
		case 21:
			strcpy(dom, "st");
			break;
		case 22:
			strcpy(dom, "nd");
			break;
		case 23:
			strcpy(dom, "rd");
			break;
		case 31:
			strcpy(dom, "st");
			break;
		default:
			strcpy(dom, "th");
			break;
	}

	printf("Today is %s, the %d%s of %d of %d.\nIt's %d:%d:%d.", dow, DATE.dom, dom, DATE.month, DATE.year, DATE.hours, DATE.minutes, DATE.seconds);
}
