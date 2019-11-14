#include <minix/syslib.h>
#include <minix/drivers.h>
#include "i8254.h"
#include "timer.h"

typedef struct{
	int bit;
	int counter;
}Timer;

Timer timerInt = {2, 0};

int timer_set_square(unsigned long timer, unsigned long freq) {

	unsigned long finalfreq;

	finalfreq = TIMER_FREQ/freq;

	switch(timer){
		case 0:
			printf("\tTimer 0 selected \n");
			if(sys_outb(TIMER_CTRL,(TIMER_SEL0|TIMER_LSB_MSB|TIMER_SQR_WAVE|TIMER_BIN)) != OK)
			{printf("\t timer_set_square: ERROR setting timer 0 \n");return 1;}
			sys_outb(TIMER_0,((finalfreq) & 0xFF)); /* LSB */
			sys_outb(TIMER_0,((finalfreq) >> 8)); /* MSB */
			break;

		case 1:
			printf("\tTimer 1 selected \n");
			if(sys_outb(TIMER_CTRL,(TIMER_SEL1|TIMER_LSB_MSB|TIMER_SQR_WAVE|TIMER_BIN)) != OK)
			{printf("\t timer_set_square: ERROR setting timer 1 \n");return 1;}
			sys_outb(TIMER_1,((finalfreq) & 0xFF)); /* LSB */
			sys_outb(TIMER_1,((finalfreq) >> 8)); /* MSB */
			break;

		case 2:
			printf("\tTimer 2 selected \n");
			if(sys_outb(TIMER_CTRL,(TIMER_SEL2|TIMER_LSB_MSB|TIMER_SQR_WAVE|TIMER_BIN)) != OK)
			{printf("\t timer_set_square: ERROR setting timer 2 \n");return 1;}
			sys_outb(TIMER_2,((finalfreq) & 0xFF)); /* LSB */
			sys_outb(TIMER_2,((finalfreq) >> 8)); /* MSB */
			break;
		default: return 1;
	}

	return 0;
}

int timer_subscribe_int(void ) {

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

int timer_unsubscribe_int() {

	if(sys_irqdisable(&(timerInt.bit)) != OK){
		printf("ERROR DISABLING SUBSCRIPTION!\n");
		return 1;
	}

	if(sys_irqrmpolicy(&(timerInt.bit)) != OK){
		return 1;
	}
	return 0;
}

unsigned long timer_int_handler(unsigned long counter) {
	return counter+1;
}

int timer_test_square(unsigned long freq) {
	/* Get Timer 0 to work at a @freq frequency */
	
	if(timer_set_square(0, freq) != 0){
		printf("ERROR SETTING FREQUENCY ON TIMER 0\n");
		return 1;
	}

	return 0;
}

int timer_test_int(unsigned long time) {
	int ipc_status;
	message msg;
	int r;

	if(timer_subscribe_int() < 0){
		printf("ERROR SUBSCRIBING/ENABLING INTERRUPTS ON TIMER 0\n");
		return 1;
	}

	while(timerInt.counter <= (time*60)){
		r = driver_receive(ANY, &msg, &ipc_status);
		if( r != 0){
			printf("driver_receive failed with: %d\n", r);
			continue;
		}
		if(is_ipc_notify(ipc_status)){
			switch(_ENDPOINT_P(msg.m_source)){
				case HARDWARE:
					if((msg.NOTIFY_ARG & TIMER_BIT_MASK) && (timerInt.counter % 60 == 0)){
						// timer_int_handler();
					}
					break;
				default:
					break;
			}
		} else {

		}
		timerInt.counter++;
	}

	if(timer_unsubscribe_int())
		return 1;
	else
		return 0;
}
