#include <minix/sysutil.h>
#include "uart.h"

int test_conf(unsigned short base_addr) {
    unsigned long RBR, DLL, IER, DLH, IIR, LCR, MCR, SR;

    // SETTING DLAB TO 0
    uart_write(base_addr, UART_LCR, (LCR & 0x7F));
    // DLAB = 0
    uart_read(base_addr, UART_RBR, &RBR);
    uart_read(base_addr, UART_IER, &IER);
    uart_read(base_addr, UART_IIR, &IIR);
    uart_read(base_addr, UART_LCR, &LCR);
    uart_read(base_addr, UART_MCR, &MCR);
    uart_read(base_addr, UART_SR, &SR);

    // SETTING DLAB TO 1
    uart_write(base_addr, UART_LCR, (LCR | 0x80));
    // DLAB = 1
    uart_read(base_addr, UART_DLL,&DLL);
    uart_read(base_addr, UART_DLH,&DLH);
    // RESETING DLAB TO 0
    uart_write(base_addr, UART_LCR, (LCR & 0x7F));

    printf("RBR: 0x%X\n", RBR);
    printf("IER: 0x%X\n", IER);
    printf("IIR: 0x%X\n", IIR);
    printf("LCR: 0x%X\n", LCR);
    printf("MCR: 0x%X\n", MCR);
    printf("SR: 0x%X\n", SR);
    printf("DLL: 0x%X\n", DLL);
    printf("DLH: 0x%X\n", DLH);

    int wordlength;
    if((LCR&0x03)==0x00)
    	wordlength = 5;
    else if((LCR&0x03)==0x01)
    	wordlength = 6;
    else if((LCR&0x03)==0x02)
    	wordlength = 7;
    else if((LCR&0x03)==0x03)
    	wordlength = 8;
    printf("Word Length Select: %d\n", wordlength);

    int n_stop_b;
    if((LCR&0x04)== 0x00)
    	n_stop_b = 0;
    else if((LCR&0x04)== 0x01)
    	n_stop_b = 1;
    printf("No. of stop bits: %d\n", n_stop_b);

   if(((LCR&0x38) == 0x01))
	   printf("Parity: Odd\n");
   else if((LCR&0x38) == 0x03)
	   printf("Parity: Even\n");
   else if((LCR&0x38) == 0x05)
	   printf("Parity: 1\n");
   else if((LCR&0x38) == 0x07)
	   printf("Parity: 0\n");
   else
	   printf("Parity: None\n");

   unsigned int bitrate, DL;

   DL = DLH;
   DL = DL << 8;
   DL = DL + DLL;
   bitrate = 115200 / DL;

   printf("Bitrate: %d\n", bitrate);

   if((IER&0x01) == 0x01)
	   printf("Rx Interrupts: Enabled\n");
   else
	   printf("Rx Interrupts: Disabled\n");

   if((IER&0x02) == 0x02)
   	   printf("Tx Interrupts: Enabled\n");
   else
	   printf("Tx Interrupts: Disabled\n");

}

int test_set(unsigned short base_addr, unsigned long bits, unsigned long stop, long parity, unsigned long rate) {

	unsigned long LCR, DL;

	// SETTING DLAB TO 0
	uart_write(base_addr, UART_LCR, (LCR & 0x7F));
	// READING LCR
	uart_read(base_addr, UART_LCR, &LCR);

	if(bits == 5)
		LCR = ((LCR >> 2) << 2);
  	else if(bits == 6)
  		LCR = ((LCR >> 2) << 2) + 0x01;
  	else if(bits == 7)
  		LCR = ((LCR >> 2) << 2) + 0x02;
  	else if(bits == 8)
  		LCR = ((LCR >> 2) << 2) + 0x03;
  	else
  	{
  		printf("Wrong value for wordlength\n");
		return -1;
  	}

	if(stop == 1)
		LCR = (LCR & 0xFB);
	else if(stop == 2)
	    LCR = ((LCR & 0xFB) ^ 0x04);
	else
		printf("Wrong value for stop bits\n");

	if(parity == 1)
		LCR = ((LCR&0xC7) ^ 0x08);
	else if(parity == 0)
		LCR = ((LCR&0xC7) ^ 0x18);
	else if(parity == 5)
		LCR = ((LCR&0xC7) ^ 0x28);
	else if(parity == 7)
		LCR = ((LCR&0xC7) ^ 0x38);
	else if(parity == -1)
		LCR = (LCR&0xC7);
	else
		printf("Wrong value for parity\n");

	// WRITE LCR
	uart_write(base_addr, UART_LCR, LCR);

	// SETTING DLAB TO 1
	uart_write(base_addr, UART_LCR, (LCR | 0x80));

	DL = 115200 / rate;

	uart_write(base_addr, UART_DLH, (DL >> 8));
	uart_write(base_addr, UART_DLL, (DL-((DL >> 8)<<8)));

	// SETTING DLAB TO 0
	uart_write(base_addr, UART_LCR, (LCR & 0x7F));
}

