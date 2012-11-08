// RTC DEFINES AND CONSTANTS

#define BIT(n) 					(0x01<<(n))

// RTC PORTS
#define RTC_ADDRESS_REG			0x70
#define RTC_DATA_REG			0x71

// RTC REGISTER A
#define RTC_REGA				10
#define RTC_UIP					BIT(7)
#define RTC_DV2					BIT(6)
#define RTC_DV1					BIT(5)
#define RTC_DV0					BIT(4)
#define RTC_RS3					BIT(3)
#define RTC_RS2					BIT(2)
#define RTC_RS1					BIT(1)
#define RTC_RS0					BIT(0)

// RTC REGISTER B
#define RTC_REGA				11
#define RTC_SET					BIT(7)
#define RTC_PIE					BIT(6)
#define RTC_AIE					BIT(5)
#define RTC_UIE					BIT(4)
#define RTC_SQWE				BIT(3)
#define RTC_DM					BIT(2)
#define RTC_2412				BIT(1)
#define RTC_DSE					BIT(0)

// RTC REGISTER C
#define RTC_REGA				12
#define RTC_IRQF				BIT(7)
#define RTC_PF					BIT(6)
#define RTC_AF					BIT(5)
#define RTC_UF					BIT(4)
#define RTC_C_3					BIT(3)
#define RTC_C_2					BIT(2)
#define RTC_C_1					BIT(1)
#define RTC_C_0					BIT(0)

// RTC REGISTER D
#define RTC_REGA				13
#define RTC_VRT					BIT(7)
#define RTC_D_6					BIT(6)
#define RTC_D_5					BIT(5)
#define RTC_D_4					BIT(4)
#define RTC_D_3					BIT(3)
#define RTC_D_2					BIT(2)
#define RTC_D_1					BIT(1)
#define RTC_D_0					BIT(0)


void enable();
void disable();
void rtc_read(unsigned long reg, unsigned long *byte);
void rtc_write(unsigned long reg, unsigned long byte);
