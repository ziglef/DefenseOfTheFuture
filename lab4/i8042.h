// Constants for KBC implementation

typedef struct{
	int hook_id;
} KeyBoardController;

// KBC Constants
#define KBC_IRQ 				1			// The IRQ line going to be used by the KBC
#define KBC_BIT 				1			// The hook_id for the KBC
#define KBC_IN_BUF				0x60		// Port for IN_BUF

// C@KDB Outputs
#define ACK						0xFA		// Acknowledgement by the C@KBD
#define RESEND					0xFE		// C@KBD asking for a resend of the data
#define ERROR					0xFC		// C@KBD reporting an error

// Keyboard Commands
#define KBD_RESET				0xFF		// Resets the KBD
#define KBD_DEFAULT_ENABLE		0xF6		// Sets default values on the KBD and enables it
#define KBD_DISABLE				0xF5		// Disables KBD (sets default values and stops scanning)
#define KBD_CLEAR_ENABLE		0xF4		// Clears the buffer and enables KBD
#define	KBD_RATE_DELAY			0xF3		// Changes KBD repetition rate/delay (bits 0-4 for rate and bits 5-6 for delay)
#define KBD_LEDS				0xED		// Turns on/off the KBD LEDs uses bits 0-2
// NOTE: The commands that have arguments must also write them to the IN_BUF and will also be acknowledged.


// Time constant
#define DELAY_US 	20000		// IBM's specification on the amount of time the i8042 waits for a command
