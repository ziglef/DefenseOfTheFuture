#include "uart.h"

int test_conf(unsigned short base_addr) {
    unsigned long RBR, DLL, IER, DLH, IIR, LCR, MCR, SR;

    printf("0x%X\n\n",base_addr);

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
    printf("DLL: 0x%X\n", DLL); // passar para baixo
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
    /* To be completed */
}

int test_poll(/* details to be provided */) { 
    /* To be completed */
}

int test_int(/* details to be provided */) { 
    /* To be completed */
}

int test_fifo(/* details to be provided */) {
    /* To be completed */
}
