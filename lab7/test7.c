include

int test_conf(unsigned short base_addr) {
    unsigned long RBR, DLL, IER, DLH, IIR, LCR, MCR, SR;

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

    printf("\n");
    printf("\n");
    printf("\n");
    printf("\n");
    printf("\n");
    printf("\n");
    printf("\n");
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
