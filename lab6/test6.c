#include "rtc.h"

int test_conf(void) {
	unsigned long regA, regB, regC, regD;

	rtc_read(RTC_REGA,&regA);
	rtc_read(RTC_REGB,&regB);
	rtc_read(RTC_REGC,&regC);
	rtc_read(RTC_REGD,&regD);

	printf("REGISTER A: 0x%X\nREGISTER B: 0x%X\nREGISTER C: 0x%X\nREGISTER D: 0x%X\n", regA, regB, regC, regD);
	printf("REG A FLAGS: UIP: %d  DV2: %d  DV1: %d  DV0: %d  RS3: %d  RS2: %d  RS1: %d  RS0: %d\n", ((regA&RTC_UIP)&&RTC_UIP), ((regA & RTC_DV2)&&RTC_DV2), ((regA & RTC_DV1)&&RTC_DV1), ((regA & RTC_DV0)&&RTC_DV0), ((regA & RTC_RS3)&&RTC_RS3), ((regA & RTC_RS2)&&RTC_RS2), ((regA & RTC_RS1)&&RTC_RS1), ((regA & RTC_RS0)&&RTC_RS0));
	printf("REG B FLAGS: SET: %d  PIE: %d  AIE: %d  UIE: %d  SQWE: %d  DM: %d  24/12: %d  DSE: %d\n", ((regB&RTC_SET)&&RTC_SET), ((regB & RTC_PIE)&&RTC_PIE), ((regB & RTC_AIE)&&RTC_AIE), ((regB & RTC_UIE)&&RTC_UIE), ((regB & RTC_SQWE)&&RTC_SQWE), ((regB & RTC_DM)&&RTC_DM), ((regB & RTC_2412)&&RTC_2412), ((regB & RTC_DSE)&&RTC_DSE));
	printf("REG C FLAGS: IRQF: %d  PF: %d  AF: %d  UF: %d\n",((regC&RTC_IRQF)&&RTC_IRQF), ((regC & RTC_PF)&&RTC_PF), ((regC & RTC_AF)&&RTC_AF), ((regC & RTC_UF)&&RTC_UF));
	printf("REG D FLAGS: VRT: %d\n", ((regD&RTC_VRT)&&RTC_VRT));

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
							if(rtc_handler()) break;
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
