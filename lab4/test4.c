#include <minix/syslib.h>
#include <minix/drivers.h>
#include "i8042.h"

typedef struct{
	int hook_id;
} KeyBoardController;

KeyBoardController KBC = {KBC_BIT};

int kbc_subscribe_exclusive(void) {

	if(sys_irqsetpolicy(KBC_IRQ, (IRQ_REENABLE & IRQ_EXCLUSIVE), &(KBC.hook_id)) != OK){
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

int test_scan(void) {
	if(kbc_subscribe_exclusive() < 0){
			printf("ERROR SUBSCRIBING TO KBC!\n");
			return 1;
	}

	/* CODE HERE */

	if(kbc_unsubscribe())
			return 1;
		else
			return 0;
}

int test_leds(unsigned short n, unsigned short *leds) {
    /* To be completed */
}