int test_poll(unsigned short base_addr, unsigned char tx, unsigned long bits,
			  unsigned long stop, long parity, unsigned long rate, int stringc,
			  char *strings[]) {

	unsigned long LSR;
	unsigned long ch = '\0';

	unsigned int i,aux=0;
	char j='\0';

	uart_read(base_addr, UART_LSR, &LSR);

	test_set(base_addr, bits, stop, parity, rate);

	if(tx == 0){ // Receiver Mode
		while(ch != '.'){
			while( !(LSR & LSR_DATA_READY) ) {
				tickdelay(micros_to_ticks(DELAY_US));
				uart_read(base_addr, UART_LSR, &LSR);
			}

			uart_read(base_addr, UART_RBR, &ch);
			printf("%c", (char)ch);

			if(LSR & LSR_OVERRUN_ERR){
				printf("OVERRUN ERROR!\n");
			}
			if(LSR & LSR_PARITY_ERR){
				printf("PARITY ERROR!\n");
			}
			if(LSR & LSR_FRAMING_ERR){
				printf("FRAMING ERROR!\n");
			}

		}
	}else{ // Transmitter Mode

		for(i=0; i<stringc; i++){
			for(j=strings[i][aux]; j!='\0'; j=strings[i][aux]){
				while( !(LSR & LSR_EMPTY_THR) ) {
					tickdelay(micros_to_ticks(DELAY_US));
					uart_read(base_addr, UART_LSR, &LSR);
				}

				uart_write(base_addr, UART_THR, j);

				aux++;
			}
			aux = 0;
		}
	}
}

int test_int(unsigned short base_addr, unsigned char tx, unsigned long bits,
		  unsigned long stop, long parity, unsigned long rate, int stringc,
		  char *strings[]) {

	if(base_addr == 0x3F8)
		uart_subscribe(0x04);
	else if(base_addr == 0x2F8)
		uart_subscribe(0x03);

	unsigned long IER, IIR, LSR;
	unsigned long ch = '\0';

	unsigned int i,aux=0;
	char j='\0';

	test_set(base_addr, bits, stop, parity, rate);
	uart_read(base_addr, UART_IER, &IER);
	uart_write(base_addr, UART_IER, ((IER&0xF9) ^ 0x06));

	if(tx == 0){ // Receiver Mode
		return 0;
	}else{ // Transmitter Mode

		uart_read(base_addr, UART_IIR, &IIR);
		while(1){
			while(!(IIR&0x01)){
				tickdelay(micros_to_ticks(DELAY_US));
				uart_read(base_addr, UART_IIR, &IIR);
			}
			if((IIR&0x02)&&(!(IIR&0x04))&&(!(IIR&0x08))){
				for(i=0; i<stringc; i++){
					for(j=strings[i][aux]; j!='\0'; j=strings[i][aux]){
						while( !(LSR & LSR_EMPTY_THR) ) {
							tickdelay(micros_to_ticks(DELAY_US));
							uart_read(base_addr, UART_LSR, &LSR);
						}

						uart_write(base_addr, UART_THR, j);

						aux++;
					}
					printf(" ");
					aux = 0;
				}
				printf(".");
				break;
			}
		}
	}
}

int test_fifo(unsigned short base_addr, unsigned char tx, unsigned long bits,
		  unsigned long stop, long parity, unsigned long rate, int stringc,
		  char *strings[]){
    /* To be completed */
}
