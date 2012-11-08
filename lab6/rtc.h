// RTC DEFINES AND CONSTANTS

#define BIT(n) 					(0x01<<(n))

// RTC PORTS
#define RTC_ADDRESS_REG			0x70
#define RTC_DATA_REG			0x71

// RTC IRQ
#define RTC_IRQ 				8

// RTC HOOK_ID BITMASK
#define RTC_BIT_MASK 			BIT(1)

// RTC TIME REGISTERS
#define RTC_SECONDS 			0			// Range 0-59
#define RTC_MINUTES 			2			// Range 0-59
#define RTC_HOURS 				4			// Range 1-12 || 0-23
#define RTC_DOW 				6			// Range 1-7 (Sunday = 1)
#define RTC_DOM 				7			// Range 1-31
#define RTC_MONTH 				8 			// Range 1-12
#define RTC_YEAR 				9			// Range 0-99

// RTC REGISTER A
#define RTC_REGA				10
#define RTC_UIP					BIT(7)		// If 1, do not access, update in progress
#define RTC_DV2					BIT(6)		// Control the counting chain
#define RTC_DV1					BIT(5)		// Control the counting chain
#define RTC_DV0					BIT(4)		// Control the counting chain
#define RTC_RS3					BIT(3)		// Rate selector - periodic interrupts && square wave output
#define RTC_RS2					BIT(2)		// Rate selector - periodic interrupts && square wave output
#define RTC_RS1					BIT(1)		// Rate selector - periodic interrupts && square wave output
#define RTC_RS0					BIT(0)		// Rate selector - periodic interrupts && square wave output

// RTC REGISTER B
// IMPORTANT: DO NOT CHANGE DM, 24/12 OR DSE
#define RTC_REGB				11
#define RTC_SET					BIT(7)		// Set to 1, inhibit update of time/date regs
#define RTC_PIE					BIT(6)		// Set to 1, enable the corresponding interrupt source
#define RTC_AIE					BIT(5)		// Set to 1, enable the corresponding interrupt source
#define RTC_UIE					BIT(4)		// Set to 1, enable the corresponding interrupt source
#define RTC_SQWE				BIT(3)		// Set to 1, enable square-wave generation
#define RTC_DM					BIT(2)		// Set to 1, sets time, alarm and date regs in binary/ Set to 0, BCD
#define RTC_2412				BIT(1)		// Set to 1, sets hours range 0-23/ Set to 0, sets hours range 1-12
#define RTC_DSE					BIT(0)		// Set to 1, enable Daylight Savings Time/ Set to 0, disable

// RTC REGISTER C
#define RTC_REGC				12
#define RTC_IRQF				BIT(7)		// IRQ line active
#define RTC_PF					BIT(6)		// Periodic interrupt pending
#define RTC_AF					BIT(5)		// Alarm interrupt pending
#define RTC_UF					BIT(4)		// Update interrupt pending
#define RTC_C_3					BIT(3)		// Always 0
#define RTC_C_2					BIT(2)		// Always 0
#define RTC_C_1					BIT(1)		// Always 0
#define RTC_C_0					BIT(0)		// Always 0

// RTC REGISTER D
#define RTC_REGD				13
#define RTC_VRT					BIT(7)		// Valid RAM/time - Set to 0, internal battery runs out of energy - RTC readings are questionable
#define RTC_D_6					BIT(6)		// Always 0
#define RTC_D_5					BIT(5)		// Always 0
#define RTC_D_4					BIT(4)		// Always 0
#define RTC_D_3					BIT(3)		// Always 0
#define RTC_D_2					BIT(2)		// Always 0
#define RTC_D_1					BIT(1)		// Always 0
#define RTC_D_0					BIT(0)		// Always 0


void rtc_handler();
void enable();
void disable();
void rtc_read(unsigned long reg, unsigned long *byte);
void rtc_write(unsigned long reg, unsigned long byte);
