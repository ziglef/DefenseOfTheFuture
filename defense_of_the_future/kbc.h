typedef struct{
	int hook_id;
	uint32_t status;
	uint32_t data;
} KeyBoardController;

int kbc_subscribe_exclusive(void);

int kbc_unsubscribe();

int kbc_handler();

int kbc_read();

int kbc_send_command(unsigned long port, unsigned char cmd);

int send_kbc_command(unsigned char cmd);
