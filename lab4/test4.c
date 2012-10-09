#include <minix/syslib.h>
#include <minix/drivers.h>

int kbc_subscribe_exclusive(void) {

	if(sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &(timerInt.bit)) != OK){
		printf("ERROR SETTING POLICY!\n");
		return -1;
	}
	if(sys_irqenable(&(timerInt.bit)) != OK){
		printf("ERROR ENABLING SUBSCRIPTION!\n");
		return -1;
	}

	timerInt.counter = 0;
	return timerInt.bit;
}

int kbc_unsubscribe() {

	if(sys_irqrmpolicy(&(timerInt.bit)) != OK){
		return 1;
	}
	if(sys_irqdisable(&(timerInt.bit)) != OK){
		printf("ERROR DISABLING SUBSCRIPTION!\n");
		return 1;
	}

	return 0;
}

int test_scan(void) {
	if(kbc_subscribe_exclusive() < 0){
			printf("ERROR SUBSCRIBING TO KBC!\n");
			return 1;
	}

	/* CODE HERE */

	if(timer_unsubscribe_int())
			return 1;
		else
			return 0;
}

int test_leds(unsigned short n, unsigned short *leds) {
    /* To be completed */
}
