int test_scan(void);
int test_leds(unsigned short n, unsigned short *leds);
int kbc_subscribe_exclusive(void);
int kbc_unsubscribe();
void kbc_handler(unsigned char scancode);
int kbc_send_command(unsigned char cmd);
