#include "uart.h"

int test_conf(unsigned short base_addr) {
    unsigned long RBR, DLL, IER, DLH, IIR, LCR, MCR, SR;

    // SETTING DLAB TO 0
    uart_write(base_addr, UART_LCR, (LCR & 0x7F));
    // DLAB = 0
    uart_read(base_addr, UART_RBR, RBR);
    uart_read(base_addr, UART_IER, IER);
    uart_read(base_addr, UART_IIR, IIR);
    uart_read(base_addr, UART_LCR, LCR);
    uart_read(base_addr, UART_MCR, MCR);
    uart_read(base_addr, UART_SR, SR);

    // SETTING DLAB TO 1
    uart_write(base_addr, UART_LCR, (LCR | 0x80));
    // DLAB = 1
    uart_read(base_addr, UART_DLL,DLL);
    uart_read(base_addr, UART_DLH,DLH);
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
