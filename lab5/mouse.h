typedef struct{
	int hook_id;
	unsigned int initialized;
	unsigned long status;
	unsigned long data;
	unsigned int pos;
	unsigned char bytes[3];
} MouseController;

void return_vars(unsigned short *counter, MouseController *mmouse);
int mouse_subscribe_exclusive();
int mouse_unsubscribe();
int mouse_handler();
int mouse_read();
int mouse_send(unsigned long port, unsigned char cmd);
