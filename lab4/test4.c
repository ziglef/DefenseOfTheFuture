// System Libraries
#include <minix/syslib.h>
#include <minix/drivers.h>
#include <minix/sysutil.h>

// User Libraries
#include "i8042.h"

KeyBoardController KBC = {KBC_BIT};
unsigned char scancode;

int kbc_subscribe_exclusive(void) {

	if(sys_irqsetpolicy(KBC_IRQ, (IRQ_REENABLE | IRQ_EXCLUSIVE), &(KBC.hook_id)) != OK){
		printf("ERROR SETTING POLICY!\n");
		return -1;
	}
	if(sys_irqenable(&(KBC.hook_id)) != OK){
		printf("ERROR ENABLING SUBSCRIPTION!\n");
		return -1;
	}

	return KBC.hook_id;
}

int kbc_unsubscribe() {

	if(sys_irqrmpolicy(&(KBC.hook_id)) != OK){
		return 1;
	}
	if(sys_irqdisable(&(KBC.hook_id)) != OK){
		printf("ERROR DISABLING SUBSCRIPTION!\n");
		return 1;
	}

	return 0;
}

void kbc_handler(unsigned char code) {
	scancode = code;

	if(scancode & BREAKCODE_MASK)
		printf("Breakcode: 0x%X\n", code);
	else
		printf("Makecode: 0x%X\n", code);
}

int test_scan(void) {
	if(kbc_subscribe_exclusive() < 0){
			printf("ERROR SUBSCRIBING TO KBC!\n");
			return 1;
	}

	/* CODE HERE */
	// When implemented the code should wait 20ms for each command. For that purpose use: tickdelay(micros_to_ticks(DELAY_US));
	/* Code @lab3/timer.c for reference
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
						timer_int_handler();
					}
					break;
				default:
					break;
			}
		} else {

		}
		timerInt.counter++;
	}
	 */




	if(kbc_unsubscribe())
			return 1;
		else
			return 0;
}

int test_leds(unsigned short n, unsigned short *leds) {
	/* CODE HERE */
}
