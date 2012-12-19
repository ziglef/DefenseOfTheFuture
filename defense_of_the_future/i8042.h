/* Notes on the i8042: All commands must
 * be called using IN/OUT assembly instructions
 * or the library functions
 * Sys_inb()/sys_outb() of the kernel API */

#define BIT(n) 					(0x01<<(n))

#define TWOBYTE_CODE			0xE0
#define ESC_BREAKCODE 			0x81
#define NO_OF_TRIES				25
#define BREAKCODE_MASK			BIT(7)
#define KBC_BIT_MASK			BIT(0)
#define MOUSE_BIT_MASK			BIT(5)

// MOUSE Constants
#define M_IRQ					0xC

// KBC Constants
#define KBC_IRQ 				0x01		// The IRQ line going to be used by the KBC
#define KBC_O_BUF				0x60		// Port for OUT_BUF
#define KBC_I_BUF				0x64		// Port for the IN_BUF
#define KBC_CMD					0x64		// Port used for issuing commands to the KBC (here arguments should be passed in the KBC_IO_BUF address)
#define KBC_STAT				0x64		// Port for the STAT_REG
/* KBC STAT_REG DATA:
 * BIT 0: OBF If set, the OUT_BUF is full, thus data is available for reading.
 * BIT 1: IBF If set, the IN_BUF is full, thus commands and arguments shouldn't be written.
 * BIT 2: SYS 0 when system is in power-on reset, 1 when system already initialized.
 * BIT 3: A2  Tells us if the A2 input line is a data byte(0) or a command byte(1).
 * BIT 4: INH 0 if keyboard is inhibited, 1 otherwise.
 * BIT 5: Aux Mouse data.
 * BIT 6: Timeout If set means Timeout error, invalid data.
 * BIT 7: Parity If set means Parity error - invalid data.
 */
#define KBC_STAT_OBF 			BIT(0)
#define KBC_STAT_IBF 			BIT(1)
#define KBC_STAT_SYS			BIT(2)
#define KBC_STAT_A2				BIT(3)
#define KBC_STAT_INH			BIT(4)
#define KBC_STAT_AUX			BIT(5)
#define KBC_STAT_TIMEOUT		BIT(6)
#define KBC_STAT_PARITY			BIT(7)

// KBC Commands - Must be written to KBC_STATUS port, arguments if any are passed in KBC_IO_BUF by the IN_BUF, return values are passed in KBC_IO_BUF by the OUT_BUF
#define	KBC_READ				0x20		// Reads command byte, returns command byte
#define KBC_WRITE				0x60		// Writes command byte
#define KBC_CHECK				0xAA		// Self-Test, returns KBC_OK if OK, KBC_ERROR if ERROR
#define KBC_INT					0xAB		// Checks keyboard interface, returns 0 if OK
#define KBC_DIS_INT				0xAD		// Disables KBD Interface
#define KBC_EN_INT				0xAE		// Enables KBD Interface

// KBC_CHECK returns
#define KBC_OK					0x55		// The KBC is OK
#define KBC_ERROR				0xFC		// The KBC is not OK

// C@KBD Outputs
#define CKBD_ACK				0xFA		// Acknowledgement by the C@KBD
#define CKBD_RESEND				0xFE		// C@KBD asking for a resend of the data
#define CKBD_ERROR				0xFC		// C@KBD reporting an error

// Keyboard Port
#define KBD_PORT				0x60		// Keyboard Port used for issuing commands and receiving aknowlegement

// Keyboard Commands
#define KBD_RESET				0xFF		// Resets the KBD
#define KBD_DEFAULT_ENABLE		0xF6		// Sets default values on the KBD and enables it
#define KBD_DISABLE				0xF5		// Disables KBD (sets default values and stops scanning)
#define KBD_CLEAR_ENABLE		0xF4		// Clears the buffer and enables KBD
#define	KBD_RATE_DELAY			0xF3		// Changes KBD repetition rate/delay (bits 0-4 for rate and bits 5-6 for delay)
#define KBD_LEDS				0xED		// Turns on/off the KBD LEDs uses bits 0-2
// NOTE: The commands that have arguments must also write them to the IN_BUF and will also be acknowledged.
// Always use KBC_IO_BUF for KBD commands


// Time constant
#define DELAY_US 				20000		// IBM's specification on the amount of time the i8042 waits for a command
