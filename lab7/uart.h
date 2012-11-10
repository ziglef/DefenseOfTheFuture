#include <minix/syslib.h>
#include <minix/drivers.h>
#include <minix/sysutil.h>

// FUNCTIONS
void uart_read(unsigned short base_addr, unsigned short OFFSET, unsigned long *byte);
void uart_write(unsigned short base_addr, unsigned short OFFSET, unsigned long byte);

// BIT FUNCTION
#define BIT(n)					(0x01 << n)

// UART DEFINES AND INCLUDES
// UART
#define UART_IRQ_COM1			4		// IRQ LINE FOR SERIAL @ COM1
#define UART_IRQ_COM2			3		// IRQ LINE FOR SERIAL @ COM2

#define UART_COM1_BASE			0x3F8	// COM1 BASE ADDRESS FOR I/O PORTS
#define UART_COM2_BASE			0x2F8	// COM2 BASE ADDRESS FOR I/O PORTS

// UART REGISTERS
#define UART_THR				0		// MODE: W   | DLAB: 0 | TRANSMITTER HOLDING BUFFER
#define UART_RBR				0		// MODE: R   | DLAB: 0 | RECEIVER BUFFER
#define UART_DLL				0		// MODE: R/W | DLAB: 1 | DIVISOR LATCH LOW BYTE
#define UART_DLH				1		// MODE: R/W | DLAB: 1 | DIVISOR LATCH HIGH BYTE
#define UART_IER				1		// MODE: R/W | DLAB: 0 | INTERRUPT ENABLE REGISTER
#define UART_IIR				2		// MODE: R   | DLAB: X | INTERRUPT IDENTIFICATION REGISTER
#define UART_FCR				2		// MODE: W   | DLAB: X | FIFO CONTROL REGISTER
#define UART_LCR				3		// MODE: R/W | DLAB: X | LINE CONTROL REGISTER
#define UART_MCR				4		// MODE: R/W | DLAB: X | MODEM CONTROL REGISTER
#define UART_LSR				5		// MODE: R   | DLAB: X | LINE STATUS REGISTER
#define UART_MSR				6		// MODE: R   | DLAB: X | MODEM STATUS REGISTER
#define UART_SR					7		// MODE: R/W | DLAB: X | SCRATCH REGISTER
// DIVISOR LATCH VALUE = 115200/BAUDRATE

/* INTERRUPT ENABLE REGISTER (IER)
 * BIT(0) - ENABLE RECEIVED DATA AVAILABLE INTERRUPT
 * BIT(1) - ENABLE TRANSMITTER HOLDING REGISTER EMPTY INTERRUPT
 * BIT(2) - ENABLE RECEIVER LINE STATUS INTERRUPT
 * BIT(3) - ENABLE MODEM STATUS INTERRUPT
 * BIT(4) - ENABLES SLEEP MODE (16750)
 * BIT(5) - ENABLES LOW POWER MODE (16750)
 * BIT(6) - RESERVED
 * BIT(7) - RESERVED
 */

/* INTERRUPT IDENTIFICATION REGISTER (IIR)
 * BIT(0) - INTERRUPT PENDING FLAG
 * BIT(3,2,1) -
 * 0,0,0 - MODEM STATUS INTERRUPT - RESET METHOD: READING MODEM STATUS REGISTER (MSR)
 * 0,0,1 - TRANSMITTER HOLDING REGISTER EMPTY INTERRUPT - RESET METHOD: READING INTERRUPT IDENTIFICATION REGISTER (IIR) || WRITING TO TRANSMIT HOLDING BUFFER (THR)
 * 0,1,0 - RECEIVED DATA AVAILABLE INTERRUPT - RESET METHOD: READING RECEIVE BUFFER REGISTER (RBR)
 * 0,1,1 - RECEIVER LINE STATUS INTERRUPT - RESET METHOD: READING LINE STATUS REGISTER (LSR)
 * 1,0,0 - RESERVED - RESET METHOD: N/A
 * 1,0,1 - RESERVED - RESET METHOD: N/A
 * 1,1,0 - TIME-OUT INTERRUPT PENDING (16550 & LATER) - RESET METHOD: READING RECEIVE BUFFER REGISTER (RBR)
 * 1,1,1 - RESERVED - RESET METHOD: N/A
 * BIT(4) - RESERVED
 * BIT(5) - 64 BYTE FIFO ENABLED (16750 ONLY)
 * BIT(7,6) -
 * 0,0 - NO FIFO ON CHIP
 * 0,1 - RESERVED CONDITION
 * 1,0 - FIFO ENABLED, BUT NOT FUNCTIONING
 * 1,1 - FIFO ENABLED
 */

