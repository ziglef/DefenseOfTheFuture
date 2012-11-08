#include "rtc.h"

int test_conf(void) {
	unsigned long regA, regB, regC, regD;

	rtc_read(RTC_REGA,&regA);
	rtc_read(RTC_REGB,&regB);
	rtc_read(RTC_REGC,&regC);
	rtc_read(RTC_REGD,&regD);

	printf("REGISTER A: 0x%X\nREGISTER B: 0x%X\nREGISTER C: 0x%X\nREGISTER D: 0x%X", regA, regB, regC, regD);
	return 0;
}

int test_date(void) {
	int ipc_status;
		message msg;
		int r;

		if(rtc_subscribe() < 0){
			printf("ERROR SUBSCRIBING/ENABLING INTERRUPTS ON RTC\n");
			return 1;
		}

		while(1){
			r = driver_receive(ANY, &msg, &ipc_status);
			if( r != 0){
				printf("driver_receive failed with: %d\n", r);
				continue;
			}
			if(is_ipc_notify(ipc_status)){
				switch(_ENDPOINT_P(msg.m_source)){
					case HARDWARE:
						if(msg.NOTIFY_ARG & RTC_BIT_MASK){
							rtc_handler();
						}
						break;
					default:
						break;
				}
			}
		}

		if(rtc_unsubscribe())
			return 1;
		else
			return 0;
}

int test_int(/* to be defined in class */) {
	/* To be completed */
}
