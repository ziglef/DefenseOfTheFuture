int test_conf(unsigned short base_addr);
/* Parity info - -1: none, 0: even, 1: odd */
int test_set(unsigned short base_addr, unsigned long bits, unsigned long stop, long parity, unsigned long rate);
int test_poll(/* details to be provided */);
int test_int(/* details to be provided */);
int test_fifo(/* details to be provided */);