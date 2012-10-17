unsigned short mousecounter = 0;

typedef struct{
	int hook_id;
	unsigned long status;
	unsigned long data;
	unsigned char bytes[3];
} MouseController;
