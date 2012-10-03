#include <minix/syslib.h>
#include <minix/drivers.h>
#include "i8254.h"

typedef struct{
	int bit;
	int *address;
}Timer;

typedef struct{
	Timer timer;
	int counter;
}TimerInterrupter;

TimerInterrupter timerInt;

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

	if(sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, timerInt.timer.address) != OK){
		printf("ERROR SETTING POLICY!\n");
		return -1;
	}
	if(sys_irqenable(timerInt.timer.address) != OK){
		printf("ERROR ENABLING SUBSCRIPTION!\n");
		return -1;
	}

	timerInt.counter = 0;
	return timerInt.timer.bit;
}

int timer_unsubscribe_int() {

	if(sys_irqrmpolicy(timerInt.timer.address) != OK){
		printf("ERROR REMOVING POLICY!\n");
		return 1;
	}
	if(sys_irqdisable(timerInt.timer.address) != OK){
		printf("ERROR DISABLING SUBSCRIPTION!\n");
		return 1;
	}

	return 0;
}

void timer_int_handler() {
	timerInt.counter++;
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

	while(1){
		r = driver_receive(ANY, &msg, &ipc_status);
		if( r != 0){
			printf("driver_receive failed with: %d\n", r);
			continue;
		}
		if(is_ipc_notify(ipc_status)){
			switch(_ENDPOINT_P(msg.m_source)){
				case HARDWARE:
					if(msg.NOTIFY_ARG & timerInt.timer.bit){
						printf("COUNTER IS AT %d!\n", timerInt.counter);
					}
					break;
				default:
					break;
			}
		} else {

		}
	}

	return 0;
}
