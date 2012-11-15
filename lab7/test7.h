int test_conf(unsigned short base_addr);
/* Parity info - -1: none, 0: even, 1: odd */
int test_set(unsigned short base_addr, unsigned long bits, unsigned long stop, long parity, unsigned long rate);
int test_poll(unsigned short base_addr, unsigned char tx, unsigned long bits, unsigned long stop, long parity, unsigned long rate, int stringc, char *strings[]);
int test_int(unsigned short base_addr, unsigned char tx, unsigned long bits, unsigned long stop, long parity, unsigned long rate, int stringc, char *strings[]);
int test_fifo(unsigned short base_addr, unsigned char tx, unsigned long bits, unsigned long stop, long parity, unsigned long rate, int stringc, char *strings[]);
