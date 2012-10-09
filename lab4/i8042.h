// Constants for KBC implementation

typedef struct{
	int hook_id;
} KeyBoardController;

#define KBC_IRQ 	1			// The IRQ line going to be used by the KBC
#define KBC_BIT 	1			// The hook_id for the KBC

#define DELAY_US 	20000		// IBM's specification on the amount of time the i8042 waits for a command
