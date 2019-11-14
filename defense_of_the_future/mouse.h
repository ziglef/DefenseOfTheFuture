typedef struct{
	int hook_id;
	unsigned int initialized;
	unsigned short counter;
	uint32_t status;
	uint32_t data;
	uint32_t pos;
	uint8_t bytes[3];
} MouseController;

void return_vars(unsigned short *counter, MouseController *mmouse, unsigned char *packet);
int mouse_subscribe_exclusive();
int mouse_unsubscribe();
MouseController mouse_handler();
int mouse_read();
int mouse_send(unsigned long port, unsigned char cmd);
void turn_mouse_on();
void turn_mouse_off();