/* FIFO CONTROL REGISTER (FCR)
 * BIT(0) - ENABLE FIFOs
 * BIT(1) - CLEAR RECEIVE FIFO
 * BIT(2) - CLEAR TRANSMIT FIFO
 * BIT(3) - DMA MODE SELECT
 * BIT(4) - RESERVED
 * BIT(5) - ENABLE 64 BYTE FIFO (16750)
 * BIT(7,6) -
 * 0,0 - INTERRUPT TRIGGER LEVEL (16 BYTE) - 1 BYTE - TRIGGER LEVEL (64 BYTE) - 1 BYTE
 * 0,1 - INTERRUPT TRIGGER LEVEL (16 BYTE) - 4 BYTE - TRIGGER LEVEL (64 BYTE) - 16 BYTE
 * 1,0 - INTERRUPT TRIGGER LEVEL (16 BYTE) - 8 BYTE - TRIGGER LEVEL (64 BYTE) - 32 BYTE
 * 1,1 - INTERRUPT TRIGGER LEVEL (16 BYTE) - 14 BYTE - TRIGGER LEVEL (64 BYTE) - 56 BYTE
 */

/* LINE CONTROL REGISTER (LCR)
 * BIT(1,0) -
 * 0,0 - WORD LENGTH - 5 BITS
 * 0,1 - WORD LENGTH - 6 BITS
 * 1,0 - WORD LENGTH - 7 BITS
 * 1,1 - WORD LENGTH - 8 BITS
 * BIT(2) -
 * 0 - ONE STOP BIT
 * 1 - 1.5 STOP BITS OR 2 STOP BITS
 * BIT(5,4,3) -
 * 0,0,0 - PARITY SELECT - NO PARITY
 * 0,0,1 - PARITY SELECT - ODD PARITY
 * 0,1,1 - PARITY SELECT - EVEN PARITY
 * 1,0,1 - PARITY SELECT - MARK
 * 1,1,1 - PARITY SELECT - SPACE
 * BIT(6) - SET BREAK ENABLE
 * BIT(7) - DIVISOR LATCH ACCESS BIT
 */

/* MODEM CONTROL REGISTER (MCR)
 * BIT(0) - DATA TERMINAL READY
 * BIT(1) - REQUEST TO SEND
 * BIT(2) - AUXILIARY OUTPUT 1
 * BIT(3) - AUXILIARY OUTPUT 2
 * BIT(4) - LOOPBACK MODE
 * BIT(5) - AUTOFLOW CONTROL ENABLED (16750)
 * BIT(6) - RESERVED
 * BIT(7) - RESERVED
 */

/* LINE STATUS REGISTER (LSR)
 * BIT(0) - DATA READY
 * BIT(1) - OVERRUN ERRO
 * BIT(2) - PARITY ERROR
 * BIT(3) - FRAMING ERROR
 * BIT(4) - BREAK INTERRUPT
 * BIT(5) - EMPTY TRANSMITTER HOLDING REGISTER
 * BIT(6) - EMPTY DATA HOLDING REGISTER
 * BIT(7) - ERROR IN RECEIVED FIFO
 */

/* MODEM STATUS REGISTER (MSR)
 * BIT(0) - DELTA CLEAR TO SEND
 * BIT(1) - DELTA DATA SET READY
 * BIT(2) - TRAILING EDGE RING INDICATOR
 * BIT(3) - DELTA DATA CARRIER DETECT
 * BIT(4) - CLEAR TO SEND
 * BIT(5) - DATA SET READY
 * BIT(6) - RING INDICATOR
 * BIT(7) - CARRIOR DETECT
 */
