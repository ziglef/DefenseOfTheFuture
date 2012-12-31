#include "rtc.h"

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

	rtc_read(RTC_REGC, &REG_C);

	if((REG_C & RTC_UF)){
		rtc_read(RTC_SECONDS, &(DATE.seconds));
		rtc_read(RTC_MINUTES, &(DATE.minutes));
		rtc_read(RTC_HOURS, &(DATE.hours));
		rtc_read(RTC_DOW, &(DATE.dayOfWeek));
		rtc_read(RTC_DOM, &(DATE.dayOfMonth));
		rtc_read(RTC_MONTH, &(DATE.month));
		rtc_read(RTC_YEAR, &(DATE.year));

		DATE.seconds = BCD_to_DEC(DATE.seconds);
		DATE.minutes = BCD_to_DEC(DATE.minutes);
		DATE.hours = BCD_to_DEC(DATE.hours);
		DATE.dayOfWeek = BCD_to_DEC(DATE.dayOfWeek);
		DATE.dayOfMonth = BCD_to_DEC(DATE.dayOfMonth);
		DATE.month = BCD_to_DEC(DATE.month);
		DATE.year = BCD_to_DEC(DATE.year);

		//print_date(DATE);
		return 1;
	}

	if((REG_C & RTC_AF)){
		return 2;
	}

	return 0;
}

void print_date(DATE_STRUCT DATE){

	char dow[32] = "";
	char ext[32] = "";
	char month[32] = "";

	DATE.seconds = BCD_to_DEC(DATE.seconds);
	DATE.minutes = BCD_to_DEC(DATE.minutes);
	DATE.hours = BCD_to_DEC(DATE.hours);
	DATE.dayOfWeek = BCD_to_DEC(DATE.dayOfWeek);
	DATE.dayOfMonth = BCD_to_DEC(DATE.dayOfMonth);
	DATE.month = BCD_to_DEC(DATE.month);
	DATE.year = BCD_to_DEC(DATE.year);

	switch(DATE.dayOfWeek){
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

	switch(DATE.dayOfMonth){
		case 1:
			strcpy(ext, "st");
			break;
		case 2:
			strcpy(ext, "nd");
			break;
		case 3:
			strcpy(ext, "rd");
			break;
		case 21:
			strcpy(ext, "st");
			break;
		case 22:
			strcpy(ext, "nd");
			break;
		case 23:
			strcpy(ext, "rd");
			break;
		case 31:
			strcpy(ext, "st");
			break;
		default:
			strcpy(ext, "th");
			break;
	}

	switch(DATE.month){
		case 1:
			strcpy(month, "January");
			break;
		case 2:
			strcpy(month, "February");
			break;
		case 3:
			strcpy(month, "March");
			break;
		case 4:
			strcpy(month, "April");
			break;
		case 5:
			strcpy(month, "May");
			break;
		case 6:
			strcpy(month, "June");
			break;
		case 7:
			strcpy(month, "July");
			break;
		case 8:
			strcpy(month, "August");
			break;
		case 9:
			strcpy(month, "September");
			break;
		case 10:
			strcpy(month, "October");
			break;
		case 11:
			strcpy(month, "November");
			break;
		case 12:
			strcpy(month, "December");
			break;
		default:
			strcpy(month, "ERROR");
			break;
	}

	printf("Today is %s, the %d%s of %s of 20%d.\nIt's %d:%d:%d.", dow, DATE.dayOfMonth, ext, month, DATE.year, DATE.hours, DATE.minutes, DATE.seconds);
}

unsigned long BCD_to_DEC(unsigned long bcdByte){
	return (((bcdByte & 0xF0) >> 4) * 10) + (bcdByte & 0x0F);
}

unsigned long DEC_to_BCD(unsigned long decimalByte){
	return (((decimalByte / 10) << 4) | (decimalByte % 10));
}
